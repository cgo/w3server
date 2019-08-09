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
 * \file w3smtp.h
 * \brief Functions for sending email via SMTP.
 */

#ifndef W3SMTP_H
#define W3SMTP_H

#include <w3types.h>

/*!
 * \addtogroup client
 * @{
 */
/*!
 * The mail data structure. The user does not necessarily need to know about this, 
 * but can use w3CreateMail() and w3DestroyMail() instead.
 */
typedef struct {
  char *recipient;
  char *sender;
  char *data;
} w3Mail;

/*!
 * Takes recipient, sender and mail body and allocates a new w3Mail structure of it.
 */
w3Mail *w3CreateMail (char* rcpt, char *sender, char *data);

/*!
 * Destroys the given mail.
 */
boolean w3DestroyMail (w3Mail *mail);

/*!
 * Sends the given mail to the SMTP server given by <CODE>mailer</CODE> running on port <CODE>port</CODE>.
 */
boolean w3SendMail (w3Mail *mail, const char *mailer, int port);  

/*!
 * Sends the file <CODE>path</CODE>, containing a mail message. <br>
 * The file must conform to the following specification: <br>
 * First line: Recipient <br>
 * Second line: Sender <br>
 * Third line: mailer <br>
 * Fourth line: port number on which the mailer accepts connections <br>
 * The following lines contain the mail body, including ALL headers the
 * sender wants to be sent (including sender and recipient headers).
 */
boolean w3SendMailFile (const char* path);
/*!
 * @}
 */


#endif








