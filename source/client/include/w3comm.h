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
/*! 
 * \file w3comm.h
 * \brief Convenient communication functions (to get rid of thr messy read/write stuff).
 */

#ifndef __W3COMM_H__
#define __W3COMM_H__

#include <w3types.h>
#include <w3client.h>
#include <sys/types.h>
/*!
 * \addtogroup client
 * @{
 */

/*!
 * Sends data in <CODE>void *data</CODE> of size <CODE>size_t size</CODE> to <CODE>con</CODE>.
 * \brief Send data through a connection.
 * \author Christian Gosch
 */
boolean w3SendData (const void* data, size_t size, int sock);

/*!
 * Receives data from <CODE>con</CODE> into <CODE>void *data</CODE>. The size of the received block is 
 * stored in <CODE>size</CODE>.
 * \brief Receive data through a connection.
 * \author Christian Gosch
 */
void* w3ReceiveData (unsigned int *size, int sock);
/*!
 * @}
 */

#endif







