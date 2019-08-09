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
#include <w3thread.h>
#include <w3server.h>
#include <string.h>

struct myStruct {
  char text[10];
};

void* dummyFunction (void* ptr) {
  printf("%s\n",((struct myStruct*)ptr)->text);
  return NULL;
}


int main (int argc, char *argv[]) {
  struct myStruct str;
  w3Thread *t[10];
  int i;
  
  if (argc < 2) {
    printf("Usage: %s <portnumber>\n", argv[0]);
    exit (2);
  }
  w3StartServer (atoi(argv[1]));
  exit (1);
  
  /* Multithreaded client test. */
  
  w3ClientInit ();
  
  strcpy (str.text, "Huhu !");
  for (i = 0; i < 10; i++) {
    t[i] = w3ThreadRun ((void*)dummyFunction, (void*)&str);
  }
  for (i = 0; i < 10; i++) {
    w3ThreadJoin (t[i]);
  }
  
  exit (1);
}
