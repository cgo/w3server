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
#include <w3thread.h>
#include <w3types.h>
#include <stdlib.h>

#ifdef HAVE_LIBPTHREAD
#include <pthread.h>

w3Thread* w3ThreadRun (void* start_func, void* arg) {
  w3Thread *t = (w3Thread*)malloc(sizeof (w3Thread));

  t->startRoutine = (void*)start_func;
  t->arg = arg;

  pthread_attr_init (&t->attr);
  if ( pthread_create (&t->ID, &t->attr, (void* (*)(void*))t->startRoutine, t->arg) != 0 ) {
    free (t);
    t = NULL;
  }
  return t;
}

boolean w3ThreadJoin (w3Thread *t) {
  if ( (pthread_join (t->ID, NULL)) != 0) {
    return false;
  }
  free (t);
  return true;
}

boolean w3ThreadDestroy (w3Thread *t) {
  if ( pthread_kill (t->ID, 15) != 0 ) {
    free (t);
    return false;
  }
  free(t);
  return true;
}

#endif
