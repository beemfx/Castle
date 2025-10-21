# Castle: A Text Based Adventure v3.xx
Castle (c) 2000 Beem Media. All rights reserved.

This is the C++ version of Castle for command prompt.

## Building

A visual studio 2022 project is provided to build the binary for Windows
platforms. The code was updated to make that possible. So it is technically
version 3.03. Non-windows platforms will require modifications since the clear
screen is dependen on Windows.

## History

Originated: 1997
This Version: December 31, 2000

### Original README.TXT
Castle: A Text Based Adventure v3.xx
Copyright (c) 1997, 2000 Beem Software

This is the dos version of Castle.  It uses the same script as WinCastle
(CastleSCRIPT 3), but functions in a dos environment.

### Original VERSION.TXT
<pre>
=========================================================
=== Version History of                                ===
===        Castle: A Text Based Adventure             ===
=========================================================

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

version 1.xx
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
