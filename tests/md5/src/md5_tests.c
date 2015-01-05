/******************************************************************************/
/* md5_tests.c --- md5 unit tests for baculua                                 */
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
#include "md5_tests.h"
#include <string.h>
#include <md5.h>

/* START TEST DEFINITIONS */

void test_bin_to_base64()
{
   int x = 0, compatible = 1, len;
   char res[100];
   
   while(strcmp(test_bin_to_base64_data[x].test_str, "") != 0) {
      len = bin_to_base64(res, 100, test_bin_to_base64_data[x].test_str, 
                          strlen(test_bin_to_base64_data[x].test_str), compatible);
      CU_ASSERT(strcmp(res, test_bin_to_base64_data[x].base64_result) == 0);
      x++;
   }
}

void test_hmac()
{
   int x = 0, y = 0;
   char keystr[100];
   size_t keylen;
   char teststr[100];
   size_t testlen;
   char hmac[100];
   char res[64];
   size_t reslen;
   
   while(strcmp(test_hmac_data[x].key,"") != 0) {
      keylen = hex_to_char_buf(test_hmac_data[x].key, test_hmac_data[x].keylen, keystr, 100);
      testlen = hex_to_char_buf(test_hmac_data[x].data, test_hmac_data[x].datalen, teststr, 100);
      reslen = hex_to_char_buf(test_hmac_data[x].hmac_result, 32, res, 64);

      hmac_md5(keystr, keylen, teststr, testlen, hmac);
      if(strncmp(hmac, res, 64) != 0) {
         y++;
      }

      CU_ASSERT(strncmp(hmac, res, 64) == 0);
      x++;
   }

   CU_FAIL("Test not implemented");
}

void test_hex_to_char_str()
{
   char hexstr[] = "0x0b0b0b0b";
   char res[100];
   size_t len;
   
   len = hex_to_char_buf(hexstr, sizeof(hexstr), res, 100);
   if(strncmp(res, "\v\000\v\000\v\000\v\000", len) != 0) {
      CU_ASSERT(0);
   }
}

/* END TEST DEFINITIONS */

CU_pSuite md5_test_suite()
{
  CU_pSuite suite = NULL;

  suite = CU_add_suite("md5_suite", NULL, NULL);

  CU_add_test(suite, "test_bin_to_base64", test_bin_to_base64);
  CU_add_test(suite, "test_hmac", test_hmac);
  CU_add_test(suite, "test_hex_to_char_str", test_hex_to_char_str);
  /* ADD TESTS TO SUITE HERE */

  return suite;
}
