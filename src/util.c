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

char * do_bacula_cmd(monitor * mon, const char * cmd)
{
	 int err;
	 char * retval = NULL;
	 if((err = connect_monitor(mon)) != 0) {
			return retval;
	 }

	 if((err = send_message(mon, cmd)) != 0) {
			disconnect_monitor(mon);
			return retval;
	 }

	 retval = receive_message(mon);
	 disconnect_monitor(mon);
	 return retval;
}
