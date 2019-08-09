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
#include <w3client.h>
#include <w3uri.h>
#include <w3types.h>
#include <w3string.h>
#include <w3smtp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define W3HELPTEXT {\
    printf("Usage: \t\t%s get <full path of file> <destination file> <port number>\n",argv[0]);\
    printf("or\n \t\t%s mail <mailfile>\n",argv[0]);\
    printf("\t<mailfile> must be a file consisting of the first four \n\t\tlines as recipient, sender, mailhost, portnumber, followed by the message body.\n");\
    printf("\t<full path of file> must be of the form my.host.address/some_path/file.extension.\n");\
    printf("\t\tNote that no protocol definition is allowed so far.\n");\
}


int main2 (int argc, char *argv[]) {
  int i;
  char *str;
  char nr[10];

  /*
  w3Mail *mail;
  char *from;
  char *to;
  char *message;
  
  from = (char*)malloc (200 * sizeof(char));
  to = (char*)malloc (200 * sizeof(char));
  message = (char*)malloc (200 * sizeof(char));
  message = strcpy (message, "From: Hund@Katze.de\nTo: Du!\nSubject: Haha !\nThis is a mail test !!!");
  from = strcpy (from, "dixie@localhost");
  to = strcpy (to, "dixie@localhost");
  */
  /*
  mail = w3CreateMail (from, to, message);
  w3SendMail (mail, "localhost", 25);
  w3DestroyMail (mail);
  return 0;
  */

  if (argc < 2) {
    W3HELPTEXT;
    exit (0);
  }

  if ( strcmp(argv[1], "get") == 0 ) {
    if (argc < 5) {
      W3HELPTEXT;
      exit (0);
    }
    w3ClientDocToFile (argv[2], argv[3], atoi(argv[4]));
    return 0;
  }
  if ( strcmp(argv[1], "mail") == 0 ) {
    if (argc < 3) {
      W3HELPTEXT;
    }
    w3SendMailFile (argv[2]);
    return 0;
  }
  W3HELPTEXT;
  return 0;

  /* Test loop for multihreaded download. */
  for (i = 0; i < 10; i++) {
    sprintf(nr, "%d", i);
    str = composeString (argv[2], nr);
    if ( w3ClientDocToFileMT (argv[1], str, atoi(argv[3])) == false ) {
      printf("Could not create thread !\n");
    } else {
      printf("Downloading !\n");
    }
    free(str);
  }

  w3ClientJoinAllThreads();

  return 0;
}





