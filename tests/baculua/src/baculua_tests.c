/******************************************************************************/
/* baculua_tests.c --- baculua unit tests for baculua                         */
/* Copyright (c) 2013 Thomas Hartman (rokstar83@gmail.com)                    */
/*                                                                            */
/* This program is free software; you can redistribute it and/or              */
/* modify it under the terms of the GNU General Public License                */
/* as published by the Free Software Foundation; either version 2             */
/* of the License, or the License, or (at your option) any later              */
/* version.                                                                   */
/*                                                                            */
/* This program is distributed in the hope that it will be useful             */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              */
/* GNU General Public License for more details.                               */
/******************************************************************************/
#include "baculua_tests.h"
#include <baculua.h>

/* START TEST DEFINITIONS */

/* END TEST DEFINITIONS */

CU_pSuite baculua_test_suite()
{
  CU_pSuite suite = NULL;

  suite = CU_add_suite("baculua_suite", NULL, NULL);

  /* ADD TESTS TO SUITE HERE */

  return suite;
}