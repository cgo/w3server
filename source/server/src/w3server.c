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
 * \file w3server.h
 * \brief Main functions to set up a server.
 */

#include <w3server.h>
#include <w3types.h>
#include <w3comm.h>
#include <w3error.h>
#include <w3string.h>
#include <w3thread.h>
#include <w3html.h>

#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 256
#endif

#define W3_HTTP_VERSION 1.0

struct serverGlobal {

} serverGlobal;

/* File and directory settings. */
/* html document base directory */
/* This has to be an ABSOLUTE PATH !!! */
char HTDOC_BASE[] = "/home/christian/pgm/http-server-project/htdocs";
/* 
 * Binary base directory. Relative to HTDOC_BASE and has to start with a '/'.
 * All requested files in this directory are handled as executables and their output is sent 
 * on their behalf.
 */
char HTDOC_BIN_BASE[] = "/bin";
/* Template filename for Unix mktemp() function */
char HTDOC_BIN_TEMPLATE[] = "/temp/temp.XXXXXX";
char HTDOC_DOTHTML[] = "";
/* Not used */
char HTDOC_ERRORFILE[] = "/error.html";
char HTDOC_STANDARDFILE[] = "index.html";

/* Response strings */
char RESPONSE_OK[] = "HTTP/1.0 200 \n";
char RESPONSE_NOT_FOUND[] = "HTTP/1.0 404 \n";
char RESPONSE_SERVICE_UNAVAILABLE[] = "HTTP/1.0 503 \n";
char RESPONSE_BAD_REQUEST[] = "HTTP/1.0 400 \n";

/* Error codes */
#define W3_ERROR_NOT_FOUND 1
#define W3_ERROR_SERVICE_UNAVAILABLE 2
#define W3_ERROR_BAD_REQUEST 3

/* Commands */
#define W3_CMD_GET 1
#define W3_CMD_POST 2
#define W3_CMD_HEAD 3
#define W3_CMD_UNKNOWN 4

/*!
 * 
 */
w3ServerConnection* w3ServerAccept (w3Server *server) {
  w3ServerConnection *con;
  int a = 0,b = 0,c = 0,d = 0;

  con = (w3ServerConnection*) malloc (sizeof (w3ServerConnection));
  if ( listen (server->socket, 1) != 0 ) {
    w3Error ("Server: can't do listen()\n");
  }
  con->server = server;
  if ( (con->socket = accept (server->socket, (struct sockaddr*)&con->sock_addr, &con->addrlen)) == -1 ) {
    // w3Error ("Server: can't accept client\n");
    // printf("Socket: %d\nHostname: %s\n", server->socket, server->hostname);
    free (con);
    con = NULL;
    // w3ServerClose (server);
  } else {
    w3ServerMessage ("Connect");
#if 0
    a = ((int)con->sock_addr.sin_addr._S_un._S_addr & 0xff000000) >> 24;
    b = ((int)con->sock_addr.sin_addr._S_un._S_addr & 0x00ff0000) >> 16;
    c = ((int)con->sock_addr.sin_addr._S_un._S_addr & 0x0000ff00) >> 8;
    d = ((int)con->sock_addr.sin_addr._S_un._S_addr & 0x000000ff);
    printf("Address: %d.%d.%d.%d\n", a,b,c,d); 
#endif
  }
  return con;
}


/*!
 * 
 */
boolean w3ServerInit (w3Server *server) {
  socklen_t namelen;
  char *msg;
  char nr[256];

  w3ServerMessage ("Initializing w3Server");

  server->hostname = (char*)malloc (sizeof (char) * MAXHOSTNAMELEN);
  if ( gethostname (server->hostname, MAXHOSTNAMELEN) != 0 ) {
    w3Error ("Server: Could not obtain hostname");
  }

  msg = composeString ("Hello, I am ", server->hostname);
  w3ServerMessage(msg);
  free(msg);
  sprintf(nr, "%d", server->port);
  msg = composeString ("Starting on port ", nr);
  w3ServerMessage(msg);
  free(msg);
  
  server->host = gethostbyname (server->hostname);
  /* convert host name to IP address and copy to sock_addr */
  bcopy( ((struct hostent*)server->host)->h_addr, &server->sock_addr.sin_addr, sizeof (server->host->h_addr));
  /* initialise protocol prot number */
  server->sock_addr.sin_port = htons(server->port);
  server->sock_addr.sin_family = AF_INET;
  /* create a socket */
  server->socket = socket (AF_INET, SOCK_STREAM, 0);
  /* connect the socket to the server */
  namelen = sizeof (server->sock_addr);

  if ( bind(server->socket, (struct sockaddr*)&server->sock_addr, namelen) != 0 ) {
    w3Error ("Server: can't bind socket !");
    return false;
  }
  return true;
}

boolean w3ServerClose (w3Server *server) {
  w3ServerMessage ("Shutting down server");
  close (server->socket);
  /* This is dodgy. */
  /*
    if ( unlink ((const char*)&server->sock_addr) == -1) {
    w3Error ("Unable to unlink");
    perror("");
    }
  */
  return true;
}

