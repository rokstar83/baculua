/*****************************************************************************/ 
/* monitor.h for baculua                                                     */
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
#ifndef MONITOR_H_
#define MONITOR_H_
#include <lua.h>
#include <netinet/in.h>

const unsigned int DEFAULT_DIRECTOR_PORTNO = 9101;

typedef struct {
	 /* connection information */
	 const char * director_name;
	 const char * director_host_name;
	 const char * client_name;
	 const char * passwd;
	 unsigned int portno;

	 /* sockets and whatnot */
	 int sock;
	 struct sockaddr_in sin;
} monitor;

int lua_checkmonitor(lua_State *L, monitor * mon);
int connect_monitor(monitor * mon);
void disconnect_monitor(monitor * mon);
int send_message(monitor * mon, const char * cmd);
char * receive_message(monitor * mon);

#endif /* MONITOR_H_ */
