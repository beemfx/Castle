Castle: A Text Based Adventure v6.xx
====================================
Castle BUILD 6 (c) 1997-2015 Beem Media

Castle will read a Text Based Adventure file(usually with extension .tba) and 
run it.

Instructions:  To play press the number of your choice when prompted.  Use the 
game|restart menu command to restart your current game.  To play a custom map 
use game|custom... and select a valid .tba file.

Credits
-------
Lead Programmer   : Blaine Myers
Written by        : Jack Everett
Technical Adviser : Bil Myers
Special Thanks    : Craig Decker

© 2015 Beem Media.  Castle, Castle BUILD, CastleSCRIPT, WinCastle and their 
respective logos are trademarks or registered trademarks of Beem Media in the 
U.S. and/or other countries.  Beem Software is a division of Beem Media.  All 
other trademarks belong to their respective owners.

Programming .tba files:
-----------------------
There is currently no documentation available for .tba construction.  I might
write one someday.

Technical Stuff:
----------------

Castle BUILD:

The Castle BUILD is the engine used to run castle games, here is some 
information about all the BUILDs:

BUILD 1: The original QuickBasic build of Castle, a fairly crude, very outdated 
	text adventure.  Had extensive use of the obsolete GOTO statement.  BUILD 1 
	is any 1.xx version of Castle.

BUILD 2: The second build of castle.  Written in the C language it had a much 
	better interface.  The user was no longer required to type in commands.  Only
	hit a single keystroke.  Story was somewhat rewritten, lots of spelling
	mistakes corrected.  Build 2 is any 2.xx version of castle.

BUILD 3: Castle finally got a script to run, allowing custom text adventures.
	The script was simple and worked very effectively with four different
	functions. Build 3 is versions 3.xx.

BUILD 4: The Main program was renamed WinCastle instead of just Castle.  Now
	Castle is in a Win32 API supporting win 9x and probably ME.  Uses the same
	script as BUILD 3 and most parts of the game functions are the same.
	Advantages are that it uses menus to control the special features in the
	game.  BUILD 4 is version 4.xx of Castle.

BUILD 5: This newest build looks exactly the same as build 4, but the
	CastleSCRIPT has been drastically updated to support better features.  BUILD
	5 is any version 5.xx of Castle.
	
BUILD 6: The script format has been changed some. Mainly the CHOICE function is
	can now associate text with a choice. This way the game wrapper can hook up
	choices however it sees fit, whether that be by UI buttons or keyboard input.
	The game processor has also been improved. "WinCastle" has been retitled back
	to "Castle". The PRINT command now requires explicit linebreaks using \n.
	The game also better handles script errors. Scripts are also processing using
	EGParse which is pretty robust for processing statements.
	
	As for the game wrapper interface, it has been updated. The font is bigger
	and the game is overall more presentable.