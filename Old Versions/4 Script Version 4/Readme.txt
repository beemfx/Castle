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