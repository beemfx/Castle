#include <stdio.h> // For the printf() function.
#include <conio.h> // For the clrscr() function, and getch() funtion.
#include <stdlib.h> // For the exit() funtion.

char name[80];
void deathend();
void playgame();

void main(void){
	playgame();
}

void playgame(){
	int c;
	printf("Castle: A Text Based Roleplaying Adventure v2.08\n");
	printf("(C) Copywrite 1997, 2000 B.M. Software\n");
	printf("\nWelcome! I am the Tunnel Master.\n");
	printf("What is your name:");
	gets( name );
	printf("\nHello %s, thats a nice name\n", name);
	while (1){
	printf("Ready to begin? 1)Yes 2)No:");
	c = getch();
		clrscr();
		switch (c){
			case '1':
				printf("You've decided to play\n\n");
				break;
			case '2':
				printf("\nYou have to start some time.\n");
				continue;
			default:
				printf("\nThat was not a choice.\n");
			continue;
		}
	break;
	}
	printf("You head through the main gate of an old Castle.\n");
	printf("Three doors catch your attention.  An old wooden door\n");
	printf("a shiny new gold door, and a relitively ugly brass door.\n");
	printf("Your first instinct is to try to open the wooden door,\n");
	printf("you try to open it, but to no avail, it's locked.\n");

	while (1){
	printf("\nWhich door do you wish to open? 1)Brass 2) Gold:");
	c = getch();
		clrscr();
		switch (c){
			case '1':
				printf("You open the brass door and walk through.\n");
				break;
			case '2':
				printf("You open the gold door and walk through.\n");
				break;
			default:
				printf("That was not a choice.\n");
			continue;
		}
	break;
	}
// The following code is for those who open the brass door.
	if (c == '1'){
		printf("As you enter the room out of the corner of your eye\n");
		printf("you see a large monster.\n");
		while (1){
			printf("\nDo you 1)Fight the monster or 2)Hide from it:");
			c = getch();
			clrscr();
			switch (c){
				case '1':
					printf("You engage in combat. Weaponless you are easily crushed.\n");
					deathend();
				case '2':
					printf("You have chosen to hide.\n");
					break;
				default:
					printf("Wrong choice\n");
					continue;
			}
		break;
		}
		printf("You immediately sense a Box and a Desk in the room.\n");
		while (1){
				printf("\nDo you hide behind the 1)Box or the 2)Desk:");
				c = getch();
				clrscr();
				switch (c){
					case '1':
						printf("You jump behind the box.\n");
						break;
					case '2':
						printf("You scrall under the desk, just not quickly enough.\n");
						printf("The monster breaks the desk into peices and tears\n");
						printf("you in half. Blood rushes all over the floor.\n");
						deathend();
					default:
						printf("Wrong choice\n");
						continue;
				}
			break;
			}
		printf("The monster doesn't seem to see you. Thank Goodness!\n");
		printf("Behind the box is a ventillation system. You decide\n");
		printf("that it would be best to crawl through it.  After a\n");
		printf("couple of minutes you approach an intersection.\n");
		while (1){
			printf("\nWould you like to go 1) Right or 2) Left:");
			c = getch();
			clrscr();
			switch (c){
				case '1':
					printf("You move down the right junction, thinking it's.\n");
					printf("the 'Right' choice.  This time your wrong, very\n");
					printf("wrong.  After three hours you become very thirsty,\n");
					printf("and give up.  Years later your toasted skeleton is\n");
					printf("is found.\n");
					deathend();
				case '2':
					printf("You crawl down the left vent.\n");
					break;
				default:
					printf("Wrong choice\n");
					continue;
			}
		break;
		}
		printf("After a couple of minutes you exit the vent,\n");
		printf("only to see a beautiful princess behing harrassed\n");
		printf("by an overweight monster.\n");
		while (1){
			printf("\nDo you wish to 1) Yell at the monster or 2) fight it:");
			c = getch();
			clrscr();
			switch (c){
				case '1':
					printf("You yell, really loud. The monster doesn't seem\n");
					printf("to be fond of loud yelling.  In an angry rage it\n");
					printf("approaches you, roars, then bites your head off\n");
					printf("as well as the princess'\n");
					deathend();
				case '2':
					printf("You choose to fight the monster. After a brutal\n");
					printf("battle lasting several days, you finally defeat\n");
					printf("the monster.  The princess stares at you with\n");
					printf("passionate eyes. Suddenly an idea pops into your\n");
					printf("head,  !!!Makeout Time!!!. You and the Princess\n");
					printf("begin to kiss.  You keep on kissing.  You love\n");
					printf("each other so much.\n");
					printf("\nYou have succesfully completed\n");
					printf("Castle: A Text Based Roleplaying Adventure\n");
					printf("\n\nThank You For Playing");
					exit(0);
					break;
				default:
					printf("Wrong choice\n");
					continue;
			}
		break;
		}
	}
// The follwoing code is for those who open the gold door.
	else if (c == '2'){
		printf("Upon Entering the room you see two tunnels.\n");
		printf("Both look about the same and you don't want\n");
		printf("to waste time choosing.\n");

			while (1){
				printf("\nWill you go 1)Right or 2)Left:");
				c = getch();
				clrscr();
				switch (c){
						case '1':
							printf("You quickly walk down the right tunnel.\n");
							break;
						case '2':
							printf("Keeping your cool you walk down the left tunnel.\n");
							break;
						default:
							printf("That was not a choice.\n");
						continue;
				}
			break;
			}
	// This is for the right Tunnel.
	if (c == '1'){
		printf("After a moment of traveling you see an Axe.");
		printf("It looks very sharp and dangersous.");
			while (1){
			printf("\nWould you like to 1) Pick it up or 2) Leave it:");
			c = getch();
			clrscr();
			switch (c){
				case '1':
					printf("You grab the Axe then you swing the smooth weapon\n");
					break;
				case '2':
					printf("You don't want to touch the Axe because you think that\n");
					printf("it's too dangerous.  Big mistake.  The Axe must have\n");
					printf("been some kind of trap.  It swings through the air\n");
					printf("chopping you in half, vertically.  Blood flows onto\n");
					printf("the floor.\n");
					deathend();
				default:
					printf("Wrong choice\n");
					continue;
			}
		break;
		}
	printf("back and forth through the air.  You are enjoying\n");
	printf("it very much.  You tie it to your waist with a rope\n");
	printf("you find, and move on.\n");
	printf("\nSoon after you approach a shaft. A fairly sturdy\n");
	printf("rope extends downward, and a rikety ladder climbs\n");
	printf("upward.");
			while (1){
				printf("\nDo you want to 1)Climb down the rope or 2) up the ladder:");
				c = getch();
				clrscr();
				switch (c){
					case '1':
						printf("You grab onto the rope and begin your descent.\n");
						printf("After a short time you figure that you aren't\n");
						printf("making any progress so you begin to climb back\n");
						printf("up.  The rope must not have been as strong as you\n");
						printf("thought, as it soon snaps and you fall to your\n");
						printf("death.\n");
						deathend();
					case '2':
						printf("The latter shakes violently as you climb up it.\n");
						break;
					default:
						printf("Wrong choice\n");
						continue;
				}
			break;
		}
	printf("Once you reach to top of the shaft you\n");
	printf("climb out onto the edge, only to find\n");
	printf("the most ugly monster you have ever seen.\n");
	printf("You remember the Axe you collected and decide\n");
	printf("that it's time to make a decision.\n");
			while (1){
				printf("\nDo you want to 1)Attack the monster or do you\n");
				printf("wish to 2)Think of something else to do:");
				c = getch();
				clrscr();
				switch (c){
					case '1':
						printf("You engage in combat with the monster.\n");
						break;
					case '2':
						printf("You decide to think of something to do beside\n");
						printf("slaughter an innocent monster.  Not such a good\n");
						printf("idea.  While you are concentrating you hardly notice\n");
						printf("that you arms and legs were just ripped off. The\n");
						printf("monster leaves you limbless. To Die.\n");
						deathend();
					default:
						printf("Wrong choice\n");
						continue;
				}
			break;
			}
	printf("The battles doesn't last long as you have\n");
	printf("a weapon and it doesn't.  You easily hack\n");
	printf("the beast into a thouand peaces.  It takes\n");
	printf("you quite a while.\n");
	printf("\nAfter the slaughter you dash down a hallway.\n");
	printf("After a few hours of running you approach a\n");
	printf("floating barricade.  There is just enough room\n");
	printf("to go under or over it.");
			while (1){
				printf("\nDo you want to 1) Craw under or 2)Jump over:");
				c = getch();
				clrscr();
				switch (c){
					case '1':
						printf("You crawl under it. Not your best choice. The\n");
						printf("barricade falls downs smashing you.  Only your\n");
						printf("blood can be seen seeping out from under it.\n");
						deathend();
					case '2':
						printf("You hop over the barricade and land on the other\n");
						break;
					default:
						printf("Wrong choice\n");
						continue;
				}
			break;
			}
	printf("side.  You soon exit the Castle.\n");
	printf("\nUpon exiting the Castle you meet a funny\n");
	printf("looking Dwarf.  A thought pops into your mind\n");
			while (1){
				printf("\nYou can 1)Kill the innocent creature or\n");
				printf("2)Let it go:");
				c = getch();
				clrscr();
				switch (c){
					case '1':
						printf("It's obvious that you are sick.  However\n");
						printf("compassion overwhelms your heart and you change\n");
						printf("your mind about slaughtering it.\n");
						break;
					case '2':
						printf("Your seem to be a desent fellow, sparring the poor\n");
						printf("creatures life and all.\n");
					default:
						printf("Wrong choice\n");
						continue;
				}
			break;
			}
	printf("Well after the encounter with the dwarf you\n");
	printf("feel pretty good about yourself.  You decide\n");
	printf("to go to the local arcade.  Two games engage\n");
	printf("your interest, but you can only play one of them.\n");
			while (1){
				printf("\nDo you wish to play 1)Duke Nukem or 2)Rashers:");
				c = getch();
				clrscr();
				switch (c){
					case '1':
						printf("You walk over to the Duke Nukem Machine. After\n");
						printf("enjoying it for a couple of minutes the machine\n");
						printf("explodes.  You body is blown into chunks of meat\n");
						printf("and blood.\n");
						deathend();
					case '2':
						printf("You pop a quarter into the Rashers machinge.  You\n");
						printf("really enjoy playing the game.  Your mind is\n");
						printf("consumed with playing video games.  You play\n");
						printf("video games for the rest of your life.\n");
						printf("\nYou have succesfully completed\n");
						printf("Castle: A Text Based Roleplaying Adventure\n");
						printf("\n\nThank You For Playing");
						exit(0);
						break;
					default:
						printf("Wrong choice\n");
						continue;
				}
			break;
			}
	}
	// This is for the left Tunnel.
	else if (c == '2'){
	printf("After a while you exit the tunnel. You\n");
		printf("realize that you were just traveling through\n");
		printf("a sewar.  Discusted you find a lake and bathe\n");
		printf("nude. You notice a babe standing by the lake.\n");
		printf("Embarrased you run to your clothes and put them\n");
		printf("on. The babe walks up to you and asks if you will\n");
		printf("kiss her.\n");
				while (1){
				printf("\nYou can 1) Go strait for the kiss or 2)Stay cool:");
				c = getch();
				clrscr();
				switch (c){
					case '1':
						printf("Hey whats to lose.  You hold her in your arms\n");
						printf("and start to kiss.  Too bad she was really a\n");
						printf("a monster that wanted to eat you.  She cleans\n");
						printf("her teeth out with your bones.\n");
						deathend();
					case '2':
						printf("Staying cool you decide not to kiss anyone that\n");
						break;
					default:
						printf("Wrong choice\n");
						continue;
				}
			break;
			}
	printf("you don't know that well.  She is kind\n");
	printf("of dissapointed, but respects your decission.\n");
	printf("She tells you that she was in trouble, but\n");
	printf("you've rescued her.  She continues to ask if\n");
	printf("you will marry her.\n");
				while (1){
				printf("\nWill you 1)Marry her or 2)Leave her:");
				c = getch();
				clrscr();
				switch (c){
					case '1':
						printf("Your wedding is the next day.  You live the rest\n");
						printf("of your lives together.  Happily ever after.\n");
						printf("\nYou have succesfully completed\n");
						printf("Castle: A Text Based Roleplaying Adventure\n");
						printf("\n\nThank You For Playing");
						exit(0);
						break;
					case '2':
						printf("Special sad ending:\n\n");
						printf("You decide that you don't want to make any long\n");
						printf("term commitement, and sadly reject her.  The two\n");
						printf("of you walk off into the sunset. . .\n");
						printf("\nBut that's not the end.  You come accross a road\n");
						printf("while traveling down it. You come to an intersection.\n");
						printf("One way leads to the country the other to the city.\n");
						printf("The babe goes to the country and you go to the city.\n");
						printf("The two of you never see each other again.");
						printf("\nYou have succesfully completed\n");
						printf("Castle: A Text Based Roleplaying Adventure\n");
						printf("\n\nThank You For Playing");
						exit(0);
						break;
					default:
						printf("Wrong choice\n");
						continue;
				}
			break;
			}
	}
	// Once again nothing
	else
		printf("How  did you get this message.");

	}
// The following is nothing
	else
		printf("How  did you get to this message.");
return 0;
}

void deathend(){
	int c;
	while (1){
		printf("\nYou have died %s. Do you wish to play again 1)Yes 2)No:", name);
		c = getch();
		clrscr();
		switch(c){
			case '1':
				clrscr();
				playgame();
			case '2':
				clrscr();
				printf("Good bye!\n");
				exit(0);
				break;
			default:
				printf("Wrong choice");
				continue;
		}
	}
}
