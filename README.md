# WinCastle: A Text Based Adventure
Castle (c) 2001 Beem Media. All rights reserved.

This is the second iteration of the Win32 C++ version of Castle.

## Building

A visual studio 2022 project is provided to build the binary for Windows
platforms. The code was updated to make that possible.

## History

* Originated: 1997
* This Version: November 27, 2001

### Original README.TXT
<pre>
WinCastle: A Text Based Adventure v4.xx for 32 bit Windows (Windows 9X, ME, 2000, XP)

Castle BUILD 4
Copyright (c) 1997, 2000, 2001 Blaine Myers for Beem Software

This folder contains:

	wCastle.exe - The executable WinCastle game.
	Advent.tba - Text Based Adventure(TBA) for the castle executable.
	Readme.txt - This file.
	Version.txt - A history of the game's versions.

WinCastle will read a Text Based Adventure file(usually with extension .tba) and run it.

Instructions:  To play press either 1 or 2 when prompted.  Use the game|restart menu command to restart your current game.  To play a custom map use game|custom... and select a valid .tba file.

Programming .tba files:
There is currently no documentation available for .tba construction.  I might write one someday.

Known Issues:

Issue #1: If you press cancel while in the custom game loader then use the restart feature the program will mysteriously terminate.  This does not occur under Windows XP.

Issue #2: Selecting an invalid map as a custom game will cause random errors.

Future of Castle:
We have not planned anymore updated versions of WinCastle.  Though, we do admit we would like to add a few more featurs to the Castle script.  Namely: The ability to take user input  strings (ie. a name, or a number).  The ability to use simple mathmatics within an adventure (ie. a score, or an amount of cash).  The ability to make more than two choices.  The ability to use mouse to make choices.  Music.  The ability to display a picture during an adventure.

Technical Stuff:

WinCastle copyright © 1997, 2000, 2001, Blaine Myers.  Beem Software, Castle, Castle BUILD, Castle Script, WinCastle and their respective logos are trademarks or registered trademarks of Blaine Myers in the U.S. and/or other countries.  Beem Software is a division of Blaine Myers.  All other trademarks belong to their respective owners.

Castle BUILD:

The Castle BUILD is the engine used to run castle games, here is some information about all the BUILD's

BUILD 1: The original QuickBasic build of Castle, a fairly crude, very outdated text adventure.  Had extensive use of the obsolete GOTO statement.  BUILD 1 is any 1.xx version of Castle.

BUILD 2: The second build of castle.  Written in the C language it had a much better interface.  The user was no longer required to type in commands.  Only hit a single keystroke.  Story was somewhat rewritten, lots of spelling mistakes corrected.  Build 2 is any 2.xx version of castle.

BUILD 3: Castle finally got a script to run, allowing custom text adventures.  The script was simple and worked very effectively with four different functions.  Build 3 is versions 3.xx.

BUILD 4: The most current build of Castle.  The Main program was renamed WinCastle instead of just Castle.  Now Castle is in a Win32 API supporting win 9x and probably ME.  Uses the same script as BUILD 3 and most parts of the game functions are the same.  Advantages are that it uses menus to control the special features in the game.  BUILD 4 is version 4.xx of Castle.
</pre>

### Original VERSION.TXT
<pre>
=========================================================
=== Version History of                                ===
===        WinCastle: A Text Based Adventure          ===
=========================================================
version 4.07 (June 02, 2002)
	Now fully supports WindowsXP.

version 4.06 (March 24, 2002)
	Improved WinXP support.

version 4.05 (March 11, 2002)
	General C++ optimizations, none of which are
	noticable to the user.

version 4.04 (November 29, 2001)
	Added accelerators to speed things up.  Such as
	restart game which is F3.

version 4.03 (November 27, 2001)
	Dialogs now appear as they should, as I am now
	compiling under microsoft visual c++ instead of
	borland.  The error on terminatoin bug when
	using a custom game is now gone.

version 4.02 (November 01, 2001)
	Improved Dialog by adding an icon.  The text
	displaying the version is now altered by the
	program for easier update.  To change the
	version change the definition of CURVERSION.
	
	Map statistics are no longer displayed at the
	bottom of the screen.  You may go under the
	map|properties to view the stats.

	You may also play a custom map by using the file
	name as a perameter (drag the file into the exe)

version 4.01 (October 30, 2001)
	Minor changes to the code, now menus have
	underlined values.  Window is no longer resizable.
	Added About Dialog Box as well.

version 4.00 (October 6, 2001)
	Caslte updated to WinCastle uses a Win32 API much
	improved over previous versions.  Uses menus for
	special commands.

version 3.02 (September 20, 2001)
	This change only applies to the source code.  The
	game is still played exactly the same.  I added
	a class called CasteGame which contains all the
	functions of a game of castle.  Declaring a game
	and then running the playGame function will the
	filename parameter as the map you wish to play
	will take care of everything.

version 3.01 (May 24, 2001)
	Added support for user maps.  Either drag them
	over the .exe file, use the map name as an argument,
	or us windows Open With command.

version 3.00 (December 31, 2000)
	Complete redo of the game.  The main .exe now reads
	a .tba file and plays the adventure from that. The
	story is the same but I fixed some grammar and
	spelling errors.  I will admit spelling errors
	probably still exist. I also dropped Roleplaying
	out of the title because it really isn't a
	roleplaying game.

version 2.08 (December 10, 2000)
	Redid the enter your name feature now it will print
	out your last name.  Minor code repair.

version 2.07 (August 4, 2000)
	You are now prompted to type your name, and it 
	will appear 2x in the game.  When you begin, and
	when you die.

version 2.06 (July 23, 2000)
	Eliminated the use of the goto statement, by having
	the program call different functions.

version 2.05 (July 10, 2000)
	Used the getch() function rather than the getchar()
	and getc() functions to take input. So now you
	only have to press the number of your choice and
	not press enter.  This eliminates major bugs.

version 2.00 (June 27, 2000)
	Castle was converted to the C language and the 
	name was changed to Castle: A Text Based 
	Roleplaying Adventure.  The game only required you
	to type a number an press enter, rather than 
	typing an entire word out.

version 1.4
	The Third Release of Castle.  Major repairs in 
	the code that allowed you to continue playing 
	even if your character is killed.  Still major 
	problems with the software.

version 1.xx*
	The Second Release of Castle.  Some of the code
	was changed to make it more appealing to the eye.
	A few portions of the game no longer need you to
	type the whole word.

	*Versions were not used by B.M. Software in this
	 release.  But it is between 1.00 and 1.4.

version 1.00
	The Orignal Release of Castle.  Written in the
	QBasic Language.
</pre>
