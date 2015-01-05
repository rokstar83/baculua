/*
 * Bacula MD5 definitions
 *
 *  Kern Sibbald, 2001
 *
 *   Version $Id$
 */
/*
   Bacula® - The Network Backup Solution

   Copyright (C) 2001-2006 Free Software Foundation Europe e.V.

   The main author of Bacula is Kern Sibbald, with contributions from
   many others, a complete list can be found in the file AUTHORS.
   This program is Free Software; you can redistribute it and/or
   modify it under the terms of version three of the GNU Affero General Public
   License as published by the Free Software Foundation and included
   in the file LICENSE.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.

   Bacula® is a registered trademark of Kern Sibbald.
   The licensor of Bacula is the Free Software Foundation Europe
   (FSFE), Fiduciary Program, Sumatrastrasse 25, 8006 Zürich,
   Switzerland, email:ftf@fsfeurope.org.
*/
#ifndef BMD5_H_
#define BMD5_H_

#include <stdint.h>
#include <arpa/inet.h>

#define MD5HashSize 16
#define PAD_LEN 64           /* PAD length */
#define SIG_LEN MD5HashSize  /* MD5 digest length */

struct MD5Context {
   uint32_t buf[4];
   uint32_t bits[2];
   uint8_t  in[64];
};

typedef struct MD5Context MD5Context;
extern int bigendian();

extern void MD5Init(struct MD5Context *ctx);
extern void MD5Update(struct MD5Context *ctx, unsigned char *buf, unsigned len);
extern void MD5Final(unsigned char digest[16], struct MD5Context *ctx);
extern void MD5Transform(uint32_t buf[4], uint32_t in[16]);
int bin_to_base64(char *buf, int buflen, char *bin, int binlen, int compatible);
void hmac_md5(uint8_t* text, int text_len, uint8_t* key, int key_len, uint8_t *hmac);
size_t hex_to_char_buf(char * hexstr, size_t hexstrlen, char * charstr, size_t charstrlen);

#endif /* BMD5_H_ */
