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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <w3client.h>
#include <w3types.h>
#include <w3uri.h>
#include <w3error.h>
#include <w3string.h>
#include <w3comm.h>
#include <w3thread.h>
#include <w3clientconfig.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

/* For mkdir */
#include <sys/stat.h>
#include <fcntl.h>

char* W3_CLIENT_CACHE_DIR  = W3_CLIENT_CACHE_DIR_DEF;
char* W3_CLIENT_INDEX_HTML = W3_CLIENT_INDEX_HTML_DEF;

extern FILE* yyout;

struct clientGlobalStruct clientGlobal;

void yy_scan_buffer (const char *buffer, int len);

void w3ClientInit () {
  clientGlobal.numOfThreads = 0;
  clientGlobal.threads = (w3Thread**)malloc (sizeof (w3Thread*));
  clientGlobal.lastpath = NULL;
}

boolean w3ClientConnect (w3ClientConnection *con) {
  struct hostent *host;

  host = gethostbyname (con->hostname);
  if ( host == NULL ) {
    printf("Connect to host %s returned error.\n",con->hostname);
    w3Error ("finding hostname");
    return false;
  }

  con->socket = socket (AF_INET, SOCK_STREAM, 0);
  
  con->sockaddr.sin_family = AF_INET;
  con->sockaddr.sin_port   = htons (con->port);
  memcpy ((char*)&(con->sockaddr.sin_addr),(char*)(host->h_addr), host->h_length);
  /* con->sockaddr.sin_addr   = ((struct in_addr*)host->h_addr); */

  if ( connect (con->socket, (struct sockaddr*)&(con->sockaddr), sizeof(con->sockaddr)) != 0 ) {
    w3Error ("connecting to remote host");
    return false;
  }

  return true;
}

boolean w3ClientDisconnect (w3ClientConnection *con) {
  if ( close (con->socket) != 0 ) {
    return false;
  }
  return true;
}

/*
 * Creates a cache path for the file uriString.
 * uriString needs to be the complete path (including filename or last character a '/')
 * of a file (complete URI).
 */ 
static void createCachePath (char *uriString) {
  char *temp;
  int i;
  int iSave;
  char charSave;
  char c;
  i = strlen(uriString) - 1;
  while ( (i >= 0) && (uriString[i--] != '/') );
  if (i >= 0) {
    i++;
    iSave = i;
    charSave = uriString[i];
    uriString[i] = 0;
#ifdef __W3CLIENT_DEBUG
    printf("Path is now %s\n", uriString);
#endif
    i = 0;
    temp = composeString (W3_CLIENT_CACHE_DIR, uriString);
    while ( i < strlen(temp) ) {
      while ( (i < strlen(temp)) && (temp[i++] != '/') );
      c = temp[i];
      temp[i] = 0;
#ifdef __W3CLIENT_DEBUG
      printf("Creating %s\n", temp);
#endif
      mkdir ((const char*)temp, 511);
      temp[i] = c;
#ifdef __W3CLIENT_DEBUG
      perror("");
#endif
    }
    free (temp);
    uriString[iSave] = charSave;
  }
}

