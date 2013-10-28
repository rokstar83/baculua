/*****************************************************************************/ 
/* baculua.c for baculua                                                     */
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
#include <stdlib.h>
#include "baculua_error.h"
#include "baculua.h"
#include "util.h"

/* registration structure */
static const struct luaL_reg baculua_lib[] =
{
	 {"get_monitor", baculua_get_monitor},
	 {"director_status", baculua_director_status},
	 {"messages", baculua_messages},
	 {NULL, NULL},
};

/* get_monitor(director_name, host_name, client_name, passwd, [portno]) */
static int baculua_get_monitor(lua_State * L)
{
	 bacula_mon * monitor = malloc(sizeof(bacula_mon));
	 int count = lua_gettop(L);
	 
	 monitor->class_name = BACULA_MON_NAME;
	 monitor->director_name = luaL_checkstring(L, 1);
	 monitor->director_host_name = luaL_checkstring(L, 2);
	 monitor->client_name = luaL_checkstring(L, 3);
	 monitor->passwd = luaL_checkstring(L, 4);
	 monitor->portno = (count > 4 ? luaL_checknumber(L, 5) : DEFAULT_DIRECTOR_PORTNO);

	 lua_newtable(L);

	 lua_pushstring(L, "class_name");
	 lua_pushstring(L, monitor->class_name);
	 lua_settable(L, -3);

	 lua_pushstring(L, "director_name");
	 lua_pushstring(L, monitor->director_name);
	 lua_settable(L, -3);

	 lua_pushstring(L, "director_host_name");
	 lua_pushstring(L, monitor->director_host_name);
	 lua_settable(L, -3);

   lua_pushstring(L, "client_name");
	 lua_pushstring(L, monitor->client_name);
	 lua_settable(L, -3);

   lua_pushstring(L, "passwd");
	 lua_pushstring(L, monitor->passwd);
	 lua_settable(L, -3);

   lua_pushstring(L, "portno");
	 lua_pushnumber(L, monitor->portno);	 
	 lua_settable(L, -3);

	 return 1;
}

/* director_status(self) */
static int baculua_director_status(lua_State * L)
{	 
	 bacula_mon mon;
	 char * msg;

	 if(get_monitor(L, &mon) != 0) {
			lua_pushnil(L);
			lua_pushstring(L, get_error_string());
			return 2;
	 }

	 msg = do_bacula_cmd(&mon, "status director");
	 if(msg == NULL) {
			lua_pushnil(L);
			lua_pushstring(L, get_error_string());
			return 2;
	 }

	 lua_pushstring(L, msg);

	 return 1;
}

static int baculua_messages(lua_State * L)
{
	 bacula_mon mon;
	 char * msg;

	 if(get_monitor(L, &mon) != 0) {
			lua_pushnil(L);
			lua_pushstring(L, get_error_string());
			return 2;
	 }

	 msg = do_bacula_cmd(&mon, "status director");
	 if(msg == NULL) {
			lua_pushnil(L);
			lua_pushstring(L, get_error_string());
			return 2;
	 }

	 lua_pushstring(L, msg);

	 return 1;
}

int luaopen_baculua (lua_State *L) {
	 luaL_openlib(L, "baculua", baculua_lib, 0);
	 return 1;
}

