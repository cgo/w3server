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
#include <w3html.h>
#include <w3string.h>
#include <stdlib.h>

char W3_HTML_ERROR_HEAD[] = "<html><head><title>\nW3 Server error message</title></head>\n\
<body bgcolor=black text=white><img src=/icons/w3server1.png alt=\"W3 Server / CNS 2000\">\n";
char W3_HTML_ERROR_FOOT[] = "<hr>\n <address>W3 Server / Christian Gosch</address><br>\n<copyright>Copyright (C) by Christian Gosch. This software is distributed under the terms of the GNU General Public License, which should be supplied with the source code of the software.</copyright><br>\n\
</body></html>\n";
char W3_HTML_PAGE_400[] = "<h2>Bad request</h2>\n";
char W3_HTML_PAGE_404[] = "<h2>Not found</h2>\n";
char W3_HTML_PAGE_503[] = "<h2>Service unavailable</h2>\n";
char W3_HTML_PAGE_XXX[] = "<h2>General error</h2>\n";


char *w3HTMLGenerateError (char *header, char *info, int pagecode) {
  char *page;
  char *body;
  char *tmp;
  
  /* Make page body */
  switch (pagecode) {
  case W3_HTML_400:
    body = W3_HTML_PAGE_400;
    break;
  case W3_HTML_404:
    body = W3_HTML_PAGE_404;
    break;
  case W3_HTML_503:
    body = W3_HTML_PAGE_503;
    break;
  default:
    body = W3_HTML_PAGE_XXX;
    break;
  }
  if (header != NULL) {
    tmp  = composeString (header, W3_HTML_ERROR_HEAD);
  } else {
    tmp = copyString(W3_HTML_ERROR_HEAD);
  }
  page = composeString (tmp, body);
  free (tmp);
  tmp = page;
  if (info != NULL) {
    page = composeString (tmp, info);
    free (tmp);
    tmp = page;
  } 
  page = composeString (tmp, W3_HTML_ERROR_FOOT);
  free (tmp);
  return page;
}




