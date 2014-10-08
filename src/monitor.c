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
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
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

int connect_monitor(monitor * mon)
{
   /* kill any connections that already exist */
   disconnect_monitor(mon);

   if((mon->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      sprintf(error_str, "%s", "Could not create new socket ");
      return 1;
   }

   mon->sin.sin_family = AF_INET;
   mon->sin.sin_port = htons(mon->portno);

   /* Convert hostname to ip address */
   { 
      struct hostent *he;
      struct in_addr **addr_list;
      int x;

      if((he = gethostbyname(mon->director_host_name)) == NULL) {
         return 2;
      }

      addr_list = (struct in_addr**) he->h_addr_list;
      for(x = 0; addr_list[x] != NULL; x++) {
         strncpy((char*)mon->director_host_ip, inet_ntoa(*addr_list[x]), 20);
      }
   }
      

   if(inet_pton(AF_INET, mon->director_host_ip, &(mon->sin.sin_addr)) <= 0) {
      sprintf(error_str, "Error with hostname `%s', ip address `%s`", 
              mon->director_host_name,
              inet_ntoa(mon->sin.sin_addr));
      return 3;
   }

   if(connect(mon->sock, (struct sockaddr *)&mon->sin, sizeof(mon->sin)) < 0) {
      sprintf(error_str, "Unable to connect to hostname `%s'", mon->director_host_name);
      return 4;
   }

   return 0;
}

void disconnect_monitor(monitor * mon)
{
   if(mon->sock > 0) {
      shutdown(mon->sock, SHUT_RDWR);
      mon->sock = 0;
      memset(&(mon->sin), 0, sizeof(mon->sin));
   }
}

int send_message(monitor * mon, const char * cmd)
{
   return 0;
}

char * receive_message(monitor * mon)
{
   return NULL;
}
