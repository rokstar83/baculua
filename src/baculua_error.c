/*****************************************************************************/ 
/* baculua_error.c for baculua                                               */
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
#include "baculua_error.h"
#include <stdio.h>

char * get_baculua_error_desc(baculua_error_t err_code)
{
   int x;
   for(x = 0; baculua_error_desc[x].err != 0; ++x) {
      if(baculua_error_desc[x].err == err_code) {
         return baculua_error_desc[x].msg;
      }
   }

   return "Unknown error code";
}

char * get_error_string()
{
   return error_str;
}
