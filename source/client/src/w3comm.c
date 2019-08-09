/*
    Copyright (C) by Christian Gosch.

    This file is part of w3server, a very small HTTP(-subset) server.

    w3server is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <w3client.h>
#include <w3comm.h>
#include <w3error.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <w3clientconfig.h>

boolean w3SendData (const void* data, size_t size, int sock) {
  if ( write (sock, data, size) == -1 ) {
    w3Error ("sending data");
    return false;
  }
  return true;
}

void* w3ReceiveData (unsigned int *size, int sock) {
  size_t s = 0;
  char* data;

  data  = (char*)malloc (1024 * sizeof(char));
  *size = 0;
  do {
    s = read ( sock, (void*)(&((char*)data)[*size]), 1024 );
    if ( s == -1 ) {
#ifdef __W3CLIENT_DEBUG
      w3Error ("receiving data"); 
#endif
      return NULL;
    }
    /* printf ("received %d bytes\n",s); */
    *size += (unsigned int)s;
    /* printf("%s\n",&data[*size - s]); */
    /* printf("reallocating %d...\n", *size); */
    data = (char*)realloc ( (void*)data, (size_t)((1024 + *size) * sizeof(char)) );
    /* printf("New pointer: %p\n",data); */
  } while (s > 0);

  /* debugging */
  /* printf("\n%s\n",(char*)data); */
  data = (char*)realloc ( (void*)data, (size_t)(*size) * sizeof(char) );
  // data[*size] = 0;

  return (void*)data;
}