boolean w3ServerDisconnect (w3ServerConnection* con) {
  w3ServerMessage("Disconnecting");
  close (con->socket);
  free (con);
  return true;
}

char* w3ServerGetToken (char *str, int *idx) {
  char *token = (char*)calloc (256, sizeof(char));
  int i = 0;
  int tokenSize = 256;
  char c = 'a';
  while ( (str[i] != ' ') && (str[i] != 0) && (str[i] != 13)) {
    c = str[i];
    token[i++] = c;
    if (i >= tokenSize) {
      tokenSize += 256;
      token = (char*)realloc ((void*)token, (tokenSize) * sizeof(char));
    }
  }
  token[i] = 0;
  printf("Got token -%s-\n",token);
  *idx = i + 1;
  return token;
}

char* w3ServerGetPath (char *str) {
  enum {
    PATH_DONE,
    PATH_GO,
    GET_PATH,
    PATH_ERROR
  };
  char *path = NULL;
  char *token = NULL;
  char *command = NULL;
  int i = 0;
  int state = PATH_GO;
  while (state != PATH_DONE) {
    switch (state) {
    case PATH_GO:
      token = w3ServerGetToken (str, &i);
      if (strcmp (token, "GET") == 0) {
	state = GET_PATH;
	command = copyString (token);
	free (token);
      } else {
	state = PATH_ERROR;
      }
      break;
    case GET_PATH:
      path = w3ServerGetToken (&str[i], &i);
      state = PATH_DONE;
      break;
    case PATH_DONE:
      break;
    case PATH_ERROR:
      token = composeString ("Error interpreting command ", command);
      w3ServerMessage (token);
      free (token);
      state = PATH_DONE;
      break;
    }
  }
  return path;
}

char* w3ServerReceiveRequest (w3ServerConnection *con) {
  size_t s = 0;
  char* data;
  unsigned int size = 0;
  
  data = (char*)calloc (1024,sizeof(char));
  
  do {
    s = read ( con->socket, (void*)(&((char*)data)[size]), 1024 );
    if ( s == -1 ) {
      w3Error ("receiving data");
      return NULL;
    }
    /* printf ("received %d bytes\n",s); */
    size += (unsigned int)s;
    printf(" %s\n",&data[size - s]);
    /* printf("reallocating %d...\n", *size); */
    data = (char*)realloc ( (void*)data, (size_t)((1024 + size) * sizeof(char)) );
    /* printf("New pointer: %p\n",data); */
  } while ( (s > 0) && (data[size - 1] != '\n') );

  /* debugging */
  /* printf("\n%s\n",(char*)data); */

  return data;
}

int w3ServerIdentifyCommand (char *cmd) {
  if (strcmp (cmd,"GET") == 0) {
    return W3_CMD_GET;
  }
  if (strcmp (cmd,"POST") == 0) {
    return W3_CMD_POST;
  }
  if (strcmp (cmd,"HEAD") == 0) {
    return W3_CMD_HEAD;
  }
  return W3_CMD_UNKNOWN;
}

boolean w3ServerRespond (char *request, w3ServerConnection *con) {
  /* For now, do only GET stuff ... */
  char *path;
  char *temp;
  char *token;
  int  idx = 0;
  
  /* path = w3ServerGetPath (request); */
  token = w3ServerGetToken (request, &idx);
  switch (w3ServerIdentifyCommand (token)) {
  case W3_CMD_GET:
    path = w3ServerGetToken (&request[idx], &idx);
    /* If client sent "GET / ", send standard index file. */ 
    if ( (strlen(path) == 1) || (path[strlen(path) - 1] == '/') ) {
      temp = composeString (path, HTDOC_STANDARDFILE);
      free(path);
      path = temp;
    }
    /* w3SendData ((void*)RESPONSE_OK, strlen (RESPONSE_OK) * sizeof(char), con->socket); */
    w3ServerSendFile (path, con);
    free (path);
    break;
  case W3_CMD_HEAD:
    w3ServerMessage ("Got HEAD request");
    w3ServerSendError (W3_ERROR_SERVICE_UNAVAILABLE, con);
    break;
  case W3_CMD_POST:    
    w3ServerMessage ("Got POST request");
    w3ServerSendError (W3_ERROR_SERVICE_UNAVAILABLE, con);
    break;
  default:
    w3ServerMessage ("Unknown request");
    w3ServerSendError (W3_ERROR_BAD_REQUEST, con);
    break;
  }
  
  return true;
}



