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
#include <w3uri.h>
#include <w3string.h>
#include <w3clientconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static
void addChar (char *str, char c, int idx) {
  if (idx >= (strlen (str) - 1)) {
    str = (char*)realloc ((void*)str, sizeof(char) * 64);
  } 
  str = strncat (str, (const char*)&c, 1);
}

char *w3ParseURIGetToken (char *str, int *i, char lastChar) {
  char *newStr;
  int newI;
  char c;
  int newSize = 256;
  newI = 0;
  newStr = (char*) calloc (newSize, sizeof(char));
  while ( (*i < strlen(str)) && (str[*i] != lastChar) ) {
    if (newI >= newSize) {
      newSize += 256;
      newStr = (char*)realloc (newStr, sizeof(char) * newSize);
    }
    c = str[(*i)++];
    newStr[newI++] = c;
  }
  newStr[newI] = 0;
  newStr = (char*)realloc((void*)newStr, (strlen(newStr) + 2) * sizeof(char));
  return newStr;
}

boolean w3ParseURI (const char *uriString, w3URI *uri) {
  enum {
    W3HOST,
    W3PATH,
    W3FILE,
    W3END
  };

  const char HOSTNAME_END   = '/';
  const char PATH_END       = '\0';
  /* const char FILENAME_BEGIN = '/'; */
  int state = W3HOST;
  int idx   = 0;
  int idx2  = 0;
  char c;
  char *temp = (char*)calloc (64, sizeof(char));

  idx2 = 0;
  uri->hostname = (char*)calloc (64, sizeof(char));
  uri->path     = (char*)calloc (64, sizeof(char));
  uri->filename = (char*)calloc (1, sizeof(char));
  
  while (state != W3END) {
    c = uriString[idx];
    switch (state) {
    case W3HOST:
      if ( (c != HOSTNAME_END) && (idx < strlen(uriString)) ) {
	addChar (uri->hostname, c, idx2);
	idx++;
	idx2++;
      } else {
	idx2 = 0;
	state = W3PATH;
      }
      break;
    case W3PATH:
      if ( (c != PATH_END) && (idx < strlen(uriString)) ) {
	addChar (uri->path, c, idx2);
	idx++;
	idx2++;
      } else {
	if (strlen (uri->path) == 0) {
	  uri->path[0] = '/';
	  uri->path[1] = 0;
	}
	idx2 = 0;
	uri->filename[0] = 0;
	state = W3END;
      }
      break;
    case W3END:
      return true;
      break;
    }
  }
#ifdef __W3CLIENT_DEBUG
  printf("w3ParseURI set uri to \n\tHostname: %s\n\tPath: %s\n", uri->hostname, uri->path);
#endif
  return true;
}

















