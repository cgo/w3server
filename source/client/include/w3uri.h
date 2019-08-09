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
 * \file w3uri.h
 * \brief Functions to manipulate URIs.
 */

#ifndef __W3URI_H__
#define __W3URI_H__

#include <w3types.h>

/*!
 * \addtogroup client
 * @{
 */
/*!
 * Parses the URI given in <CODE>uristr</CODE> and stores the corresponding values is <CODE>uri</CODE>.
 * No error checking or checks for malformed URIs is done so far. This has to be added.
 */
boolean w3ParseURI (const char *uristr, w3URI *uri);
/*!
 * @}
 */

#endif /* __URI_H__ */


