/******************************************************************************/
/* monitor_tests.c --- monitor unit tests for baculua                         */
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
#include "monitor_tests.h"
#include "baculua_error.h"
#include "secret.h"
#include <monitor.h>

/* START TEST DEFINITIONS */

void test_connect_monitor()
{
   
   monitor mon;
   int err;
  
   mon.sock = 0;
   mon.director_name = "homer";
   mon.director_host_name = "homer";
   mon.portno = DEFAULT_DIRECTOR_PORTNO;

   err = connect_monitor(&mon, 3);
   disconnect_monitor(&mon); 

   CU_ASSERT_EQUAL(err, E_SUCCESS); 
}

void test_bad_connect()
{
   /* Test connecting to a non-existant host */
   {
      monitor mon;
      int err;      

      init_monitor(&mon);
      mon.director_name = "bad_host";
      mon.director_host_name = "bad_host";
      
      err = connect_monitor(&mon, 3);
      disconnect_monitor(&mon);
      
      CU_ASSERT_EQUAL(err, E_MONITOR_NO_HOST);
   }

   /* Test connecting to an existing host, but one that isn't listening on 
      the DEFAULT_DIRECTOR_PORTNO */
   {
      monitor mon;
      int err;

      init_monitor(&mon);
      mon.director_name = "google";
      mon.director_host_name = "google.com";
      
      err = connect_monitor(&mon, 3);
      disconnect_monitor(&mon);
      
      CU_ASSERT_EQUAL(err, E_MONITOR_TIMEOUT);
   }
}

void test_send_message()
{
   /* Send a message over a non-connected monitor */
   {
      monitor mon;
      int err;
      
      init_monitor(&mon);
      mon.director_name = "homer";
      mon.director_host_name = "homer";

      err = send_message(&mon, "HODOR!", strlen("HODOR!"));
      CU_ASSERT_EQUAL(err, E_MONITOR_NOT_CONNECTED);
   }

   /* Test sending a message */
   {
      monitor mon;
      int err;
      char hodor[] = "HODOR!";
      
      init_monitor(&mon);
      mon.director_name = "homer";
      mon.director_host_name = "homer";
      
      CU_ASSERT_EQUAL(connect_monitor(&mon, 3), E_SUCCESS);      
      CU_ASSERT_EQUAL(send_message(&mon, hodor, strlen(hodor)), E_SUCCESS);
      disconnect_monitor(&mon);
   }

   /* Test sending a proper hello */
   {
      monitor mon;
      int err;
      char hello[] = "Hello nestor calling\n";
      
      init_monitor(&mon);
      mon.director_name = "homer";
      mon.director_host_name = "homer";
      
      CU_ASSERT_EQUAL(connect_monitor(&mon, 3), E_SUCCESS);
      CU_ASSERT_EQUAL(send_message(&mon, hello, strlen(hello)), E_SUCCESS);
      disconnect_monitor(&mon);
   }
}

void test_receive_message()
{
   /* Test trying to get a message without a connected monitor */
   {
      monitor mon;
       
      init_monitor(&mon);
      mon.director_name = "homer";
      mon.director_host_name = "homer";
      char msg[100];
      
      CU_ASSERT_EQUAL(receive_message(&mon,msg,strlen(msg)), E_MONITOR_NOT_CONNECTED);
   }

   /* Test trying to get a message when we aren't expecting anything (timeout) */
   {
      monitor mon;
      int err;
      char msg[100];
      
      init_monitor(&mon);
      mon.director_name = "homer";
      mon.director_host_name = "homer";      
      
      CU_ASSERT_EQUAL(connect_monitor(&mon, 3), E_SUCCESS);

      err = receive_message(&mon, msg, sizeof(msg));
      CU_ASSERT_EQUAL(err, E_MONITOR_RECV_TIMEOUT);

      disconnect_monitor(&mon); 
   } 

   /* A proper hello test */
   {
      char msg[100];
      monitor mon;
      int err;
      char hello[] = "Hello *UserAgent* calling\n";
      
      init_monitor(&mon);
      mon.director_name = "homer";
      mon.director_host_name = "homer";
      
      CU_ASSERT_EQUAL(connect_monitor(&mon, 3), E_SUCCESS);
      CU_ASSERT_EQUAL(send_message(&mon, hello, strlen(hello)), E_SUCCESS);
      err = receive_message(&mon, msg, sizeof(msg));
      CU_ASSERT(err > 0);
      disconnect_monitor(&mon);
   }
}

void test_authenticate_monitor()
{
   {
      char msg[100];
      int err;
      monitor mon;

      init_monitor(&mon);
      mon.director_name = "homer";
      mon.director_host_name = "homer";
      mon.host_name = "nestor";
      mon.passwd = monitor_passwd;
      CU_ASSERT_EQUAL(connect_monitor(&mon, 3), E_SUCCESS);
      CU_ASSERT_EQUAL(authenticate_monitor(&mon), E_SUCCESS);
   }
}

/* END TEST DEFINITIONS */

CU_pSuite monitor_test_suite()
{
  CU_pSuite suite = NULL;

  suite = CU_add_suite("monitor_suite", NULL, NULL);

  CU_add_test(suite, "test_connect_monitor", test_connect_monitor);
  CU_add_test(suite, "test_bad_connect", test_bad_connect);
  CU_add_test(suite, "test_receive_message", test_receive_message);
  CU_add_test(suite, "test_send_message", test_send_message);
  CU_add_test(suite, "test_authenticate_monitor", test_authenticate_monitor);
  /* ADD TESTS TO SUITE HERE */

  return suite;
}
