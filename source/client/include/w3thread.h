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
#ifndef __W3THREAD_H
#define __W3THREAD_H

#include <config.h>

#ifdef HAVE_LIBPTHREAD
#include <pthread.h> 


#include <w3types.h>

/*!
 * \addtogroup client
 * @{
 */
/*!
 * Thread structure to hide lower level thread libraries.
 * Only startRoutine and arg have to be set by the caller.
 */
typedef struct {
  void *startRoutine;
  void *arg;

  pthread_t      ID;
  pthread_attr_t attr;
} w3Thread;

/*!
 * Starts start_func as a new thread. The pointer to a newly created w3Thread structure is returned.
 */
w3Thread* w3ThreadRun (void* start_func, void* arg);

/*!
 * Joins thread t. The structure is deleted.
 */
boolean w3ThreadJoin (w3Thread *t);

/*!
 * 
 */
boolean w3ThreadDestroy (w3Thread *t);
/*!
 * @}
 */

#endif
#endif
