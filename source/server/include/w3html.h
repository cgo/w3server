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
#ifndef __W3HTML_H
#define __W3HTML_H

#include <w3types.h>

#define W3_HTML_400 1
#define W3_HTML_404 2
#define W3_HTML_503 3

/*!
* \addtogroup server
* @{
*/
/*!
 * Generates an appropriate error page.
 * The returned pointer points to a newly created string that should be free()d after use.
 */
char *w3HTMLGenerateError (char *header, char *info, int pagecode);
/*! @} */
#endif