boolean w3ServerSendFile (char *path, w3ServerConnection *con) {
  FILE		*f;
  char		*filename;
  char		*tmp;
  boolean	isBin;
  pid_t		PID; 
  int		i, l;
  char		*tempFilename;
  FILE          *temp_stdout;
  char		buffer[10240];
  size_t	bytes = 0;

  filename = composeString (HTDOC_BASE, path);

  w3ServerMessage(filename);
  isBin = false;

  /****************************************************************/
  /* Check for and handle requests for binaries in HTDOC_BIN_BASE */
  /* tmp = copyString (path); */
  l = strlen (HTDOC_BIN_BASE);
  if ( strlen (path) >= l ) {
    isBin = true;
    for (i = 0; i < l; i++) {
      if (path[i] != HTDOC_BIN_BASE[i]) {
	isBin = false;
	break;
      }
    }
    if (isBin == true) {
      if ( !((i < strlen (path)) && (path[i] == '/')) ) {
	isBin = false;
      }
    }
    if (isBin == true) {
      /* Try to execute the filename, since it seems to be in /bin: */
      tmp = copyString (HTDOC_BIN_TEMPLATE);
      if ( mktemp (tmp) == NULL ) {
	w3ServerMessage ("Could not create temporary filename\n");
	free (tmp);
	return false;
      }
      tempFilename = composeString (tmp, HTDOC_DOTHTML);
      free (tmp);
      PID = fork();
      switch (PID) {
      case 0: 
	tmp = composeString (HTDOC_BASE, tempFilename);
	free (tempFilename);
	tempFilename = tmp;
	tmp = NULL;
	/* tmp = composeString ("> ", tempFilename); */
	// fclose (stdout);
	freopen (tempFilename, "w", stdout);
	execl (filename, filename, NULL);
	perror ("executing file");
	// fclose(stdout);
	break;
      default: waitpid (PID, NULL, 0);
	break;
      }
      w3ServerSendFile (tempFilename, con);
      free (tempFilename);
      return true;
    }
  }
  /****************************************/
  
  f = fopen (filename, "r");

  for (i = 0; i < 10240; i++) {
    buffer[i] = 0;
  }

  if (f == NULL) {
    free (filename);
    w3ServerSendError (W3_ERROR_NOT_FOUND, con);
    return false;
  }
  while (!feof(f)) {
    bytes = fread ( (void*)&buffer[0], sizeof(char), 10240, f);
    if (ferror(f)) {
      w3ServerMessage ("Error reading requested file, terminating connection");
      w3ServerSendError (W3_ERROR_NOT_FOUND, con);
      fclose(f);
      free (filename);
      return false;
    }
    w3SendData ((void*)&buffer[0], bytes, con->socket);
  }
  fclose(f);
  free (filename);
  return true;
}

boolean w3ServerSendError (int errorcode, w3ServerConnection *con) {
  char *page;
  switch (errorcode) {
  case W3_ERROR_NOT_FOUND:
    page = w3HTMLGenerateError (NULL, NULL, W3_HTML_404);
    w3SendData ((void*)page, strlen(page) * sizeof(char), con->socket);
    free (page);
    break;
  case W3_ERROR_SERVICE_UNAVAILABLE:
    page = w3HTMLGenerateError (RESPONSE_SERVICE_UNAVAILABLE, NULL, W3_HTML_503);
    w3SendData ((void*)page, strlen(page) * sizeof(char), con->socket);
    free (page);
    break;
  case W3_ERROR_BAD_REQUEST:
    page = w3HTMLGenerateError (RESPONSE_BAD_REQUEST, NULL, W3_HTML_400);
    w3SendData ((void*)page, strlen(page) * sizeof(char), con->socket);
    free (page);
    break;
  default:
    page = w3HTMLGenerateError (RESPONSE_BAD_REQUEST, NULL, 0);
    w3SendData ((void*)page, strlen(page) * sizeof(char), con->socket);
    free (page);
    w3ServerMessage("SendError: no such error code");
    break;
  }
  return true;
}

boolean w3ServerServeClient (w3Server *server, w3ServerConnection *con) {
  char *str;

  str = w3ServerReceiveRequest (con);
  if (str == NULL) {
    w3ServerMessage("No data received !\n");
  }
  /* w3ServerMessage (str); */
  w3ServerRespond (str, con);
  free (str);
  return true;
}

void* acceptThread (void* p) {
  w3ServerConnection *con;  
  w3Thread *thread;
  con = w3ServerAccept ((w3Server*)p);
  thread = w3ThreadRun ((void*)acceptThread, p);

  if (con != NULL) {
    w3ServerServeClient ((w3Server*)p, con);
    w3ServerDisconnect (con);
  }
  free (thread);
  return NULL;
}

/*!
 * 
 */
boolean w3StartServer (int port) {
  w3ServerConnection *con;
  w3Server server;
  w3Thread *thread[5];
  int i;
  server.port = port;
  w3ServerInit (&server);

  for (i = 0; i < 5; i++) {
    thread[i] = w3ThreadRun ((void*)acceptThread, (void*)&server);
  }
  while (1) {
    con = w3ServerAccept (&server);
    if (con != NULL) {
      w3ServerServeClient (&server, con);
      w3ServerDisconnect (con);
    }
  }
  w3ServerClose(&server);
  return false;
}

void w3ServerMessage (const char* msg) {
  printf("w3Server: %s\n", msg);
}
















