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
 * \file img.lex
 * Flex code for a lexical analyzer that retrieves IMG tags out of
 * html code. This is used by functions in <CODE>w3client.h</CODE> to 
 * download all images referenced in an html file.
 * The second task this analyzer performs is to replace all
 * relative <A HREF tags with absolute ones (http://server.com/....)
 * This feature has not yet been entirely tested. 
 * \author Christian Gosch
 */

%{
#include <stdio.h>
#include <w3client.h>
#include <w3string.h>

extern struct clientGlobalStruct clientGlobal;

int main2 (int, char**);
%}
SPACES    (" "|"\t"|"\n")*
IMAGE     {SPACES}(img|IMG){SPACES}
SOURCE    {SPACES}(src|SRC){SPACES}("="){SPACES}
/* IMAGE     img|IMG("\n")* */
/* SOURCE    (src|SRC)(" "|"\n")*("="|"\n")(" "|"\n")* */
STRING    ([a-zA-Z0-9]|"."|"/"|"\n"|"_")+
HTTPSTRING  (http|HTTP)"://"
OTHERTAGS ([a-zA-Z]*("\"")*[a-zA-Z0-9]*("\"")*) 
IMGFRONT  ("<"){IMAGE}
SRCFRONT  {SOURCE}("\"")*
IMGTAIL   ("\"")*(" ")*(">")
EQUALS    {SPACES}("="){SPACES}("\"")*
ATAG      ("<"){SPACES}(a|A)
HREFTAG   (href|HREF){EQUALS}
ATAGEND   {SPACES}("\"")*{SPACES}("/a"){SPACES}(">")

%x SOURCEFRONT
%x IMAGENAME
%x IMAGETAIL
%x IMAGEREMOTE
%x ATAGFOUND
%x HREFTAGFOUND
%x ATAGENDFOUND

%%
{IMGFRONT} {
  ECHO;
  BEGIN(SOURCEFRONT);
}

{ATAG} {
  ECHO;
  BEGIN(ATAGFOUND);
}

<SOURCEFRONT>{SRCFRONT} {
  ECHO;
  BEGIN(IMAGENAME);
}

<IMAGENAME>{HTTPSTRING} {
  BEGIN(IMAGEREMOTE);
}

<IMAGENAME>{STRING} {

  /* printf("Lexical analyzer found %s\n", yytext); */
  w3ClientGetImagePath (yytext, 1);

  fwrite (clientGlobal.hostname, strlen (clientGlobal.hostname), 1, yyout);
  /* fprintf(stderr,"imagename: %s\n", yytext); */
  /* fprintf(stderr,"hostname: %s\n", clientGlobal.hostname); */
  if (yytext[0] != '/') {
    /* fwrite ("/", 1, 1, yyout); */
    fwrite ( clientGlobal.lastpath, strlen (clientGlobal.lastpath), 1, yyout );
    /* fprintf(stderr,"lastpath: %s\n", clientGlobal.lastpath); */
  }
  
  ECHO;
  /* BEGIN(IMAGETAIL); */
  BEGIN(INITIAL);
}

<IMAGEREMOTE>{STRING} {
  /* printf("Lexical analyzer found remote %s\n", yytext); */
  w3ClientGetImagePath (yytext, 0);
  ECHO;
  /* BEGIN(IMAGETAIL); */
  /* Some image tags were not ended correctly .. */
  BEGIN(INITIAL);
}

<IMAGETAIL>{IMGTAIL} {
  ECHO;
  BEGIN(INITIAL);
}



<ATAGFOUND>{HREFTAG} {
  ECHO;
  BEGIN(HREFTAGFOUND);
}

<HREFTAGFOUND>{HTTPSTRING} {
  ECHO;
  BEGIN(INITIAL);
}

<HREFTAGFOUND>{STRING} {
  fwrite ("http://", 7, 1, yyout);
  fwrite ( clientGlobal.hostname, strlen (clientGlobal.hostname), 1, yyout);
  if (yytext[0] != '/') {
    fwrite ( clientGlobal.lastpath, strlen (clientGlobal.lastpath), 1, yyout );
  }    
  ECHO;
  BEGIN(INITIAL);
}


<*>.|\n { ECHO; };

%%
int main (int argc, char *argv[]) {
  return main2(argc,argv);
}















