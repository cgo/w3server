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
* \mainpage Documentation for w3server
* - \subpage serverdoc
* - \subpage clientdoc
*/
/*!
 * \page serverdoc Documentation for the server
 * \section intro Introduction
 *  This is the documentation for the server program. <br>
 *  It is a simple multithreaded HTTP server.
 *  The second part of the report can be found <a href="../../../docs/report.html">here</a>. 
 *  Note that there may be links in that report to code documentation
 *  that don't work. Use this documentation directly instead.
 * \section make Building server
 *  \subsection need What you need
 *  <ul>
 *   <li>Linux or Solaris <br>
 *    (Tested with Linux 2.2.13 and libc6)</li>
 *   <li>pthread library</li>
 *   <li>gcc or similar compiler (tested with egcs 2.91-66)</li>
 *   <li>GNU make</li>
 *   <li>libfl (from the flex package)</li>
 *   <li>flex if you wish to remake the C-code for the lexical analyzers
 *    from the .lex files (usually not)</li>
 *  </ul>
 *
 * \subsection build Building it
 *  server needs a little bit more attention than client:
 *  <ul>
 *   <li>Change to the server directory </li>
 *   <li>Type <code>./configure</code> to configure the source for your system.
 *       If configure reports any error, the source will very likely not build 
 *       correctly. Please report bugs to the authors.</li>
 *   <li>Edit the file src/w3server.c and <STRONG>change
 *       the global variable</STRONG> <CODE>HTDOC_BASE</CODE> to the root directory
 *       containing your html files. Have a look at the other variables, even
 *       if they are unlikely to require any changes. </li>
 *   <li>Type <CODE>make</CODE> at the command prompt. This 
 *   should build the server executable in the server directory.<br>
 *   If something does not compile, it might be a good idea to call <CODE>make clean</CODE> first.
 *   Optional: Call <CODE>make clean</CODE> to delete all object 
 *   files and auto-save
 *   files. <CODE>make stat</CODE> outputs a little code statistics.
 *   You need the <CODE>wc</CODE> program to use this.</li>
 *  </ul>
 * \section usage Usage
 *  The command line format to call <CODE>server</CODE> is: <br>
 *  <CODE>server port</CODE> <br>
 *  Where
 *   <ul>
 *    <li><CODE>port</CODE> is the port number the server is running on.
 *        The standard is 80, but you must supply the port number anyway.</li>
 *   </ul>
 *
 *
 * \section theory Technical: Operation of server
 *
 *  \subsection main main.c

Call to w3StartServer (in w3server.c), the argument being the
portnumber.
This call does everything the calling function wants, namely setting
up a simple HTTP server for the given port :-)

 *  \subsection start w3StartServer
Calls w3ServerInit to initialize the server.
This sets up the socket and fills in the given w3Server structure.

Sets up 5 threads of execution which do the following:
 1. Call w3ServerAccept to accept a connection request from a client.
   1.a) when a connection is accepted, a new thread is immediately
        started to replace the actual thread.
 2. Call w3ServerServeClient to handle anything the client could want.
 3. Call w3ServerDisconnect to close the connection.

 *  \subsection accept w3ServerAccept

Calls the listen() library function to set the socket up to listen to
one incoming connection request.
Calls the accept() library function to accept a connection on the
previously created socket (created by w3ServerInit).
Returns a pointer to a w3ServerConnection structure.

 *  \subsection serve w3ServerServeClient
Receives a request string from the (connected) client
and responds to it using w3ServerRespond.

 *  \subsection respond w3ServerRespond
Parses the given request string for the requests
GET, POST, HEAD, of which currently only
GET is handled properly (I hope).
It responds to GET by sending the requested file to the
client.

 *  \subsection errors Error handling
It was tried to catch any error that is likely to occur,
but since this is an early development stage, nothing is
guaranteed ;)
Errors like file-not-found in a request are handled by
sending an error page to the client (see routines in
w3html.h / w3html.c).


 *  \section bugs Known bugs
 *   Currently, no bugs are known. If you encounter a bug, please report it
 *   to <br>
 *   <a href=mailto:christian@goschs.de>Christian Gosch</a> <br>
 *   or <br>
 *   <a href=mailto:tonly002@students.unisa.edu.au>Liuben Tonev</a> <br>
 */