boolean w3ClientGetImagePath (char *uriString, boolean withoutHostname) {
  w3Document doc;
  char *tmp;
  char *filename = NULL;
  FILE *f = NULL;
  unsigned int i = 0;
  int i2 = strlen(uriString);
  do {
    i2--;
  } while ( (i2 >= 0) && (uriString[i2] != '/') );
  /* filename = copyString (&uriString[i2+1]); */

  doc.uri.filename = NULL;

  /* This is a bit messy ... check if it is a relative path and add the document base 
   * if necessary. 
   */
  if (withoutHostname == 1) {
    if (uriString[0] != '/') {
      doc.uri.path = composeString (clientGlobal.lastpath, uriString);
      doc.uri.hostname = copyString (clientGlobal.hostname);
    } else { doc.uri.path = copyString (uriString); }
    doc.uri.hostname = copyString (clientGlobal.hostname);
  }
  else { w3ParseURI (uriString, &doc.uri); }

  /* Set port to global port setting. */
  doc.uri.port = clientGlobal.port;
  tmp = composeString (doc.uri.hostname, doc.uri.path);
  createCachePath (tmp); 
  filename = composeString (W3_CLIENT_CACHE_DIR, tmp);
#ifdef __W3CLIENT_DEBUG  
  printf("Filename is %s\n", filename);
#endif
  free (tmp);
  if ( w3ClientGetDocument (&doc) == true ) {
    f = fopen (filename, "w");
    if ( f == NULL ) {
      w3Error ("Error opening file for download.\n");
      if (filename) {
	free (filename);
      }
      return false;
    } else {
      for ( i = 0; i < doc.dataLength; i++ ) {
	fputc ( ((char*)doc.data)[i], f);
      }
      fclose (f);
      if (filename) {
	free (filename);
      }
      return true;
    }
  }
  return false;
}


/*
 * Gets all images referenced to in doc. Not used at the moment.
 */
boolean w3ClientGetAllImages (w3Document *doc) {
#ifdef __W3CLIENT_DEBUG
  printf("Calling the parser ...\n");
#endif
#ifndef NO_FLEX
  yy_scan_string ((const char*)doc->data, doc->dataLength);
  yylex();
#endif
  return true;
}

boolean w3ClientGetDocument (w3Document *doc) {
  w3ClientConnection con;
  char *commandString;
  char *temp, *temp2;
  char *getCmd = "GET ";
  char cr_string[] = {13,10,0};

  con.hostname = copyString (doc->uri.hostname);
  con.port     = doc->uri.port;
  if ( w3ClientConnect (&con) == false ) {
    return false;
  }
  
  temp = composeString (doc->uri.path, doc->uri.filename);
  temp2 = composeString (getCmd, temp);
  commandString = composeString (temp2, cr_string);
#ifdef __W3CLIENT_DEBUG
  printf ("Sending string: %s\n",commandString);
#endif
  /* Send command string. */
  w3SendData ((const void*)commandString, strlen(commandString) * sizeof(char), con.socket);
  doc->data = w3ReceiveData (&doc->dataLength, con.socket);
  
  w3ClientDisconnect (&con);
  free (temp);
  free (temp2);
  free (commandString);

  return true;
}

