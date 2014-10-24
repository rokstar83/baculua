/*****************************************************************************/ 
/* monitor.c for baculua                                                     */
/* Copyright (c) 2013 Tom Hartman (rokstar83@gmail.com)                      */
/*                                                                           */
/* This program is free software; you can redistribute it and/or             */
/* modify it under the terms of the GNU General Public License               */
/* as published by the Free Software Foundation; either version 2            */
/* of the License, or the License, or (at your option) any later             */
/* version.                                                                  */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*****************************************************************************/
#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include "monitor.h"
#include "baculua_error.h"

/* Assumes that the top of the stack is the monitor class */ 
int lua_checkmonitor(lua_State *L, monitor * mon)
{
   if(!lua_istable(L, -1)) {
      sprintf(error_str, "bad parameter, expected %s", "baculua_mon");
      return 1;
   }

   lua_pushstring(L, "director_name");
   lua_gettable(L, -2);
   mon->director_name = luaL_checkstring(L, -1);
   lua_pop(L, 1);

   lua_pushstring(L, "director_host_name");
   lua_gettable(L, -2);
   mon->director_host_name = luaL_checkstring(L, -1);
   lua_pop(L, 1);

   lua_pushstring(L, "client_name");
   lua_gettable(L, -2);
   mon->client_name = luaL_checkstring(L, -1);
   lua_pop(L, 1);

   lua_pushstring(L, "passwd");
   lua_gettable(L, -2);
   mon->passwd = luaL_checkstring(L, -1);
   lua_pop(L, 1);

   lua_pushstring(L, "portno");
   lua_gettable(L, -2);
   mon->portno = luaL_checknumber(L, -1);
   lua_pop(L, 1);
   
   return 0;
}

void init_monitor(monitor * mon)
{
   mon->sock = 0;
   mon->director_name = "";
   mon->director_host_name = "";
   mon->portno = DEFAULT_DIRECTOR_PORTNO;
}

int connect_monitor(monitor * mon, int timeout)
{
   /* kill any connections that already exist */
   disconnect_monitor(mon);

   if((mon->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      return E_MONITOR_NO_SOCKET;
   }

   mon->sin.sin_family = AF_INET;
   mon->sin.sin_port = htons(mon->portno);

   /* Convert hostname to ip address */
   { 
      struct hostent *he;
      struct in_addr **addr_list;
      int x;

      if((he = gethostbyname(mon->director_host_name)) == NULL) {
        return E_MONITOR_NO_HOST;
      }

      addr_list = (struct in_addr**) he->h_addr_list;
      for(x = 0; addr_list[x] != NULL; x++) {
         strncpy((char*)mon->director_host_ip, inet_ntoa(*addr_list[x]), 20);
      }
   }
      
   
   if(inet_pton(AF_INET, mon->director_host_ip, &(mon->sin.sin_addr)) <= 0) {
      return E_MONITOR_ADDY_CONVERT;
   }

   /* Set to nonblocking */
   {
      long arg;
      
      if((arg = fcntl(mon->sock, F_GETFL, NULL)) < 0) {
         return E_MONITOR_GET_SOCK_FLAG;
      }

      arg |= O_NONBLOCK;
      
      if(fcntl(mon->sock, F_SETFL, arg) < 0) {
         return E_MONITOR_SET_SOCK_FLAG;
      }
   }
   
   /* connect to monitor */
   {
      struct timeval tv;
      fd_set readfds, writefds;
      int res;

      tv.tv_sec = timeout;
      tv.tv_usec = 0;

      res = connect(mon->sock, (struct sockaddr *)&mon->sin, sizeof(mon->sin));

      if(res < 0) {
         if(errno == EINPROGRESS) {
            do {
               FD_ZERO(&readfds);
               FD_ZERO(&writefds);
               FD_SET(mon->sock, &readfds);
               FD_SET(mon->sock, &writefds);
               res = select(mon->sock+1, &readfds, &writefds, NULL, &tv);
               if(res < 0 && errno != EINTR) { /* Error while connecting */
                  return E_MONITOR_SELECT;
               } else if(errno == EINTR) {
                  continue;
               } else if(res > 0) { /* socket selected */
                  break;
               } else { /* timeout */
                  return E_MONITOR_TIMEOUT;
               }
            } while (1);
         }
      }      
   }

   /* Reset socket back to blocking */
   {
      long arg;
      if((arg = fcntl(mon->sock, F_GETFL, NULL)) < 0) {
         return E_MONITOR_GET_SOCK_FLAG;
      }
      
      arg &= (~O_NONBLOCK);
      if(fcntl(mon->sock, F_SETFL, arg) < 0) {
         return E_MONITOR_SET_SOCK_FLAG;
      }
   }

   return E_SUCCESS;
}

void disconnect_monitor(monitor * mon)
{
   if(mon->sock > 0) {
      shutdown(mon->sock, SHUT_RDWR);
      mon->sock = 0;
      memset(&(mon->sin), 0, sizeof(mon->sin));
   }
}

int send_message(monitor * mon, const char * msg)
{
   int32_t buf[MAX_BUF_LEN] = {0};
   int32_t * tmp;
   size_t buflen, msglen, sendlen;

   if(mon->sock == 0) {
      return E_MONITOR_NOT_CONNECTED;
   }

   msglen = strlen(msg);
   buflen = msglen + sizeof(int32_t);

   *buf = htonl(msglen);
   tmp = buf+1;
   memcpy((void*)tmp, msg, MAX_BUF_LEN);

   sendlen = send(mon->sock, buf, buflen, 0);
   if(sendlen != buflen) {
      
      return E_MONITOR_BAD_SEND;
   }

   return E_SUCCESS;
}

int receive_message(monitor * mon, char * msg, int msglen)
{
   int size;
   if(mon->sock == 0) {
      return E_MONITOR_NOT_CONNECTED;
   }

   size = recv(mon->sock, (void*)msg, msglen, 0);
   if(size == 0) {
      return E_MONITOR_NO_MSG;
   }

   if(size == -1) {
      return E_MONITOR_BAD_RECV;
   }

   return E_SUCCESS;
}
