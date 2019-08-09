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
#include <w3smtp.h>
#include <w3comm.h>
#include <w3string.h>
#include <w3types.h>
#include <w3client.h>
#include <w3error.h>
#include <w3clientconfig.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

w3Mail *w3CreateMail (char* rcpt, char* sender, char* data) {
  w3Mail *mail = (w3Mail*)malloc (sizeof (w3Mail));
  mail->recipient = copyString ((char*)rcpt);
  mail->sender = copyString ((char*)sender);
  mail->data   = copyString ((char*)data);
  return mail;
}

boolean w3DestroyMail (w3Mail *mail) {
  if (mail) {
    free (mail->recipient);
    free (mail->sender);
    free (mail->data);
    return true;
  }
  return false;
}

void* w3ReceiveMailReply (unsigned int *size, int sock) {
  size_t s = 0;
  char* data;

  data  = (char*)malloc (1024 * sizeof(char));
  *size = 0;
  do {
    s = read ( sock, (void*)(&((char*)data)[*size]), 1024 );
    if ( s == -1 ) {
#ifdef __W3CLIENT_DEBUG
      w3Error ("receiving data");
#endif
      return NULL;
    }
    /* printf ("received %d bytes\n",s); */
    *size += (unsigned int)s;
    /* printf("%s\n",&data[*size - s]); */
    /* printf("reallocating %d...\n", *size); */
    data = (char*)realloc ( (void*)data, (size_t)((1024 + *size) * sizeof(char)) );
    /* printf("New pointer: %p\n",data); */
  } while ( ( s > 0 ) && (data[*size - 1] != 10) );

  /* debugging */
  /* printf("\n%s\n",(char*)data); */

  return (void*)data;
}


boolean w3SendMail (w3Mail *mail, const char *mailer, int port) {
  w3ClientConnection con;
  char hostname[255];
  char *command;
  char *tmp;
  unsigned int size;

  const char helo[] = "HELO ";
  const char mailfrom[] = "MAIL FROM:";
  const char rcptto[] = "RCPT TO:";
  const char data[] = "DATA";
  const char crlf[] = {13,10,0};
  const char endmail[] = {13,10,'.',13,10,0};
  const char quit[] = "QUIT";

  con.hostname = copyString ((char*)mailer);
  con.port     = port;

#ifdef __W3CLIENT_DEBUG
  printf("Mail message to %s from %s via %s\n",mail->recipient, mail->sender, mailer);
#endif

  w3ClientConnect (&con);
  
  gethostname (&hostname[0], 255);
  tmp = composeString ((char*)helo, &hostname[0]);
  command = composeString (tmp, (char*)crlf);
  free (tmp);
  w3SendData ((const void*)command, sizeof (char) * strlen(command), con.socket);
  tmp = (char*)w3ReceiveMailReply (&size, con.socket);
#ifdef __W3CLIENT_DEBUG
  printf("Reply: %s\n", tmp);
#endif
  free (tmp);
  free (command);
 

  tmp = composeString ((char*)mailfrom, mail->sender);
  command = composeString (tmp, (char*)crlf);
  free (tmp);
  w3SendData ((const void*)command, sizeof (char) * strlen(command), con.socket);
  tmp = (char*)w3ReceiveMailReply (&size, con.socket);
#ifdef __W3CLIENT_DEBUG
  printf("Reply: %s\n", tmp);
#endif
  free (tmp);
  free (command);

  tmp = composeString ((char*)rcptto, mail->recipient);
  command = composeString (tmp, (char*)crlf);
  free (tmp);
  w3SendData ((const void*)command, sizeof (char) * strlen(command), con.socket);
  tmp = (char*)w3ReceiveMailReply (&size, con.socket);
#ifdef __W3CLIENT_DEBUG
  printf("Reply: %s\n", tmp);
#endif
  free (tmp);
  free (command);

  command = composeString ((char*)data, (char*)crlf);
  w3SendData ((const void*)command, sizeof (char) * strlen(command), con.socket);
  tmp = (char*)w3ReceiveMailReply (&size, con.socket);
#ifdef __W3CLIENT_DEBUG
  printf("Reply: %s\n", tmp);
#endif
  free (tmp);
  free (command);

#ifdef __W3CLIENT_DEBUG
  printf("Sending data ...\n");
#endif
  tmp = composeString (mail->data, (char*)endmail);
  w3SendData ((const void*)tmp, sizeof (char) * strlen(tmp), con.socket);
  free (tmp);
  tmp = (char*)w3ReceiveMailReply (&size, con.socket);
#ifdef __W3CLIENT_DEBUG
  printf("Reply: %s\n", tmp);
#endif
  free (tmp);

  tmp = composeString ((char*)quit, (char*)crlf);
  w3SendData ((const void*)tmp, sizeof (char) * strlen(tmp), con.socket);
  free (tmp);

  w3ClientDisconnect (&con);
  
  return true;
}

static char* __w3ReadLine (FILE *f) {
  char *tmp;
  char c;
  int i = 0;
  int size = 64;
  tmp = (char*)calloc (64, sizeof(char));
  do {
    c = fgetc (f);
    tmp[i] = c;
    i++;
    if (i >= size) {
      realloc ((void*)tmp, size + 64);
      size += 64;
    }
  } while ( (c != 10) && (!feof (f)) );
  tmp[i - 1] = 0;
  return tmp;
}

boolean w3SendMailFile (const char* path) {
  w3Mail *mail;
  FILE *f;
  char *recipient;
  char *sender;
  char *mailer;
  char *mailbody;
  int i;
  int size;
  char c;
  int port;
  char *portStr;
  /* Continue here. */
  /* Read recipient, sender, mailer, portnumber and message body */
  if( (f = fopen(path, "r")) == NULL ) {
#ifdef __W3_CLIENT_DEBUG    
    w3Error ("Opening path to mail file");
#endif
    return false;
  }
  recipient = __w3ReadLine (f);
  sender    = __w3ReadLine (f);
  mailer    = __w3ReadLine (f);
  portStr   = __w3ReadLine (f);
  port      = atoi (portStr);
  mailbody  = (char*)malloc (sizeof (char) * 65);
  i    = 0;
  size = 64;
  while (!feof (f)) {
    c = fgetc (f);
    mailbody[i] = c;
    i++;
    if (i >= size) {
      mailbody = (char*)realloc ((void*)mailbody, size + 64);
      size += 64;
    }
  }
  mailbody[i - 1] = 0;
  mail = w3CreateMail (recipient, sender, mailbody);
  if (mail != NULL) {
    w3SendMail (mail, mailer, port);
    w3DestroyMail (mail);
  }
  fclose (f);
  free (recipient);
  free (sender);
  free (mailbody);
  free (mailer);
  free (portStr);
  return true;
}