boolean w3ClientDocToFile (const char *uriString, const char *filename, int port) {
  w3Document doc;
  char *tmp;
  char *cacheFilename;
  FILE *f = NULL;
  long filePos = 0;
  FILE *cacheF = NULL;
  unsigned int i = 0;
  clientGlobal.port = port;
  doc.uri.port = port;
  clientGlobal.hostname = NULL;
  if ( w3ParseURI (uriString, &doc.uri) == true ) {

    clientGlobal.hostname = copyString (doc.uri.hostname);

    if (strcmp (doc.uri.path, "/") == 0) {
      tmp = composeString (doc.uri.hostname, W3_CLIENT_INDEX_HTML);
    } else {
      tmp = composeString (doc.uri.hostname, doc.uri.path);
    }
    /* createCachePath (tmp); */
    for (i = 0; i < strlen (tmp); i++) {
      if (tmp[i] == '/') {
	tmp[i] = '_';
      }
    }
    cacheFilename = composeString (W3_CLIENT_CACHE_DIR, tmp);
    free (tmp);

    if ( w3ClientGetDocument (&doc) == true ) {
      /********************
       * Get images       *
       ********************/
      tmp = copyString (doc.uri.path);
      i = strlen(tmp) - 1;
      while ( (i > 0) && (tmp[i] != '/') ) {
	i--;
      }
      i++;
      tmp[i] = 0;
      if (clientGlobal.lastpath != NULL) {
	free (clientGlobal.lastpath);
      }
      clientGlobal.lastpath = copyString (tmp);
#ifdef __W3CLIENT_DEBUG
      printf("lastpath is now %s\n", clientGlobal.lastpath);
#endif
      free (tmp);
      /* w3ClientGetAllImages (&doc); */
      /********************************/
      
      f = fopen (filename, "w");
#ifdef W3CLIENT_CREATE_CACHE
      cacheF = fopen (cacheFilename, "w");
      if ( (f == NULL) || (cacheF == NULL) ) {
#else
      if ( (f == NULL) ) {
#endif

	w3Error ("Error opening file for download.\n");
	return false;
      }

      printf("doc.dataLength: %d\n", doc.dataLength);
#ifndef NO_FLEX
      yy_scan_bytes ((const char*)doc.data, doc.dataLength);
      yyout = f;
      yylex();
 #ifdef W3CLIENT_CREATE_CACHE
      filePos = ftell (f);
 #endif
      fclose (f);
      f = fopen (filename, "r");
 #ifdef W3CLIENT_CREATE_CACHE
      for (i = 0; i < filePos; i++) {
	fputc ( (int)fgetc(f), cacheF);
      }
 #endif
#else
      for ( i = 0; i < doc.dataLength; i++ ) {
	fputc ( (int)((char*)doc.data)[i], f);
 #ifdef W3CLIENT_CREATE_CACHE
	fputc ( (int)((char*)doc.data)[i], cacheF);
 #endif
      }
#endif
      fclose (f);
#ifdef W3CLIENT_CREATE_CACHE
      fclose (cacheF);
#endif
    }

    /* Hack around the dodgy added lines at the end of html files */
    /* 
    f      = fopen (filename,"rw");
    cacheF = fopen (cacheFilename,"rw");
    for ( i = 0; i < doc.dataLength; i++ ) {
      fputc ( (int)((char*)doc.data)[i], f);
      fputc ( (int)((char*)doc.data)[i], cacheF);
    }
    */
    free (cacheFilename);
  }

  if (clientGlobal.hostname != NULL) {
    free (clientGlobal.hostname);
    clientGlobal.hostname = NULL;
  }
  /* Donkey-rapin' bullshit !!! */
  /* free (clientGlobal.lastpath); */
  return true;
}

struct MTDocStruct {
  char *uriString;
  char *filename;
  int port;
};


void* MTGetDoc (void* arg) {
  FILE *f;
  char *flagFileName;
  struct MTDocStruct *str = (struct MTDocStruct*)arg;

  w3ClientDocToFile ( str->uriString, str->filename, str->port );

  /* Touch the flag file. */
  flagFileName = composeString (str->filename, ".done");
  f = fopen (flagFileName, "w");
  fclose (f);

  free (flagFileName);
  free (str->uriString);
  free (str->filename);
  free (arg);
  return NULL;
}

boolean w3ClientDocToFileMT (const char *uriString, const char *filename, int port) {
  w3Thread *retrieveThread;
  struct MTDocStruct *arg = (struct MTDocStruct*)malloc (sizeof (struct MTDocStruct));
  
  arg->uriString = copyString ((char*)uriString);
  arg->filename  = copyString ((char*)filename);
  arg->port      = port;

  retrieveThread = w3ThreadRun ((void*)MTGetDoc, (void*)arg);

  if (retrieveThread != NULL) {
    clientGlobal.numOfThreads++;
    clientGlobal.threads = (w3Thread**)realloc ((void*)clientGlobal.threads, sizeof (w3Thread*) * clientGlobal.numOfThreads);
    clientGlobal.threads[clientGlobal.numOfThreads - 1] = retrieveThread;
    return true;
  }
  return false;
}

boolean w3ClientJoinAllThreads () {
  int i;
  for (i = 0; i < clientGlobal.numOfThreads; i++) {
    w3ThreadJoin (clientGlobal.threads[i]);
  }
  clientGlobal.numOfThreads = 0;
  return true;
}

boolean w3ClientDestroyAllThreads () {
  int i;
  for (i = 0; i < clientGlobal.numOfThreads; i++) {
    w3ThreadDestroy (clientGlobal.threads[i]);
  }
  clientGlobal.numOfThreads = 0;
  return true;
}











