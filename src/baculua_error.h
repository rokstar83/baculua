/*****************************************************************************/ 
/* baculua_error.h for baculua                                               */
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
#ifndef BACULUA_ERROR_H_
#define BACULUA_ERROR_H_

enum _baculua_error
{
   E_SUCCESS = 0,

   /* Monitor specific errors */
   E_MONITOR_NO_SOCKET = -1,
   E_MONITOR_NO_HOST = -2,
   E_MONITOR_ADDY_CONVERT = -3,
   E_MONITOR_GET_SOCK_FLAG = -4,
   E_MONITOR_SET_SOCK_FLAG = -5,
   E_MONITOR_SELECT = -6,
   E_MONITOR_TIMEOUT = -7,
   E_MONITOR_NOT_CONNECTED = -8,
   E_MONITOR_BAD_SEND = -10,
   E_MONITOR_NO_MSG = -11,
   E_MONITOR_BAD_RECV = -12,
};

typedef enum _baculua_error baculua_error_t;

struct {
   baculua_error_t err;
   char * message;
} static baculua_error_desc[] = {
   { E_SUCCESS, "No error" },

   /* Monitor specific errors descriptions */
   { E_MONITOR_NO_SOCKET, "Monitor was unable to create a new socket" },
   { E_MONITOR_NO_HOST, "Unable to find monitor host" },
   { E_MONITOR_ADDY_CONVERT, "Unable to convert IP string to address" },
   { E_MONITOR_GET_SOCK_FLAG, "Unable to retrieve socket flags" },
   { E_MONITOR_SET_SOCK_FLAG, "Unable to set socket flags" },
   { E_MONITOR_SELECT, "Error while attempting to connect to host" },
   { E_MONITOR_TIMEOUT, "Timeout occured before connection could be "
                        "established with host" },
   { E_MONITOR_NOT_CONNECTED, "Not connected to monitor" },
   { E_MONITOR_BAD_SEND, "Sending message to monitor failed." },
   { E_MONITOR_NO_MSG, "No message available from directory." },
   { E_MONITOR_BAD_RECV, "Error while attempting to read daat from directory." },
};

char error_str[100];
char * get_error_string();

#endif /* BACULUA_ERROR_H_ */
