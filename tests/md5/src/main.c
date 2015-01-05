/******************************************************************************/
/* main.c --- md5 unit tests for baculua                                      */
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
#include <CUnit/CUnit.h>
#include <cunitsexpoutputter/sexp_outputter.h>
#include <CUnit/TestDB.h>
#include <stdio.h>
#include "md5_tests.h"

int main(int argc, char ** argv)
{
   CU_pSuite suite = NULL;
   CU_ErrorCode err = CU_initialize_registry();

   if(err != CUE_SUCCESS)
      return 1;

   suite = md5_test_suite();
   if(suite == NULL) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   CU_sexp_set_output(stdout);
   CU_sexp_run_tests();

   CU_cleanup_registry();
   return CU_get_error();
}