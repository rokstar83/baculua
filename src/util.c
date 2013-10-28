/*****************************************************************************/ 
/* util.h for baculua                                                        */
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
#include "util.h"

int get_monitor(lua_State *L, bacula_mon * mon)
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

char * do_bacula_cmd(bacula_mon * mon, const char * cmd)
{
	 
}
