/******************************************************************************/
/* monitor_tests.h --- monitor unit tests for baculua                         */
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
#ifndef MONITOR_TESTS_H_
#define MONITOR_TESTS_H_

#include <CUnit/CUnit.h>
/* START TEST PROTOTYPES */
void test_connect_monitor();
void test_bad_connect();
void test_send_message();
/* END TEST PROTOTYPES */

CU_pSuite monitor_test_suite();

#endif/* MONITOR_TESTS_H_ */