/*!
 * \page clientdoc Documentation for the client
 * \section intro Introduction
 *  This is the documentation for the client program. It is the 
 *  command line part of the HTML browser.
 *  The second part of the report can be found <a href="../../../../docs/report.html">here</a>.
 * \section makeclient Building client
 *  \subsection need What you need
 *  <ul>
 *   <li>Linux or Solaris <br>
 *    (Tested with Linux 2.2.13 and libc6)</li>
 *   <li>pthread library</li>
 *   <li>gcc or similar compiler (tested with egcs 2.91-66)</li>
 *   <li>GNU make</li>
 *   <li>libfl (from the flex package)</li>
 *   <li>flex if you wish to remake the C-code for the lexical analyzers
 *    from the .lex files (usually not)</li>
 *   <li>GNU autoconf if you want to remake the configure scripts</li>
 *  </ul>
 *
 * \subsection build Building it
 *  There are few things to do: 
 *  <ul>
 *   <li>Change to the client directory </li>
 *   <li>Type <code>./configure</code> to configure the source for your system.
 *       If configure reports any error, the source will very likely not build 
 *       correctly. Please report bugs to the authors.</li>
 *   <li>Type <CODE>make</CODE> at the command prompt. This 
 *   should build the client executable in the client directory.<br>
 *   Optional: Call <CODE>make clean</CODE> to delete all object 
 *   files and auto-save
 *   files. <CODE>make stat</CODE> outputs a little code statistics.
 *   You need the <CODE>wc</CODE> program to use this.</li>
 *   <li>In the rare case you should want to rebuild the flex analyser, call <br>
 *       <code>flex -i img.lex</code> <br>
         in the source directory and then <br>
	 <code>make clean; make</code> <br>
	 in the client directory.
 *  </ul>
 * \section usage Usage
 *  The command line format to call <CODE>client</CODE> is: <br>
 * \subsection usageget For getting a HTML page
 *  <CODE>client get full-path-to-file destination-file port</CODE> <br>
 *  Where
 *   <ul>
 *    <li><CODE>full-path-to-file</CODE> is the full URI, like <br>
 *        <CODE>io.iupiter.universe/www/hund.html</CODE>. </li>
 *    <li><CODE>destination-file</CODE> is the local file where the 
 *        downloaded file is to be stored.</li>
 *    <li><CODE>port</CODE> is the port number the server is running on.
 *        Usually, this is 80.</li>
 *   </ul>
 *
 * \subsection usagemail For sending mail
 * <CODE>client mail mailfile</CODE> <br>
 * Where
 * <ul>
 *  <li><CODE>mailfile</CODE> is a filename of a file of the following format: <br>
 *    First line: Recipient <br>
 *    Second line: Sender <br>
 *    Third line: mailer <br>
 *    Fourth line: port number on which the mailer accepts connections <br>
 *    The following lines contain the mail body, including ALL headers the
 *    sender wants to be sent (including sender and recipient headers).
 *  </li>
 * </ul>
 *
 * \section cachestuff Downloaded Files
 * If you download a HTML file, for example www.animals.org/dog/cat/mouse.html,
 * it is stored in the given destination file AND in the cache subdirectory (which is specified in 
 * client/src/w3clientconfig.h). The HTML file is scanned for image tags and the corresponding images 
 * are downloaded and stored in the cache directory, in this example that could be <br>
 * cache/www.animals.org/pictures/dog.gif <br>
 * The HTML pages are stored flatly in cache/, e.g. as <br>
 * cache/www.animals.org_index.html <br>
 * The slashes are replaced by underscores. This CAN lead to clashes but that is 
 * neglected for this program. In a release version, another mechanism would have to be
 * used.
 */











