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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <w3string.h>

char*
composeString (char *str1, char *str2) {
  char *str;
  if ( (str2 != NULL) && (str1 != NULL) ) {
    str = (char*)calloc ((strlen(str1) + strlen(str2) + 2), sizeof (char));
    str = strcpy (str, str1);
    return (strcat (str, str2));
  } else {
    if (str1 != NULL) {
      str = copyString(str1);
    } 
    if (str2 != NULL) {
      str = copyString(str2);
    } 
    return str;
  }
  return NULL;
}

char*
copyString (char *str) {
  char *newstr;
  newstr = (char*)calloc ( (strlen(str) + 1), sizeof(char));
  newstr = strcpy (newstr, str);
  return newstr;
}
