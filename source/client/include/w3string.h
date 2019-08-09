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
 * \file w3string.h
 * \brief Convenience functions to handle <CODE>char*</CODE> strings.
 */
#ifndef __W3STRING_H__
#define __W3STRING_H__

/*!
 * \addtogroup client
 * @{
 */
/*!
 * Concatenates <CODE>str1</CODE> and <CODE>str2</CODE>.
 * \return A new string containing the concatenation of <CODE>str1</CODE and <CODE>str2</CODE>.
 * \author Christian Gosch
 */
char* composeString (char *str1, char *str2);

/*!
 * Copies <CODE>str</CODE>.
 * \return A new string.
 * \author Christian Gosch
 */
char* copyString (char *str);
/*!
 * @}
 */

#endif
