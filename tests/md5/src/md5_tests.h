/******************************************************************************/
/* md5_tests.h --- md5 unit tests for baculua                                 */
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
#ifndef MD5_TESTS_H_
#define MD5_TESTS_H_

#include <CUnit/CUnit.h>
#include <stdint.h>

/* Test data from RFC 4648 
   https://tools.ietf.org/html/rfc4648#page-11 */

struct test_bin_to_base64_data_t {
   char * test_str;
   char * base64_result;
} static test_bin_to_base64_data[] = {
   {"f", "Zg" },
   {"fo", "Zm8" },
   {"foo", "Zm9v"},
   {"foob","Zm9vYg"},
   {"fooba","Zm9vYmE"},
   {"foobar","Zm9vYmFy"},
   {"", ""},
  };

/* Test data from rfc 2202
   https://tools.ietf.org/html/rfc4648#page-11
 */

struct test_hmac_data_t {
   char* key;
   size_t keylen;
   char* data;
   size_t datalen;
   char* hmac_result;
} static test_hmac_data[] = {   
   {"0x0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b", 16, "Hi There", 8, "0x9294727a3638bb1c13f48ef8158bfc9d" },
   {"Jefe", 4, "what do ya want for nothing?", 28, "0x750c783e6ab0b503eaa86e310a5db738" },
   {"0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 16, 
    "0xdddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd",
    50, "0x56be34521d144c88dbb8c733f0e8b3f6"},
   
   {"0x0102030405060708090a0b0c0d0e0f10111213141516171819", 25, 
    "0xcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd",
    50, "0x697eaf0aca3a3aea3a75164746ffaa79"},
   {"",0,"",0,""},
};

/* START TEST PROTOTYPES */
void test_bin_to_base64();
void test_hmac();
void test_hex_to_char_str();
/* END TEST PROTOTYPES */

CU_pSuite md5_test_suite();

#endif/* MD5_TESTS_H_ */
