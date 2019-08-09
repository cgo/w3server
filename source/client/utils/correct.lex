/*!
 * \file correct.lex
 * Flex source file for a correctional analyzer.
 * Corrects IMG tags in a downloaded file to point to the current directory.
 * Pipe any downloaded html file containing IMG tags through this 
 * analyzer before displaying it.
 * This is not a good programming style but is needed in order to maintain
 * the flat storage structure used by the Tcl code for the browser.
 * Replace this by a better solution when you want to make a professional
 * browser out of this basic one.
 * \author Christian Gosch
 */

%{
#include <stdio.h>
%}
SPACES    (" "|"\t"|"\n")*
IMAGE     {SPACES}(img|IMG){SPACES}
SOURCE    {SPACES}(src|SRC){SPACES}("="|"\n"){SPACES}
STRING    ([a-zA-Z0-9]|"."|"/"|"\n"|"_"|"-")+
HTTPSTRING  (http|HTTP)"://"
IMGFRONT  ("<")(" ")*{IMAGE}(.*){SOURCE}("\"")*
IMGTAIL   ("\"")*(" ")*(">")
PATH      {STRING}"/"
FILENAME  ([a-zA-Z0-9]|"."|"_"|"-")+

%x IMAGENAME
%x IMAGEFILENAME
%x IMAGETAIL
%x IMAGEREMOTE

%%

{IMGFRONT} {
  printf("%s",yytext);
  BEGIN(IMAGENAME);
}

<IMAGENAME>{HTTPSTRING} {
  printf("%s",yytext);
  BEGIN(IMAGEREMOTE);
}

<IMAGENAME>{PATH} {
}

<IMAGENAME>{FILENAME} {
  printf("%s",yytext);
  BEGIN(IMAGETAIL);
}

<IMAGEREMOTE>{STRING} {
  printf("%s",yytext);
  BEGIN(IMAGETAIL);
}

<IMAGETAIL>{IMGTAIL} {
  printf("%s",yytext);
  BEGIN(INITIAL);
}


%%
main () {
  yylex();
}








