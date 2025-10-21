# WinCastle: A Text Based Adventure (With Plugins)
Castle (c) 2003 Beem Media. All rights reserved.

This is a 5.XX version of Castle that had plugin support. I had this _great_
idea that Castle would be a base engine and support all kinds of things, so
there would be a music plugin (which was developed), and there would be a
graphics plugin, and so on.

## Building

A visual studio 2022 project is provided to build the binaries for Windows
platforms.

## History

* Originated: 1997
* This Version: Some time in 2003

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
