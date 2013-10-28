/*****************************************************************************/ 
/* baculua.h for baculua                                                     */
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
#include <netinet/in.h>

const char * BACULA_MON_NAME = "bacula_mon";
const unsigned int DEFAULT_DIRECTOR_PORTNO = 9101;

typedef struct {	 
	 /* name of corresponding lua class */
	 const char * class_name;

	 /* connection information */
	 const char * director_name;
	 const char * director_host_name;
	 const char * client_name;
	 const char * passwd;
	 unsigned int portno;

	 /* sockets and whatnot */
	 int sock;
	 struct sockaddr_in sockInfo;
} bacula_mon;

static int baculua_get_monitor(lua_State * L);
static int baculua_director_status(lua_State * L);
static int baculua_messages(lua_State * L);
