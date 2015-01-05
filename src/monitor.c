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
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "monitor.h"
#include "md5.h"
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

   return E_SUCCESS;
}

/* This function is a modified version of the one found in bacula in src/lib/cram-md5.c */
int authenticate_monitor(monitor * mon)
{   
   char hello[] = "Hello *UserAgent* calling\n";
   int tls, compatible, err;
      
   /* Send the initial hello message */
   err = send_message(mon, hello, strlen(hello));
   if(err != E_SUCCESS) {
      return err;
   }

   /* Get the cram challenge and respond */
   {
      char resp[MAXSTRING];
      char chal[MAXSTRING];
      char msg[MAXSTRING];
      uint8_t hmac[20];
      size_t len;

      err = receive_message(mon, resp, MAXSTRING);
      if(err < E_SUCCESS) {
         return err;
      }

      compatible = 0;
      if(sscanf(resp, "auth cram-md5c %s ssl=%d", chal, &tls) == 2) {
         compatible = 1;
      } else if(sscanf(resp, "auth cram-md5 %s ssl=%d", chal, &tls) != 2) {
         if(sscanf(resp, "auth cram-md5 %s\n", chal) != 1) {
            return E_MONITOR_BAD_SCAN_CHALLENGE;
         }
      }

      hmac_md5((uint8_t *)chal, strlen(chal), (uint8_t *)mon->passwd, strlen(mon->passwd), hmac);
      len = bin_to_base64(msg, 50, (char *)hmac, 16, compatible) + 1;
      
      err = send_message(mon, msg, len);
      if(err != E_SUCCESS) {
         return err;
      }

      err = receive_message(mon, resp, MAXSTRING);
      if(err < E_SUCCESS) {
         return err;
      }
      
      if(strcmp(resp, "1000 OK auth\n") != 0) {
         return E_MONITOR_AUTH_FAILED;
      }
   }

   /* send our cram challenge */
   {
      int i;
      struct timeval t1;
      struct timeval t2;
      struct timezone tz;
      char chal[MAXSTRING];
   
      gettimeofday(&t1, &tz);
      for(i = 0; i < 4; ++i) {
         gettimeofday(&t2, &tz);
      }

      srandom((t1.tv_sec&0xffff) * (t2.tv_usec&0xff));
      snprintf(chal, sizeof(chal), "auth cram-md5 <%u.%u@%s> ssl=0\n", 
               (uint32_t)random(), (uint32_t)time(NULL), mon->host_name);
   }

/*   err = receive_message(mon, resp, MAXSTRING);
   if(err != E_SUCCESS) {
      return err;
   }
*/
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

int send_message(monitor * mon, const char * msg, size_t msglen)
{
   int32_t buf[MAX_BUF_LEN] = {0};
   size_t buflen, sendlen;

   if(mon->sock == 0) {
      return E_MONITOR_NOT_CONNECTED;
   }

   buflen = msglen + sizeof(int32_t);

   *buf = htonl(msglen);
   memcpy((void*)(buf+1), msg, MAX_BUF_LEN);

   sendlen = send(mon->sock, buf, buflen, 0);
   if(sendlen != buflen) {
      
      return E_MONITOR_BAD_SEND;
   }

   return E_SUCCESS;
}

int receive_message(monitor * mon, char * msg, int msglen)
{
   size_t size;
   fd_set readfds;
   int res, recvlen;
   int32_t tmp[MAXSTRING];

   struct timeval tv;
   tv.tv_sec = DEFAULT_MONITOR_TIMEOUT;
   tv.tv_usec = 0;

   if(mon->sock == 0) {
      return E_MONITOR_NOT_CONNECTED;
   }

   FD_ZERO(&readfds);
   FD_SET(mon->sock, &readfds);
   
   res = select(mon->sock+1, &readfds, (fd_set*)0, (fd_set*)0, &tv);
   if(res < 0) {
      return E_MONITOR_BAD_SELECT;
   } else if(res == 0) {
      return E_MONITOR_RECV_TIMEOUT;
   }
   
   size = recv(mon->sock, (void*)tmp, msglen, 0);
   if(size == 0) {
      return E_MONITOR_NO_MSG;
   }

   if(size == -1) {
      return E_MONITOR_BAD_RECV;
   }

   recvlen = ntohl(*tmp);
   if(recvlen < 0)
   {
      return E_MONITOR_BAD_RECV;
   }

   memcpy(msg, (void*)(tmp+1), recvlen);

   return recvlen;
}


