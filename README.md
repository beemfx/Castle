# Castle: A Text Based Adventure v6.xx
(c) 1997-2015, 2025 Beem Media. All rights reserved.

The game Castle, the first game developed by Beem Software. See the various
branches for a history of the game going all the way back to the original QBasic
source code from 1997.

It's actually kind of cool in that it has it's own scripting language called
CastleSCRIPT so technically you can create any kind of text adventure that you
want using this application.

The current version has been through quite a bit, but now I pretty much consider
this software complete other than the occasional maintenance to bring it up to
C++ standards.

## History
Origin: 1997

### Original Version.txt
<pre>
=========================================================
=== Version History of                                ===
===        WinCastle: A Text Based Adventure          ===
=========================================================
version 6.XX (October 1, 2015)
	There have been many major changes to how scripts are
	processed, but the script themselves have also changed
	namely the CHOICE statement now uses the format
	CHOICE( "Choice Text 1" , GOTOLABEL1 , "Choice Text 2" , GOTOLABLE2 );
	That way the wrapper for the game can handle choices
	however it wants, whether it be by keyboard, or by
	UI buttons.
	
version 5.50 (December 09, 2006)
	A lot of internal stuff was redone.  Plugin 
	support was removed.  The edit menu was removed.
	Better handling of invalid maps, blank screen
	instead of a crash in most cases.  Most of what
	has been done was to make it easy to create
	an online version of the game.

version 5.21B (November 28, 2003)
	Now using improved data streaming library.

version 5.20B (June 28, 2003)
	WinCastle now uses data streaming, rather than
	file streaming.  So the adventures will now run
	significantly faster.

version 5.11B (June 27, 2003)
	Removed Help File as it was redicuously stupid.
	Also program has compilation optimizations.

version 5.10b (January 24, 2003)
	Goto Statement now works more like c language
	Plugin support is also available, but I am still
	attempting to improve on it.

version 5.01b (January 17, 2003)
	Finally fixed the problem where the icon on the
	about dialog wouldn't show up on Windows NT.  So
	it shows up now.

version 5.00b (June 20, 2002)
	I completely redid CastleSCRIPT to look more like
	c++.  It also compiles more like c++;  The game
	itself looks exactly the same.  This is actually
	a beta release, but for now it is the official
	release.

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
	a class called CastleGame which contains all the
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

### Original Readme.txt
<pre>
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
</pre>

### Original ReadMeDev.txt
<pre>
ReadMeDev - Readme file pertaining to the source code for WinCastle

Here is the old Log:

June 20, 2002

I've completly redone CastleSCRIPT, the game looks exactly the same,
however the new CastleSCRIPT has much greater functionality. (ie.  There may
now be up to nine choices, and the script will better support adding new functions.)
Right now the script looks a lot like c++, one thing I want to do is change the way
GOTO(x) works, so that it looks for labels.

/*
	This program will read a castle
	map and run it.

	Recorded Log (see version.txt for more history):

	November 27, 2001:

	I am now compiling under Microsoft Visual C++
	The code is working fine except that the dialog boxes are not appearing
	correctly.

	Now everything is perfectly fine.
	  
	September 20, 2001:

	I converted the code to C++ by implementing the CastleGame class which
	plays the game of castle.  This way I beleive it will be easier to convert
	it to a Windows API.

	Notice: WinCastle source code should be work with any C++ compiler with
	a Windows API SDK of 3.1 or higher.  It was written with Borland C++ 4.5x
	and the project file should be compatible with most Borland IDE's.
	For microsoft, you'll have to put together the makefile yourself.

	October 3, 2001

	I've introduced the windows API.
	Right now all the functions execpt makeChoice() are working.  To implement
	makeChoice I need to learn how to scan from the keyboard in a Win API.

	The functions that print out to the screen print the entire line, not just
	what should be printed.  It won't be too hard to fix that.(Fixed)

	I also need to implement the ability to open a custom map, and restart the
	current map.  I doubt either will be too hard.

	I'd also like to add a help, about menu item.  Maybe even a game help item.

	October 4, 2001

	I've added the keyboard support.  After working out a few bugs I'm glad to
	quote that kid in star wars that said: "It's working.  It's woooooorking!"

	Now only one problem exists.  The game does not refresh the screen when a
	a button is pressed.  This is very strange indeed.

	Used RedrawWindow to update the playing screen with the tags RDW_ERASE and
	RDW_INVALIDATE it works fine.

	The GOT function of CastleScript is my only problem.  Already fixed it, the
	buff array for seeker was the wrong size.  Now all is so fucking fine.  I
	am one happy bastard.  WinCastle really kicks ass.  Honestly it doesn't
	kick ass so much as makes me happy that it actually works.

	All that is left is to implement the restart game and custom game menu
	commands.  True it will be much difficult but I'll get it done.  Tell you the
	truth it shouldn't be that hard.  I won't have to learn much to figure it out
	though.

	After that I think I'll want to add a help menu with an about option.  Maybe
	even a help file if I'm willing to learn how to create one.

	On a personal note I should be receiving my amazon.com order tomorrow which
	should include the classic hit Stand By Me.

	October 6, 2001
	Implemented Restart Game after much difficulty.

	Implemented Custom Game, had to create a constructor and destructor to
	initialize a game.  One thing I'm not satisfied with is that if you use
	the custom game commnad, the program will give an error when terminated.

	October 30, 2001 added the about dialog box.  It doesn't appear if the IDE
	isn't running and I can't figure out why.  Made a definition to filter out
	the dialob.

	June 02, 2002 Now works under WindowsXP just freaking fine
*/
</pre>