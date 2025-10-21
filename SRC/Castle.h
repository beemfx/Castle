/*
	castle.h - Hearder file for Castle: A Text Based Adventure
*/

#define NEWLINE 2
#define PR (line[0] == 'P' && line[1] == 'R')
#define CLS (line[0] == 'C' && line[1] == 'L' && line[2] == 'S')
#define CH (line[0] == 'C' && line[1] == 'H')
#define END (line[0] == 'E' && line[1] == 'N' && line[2] == 'D')
#define GOT (line[0] == 'G' && line[1] == 'O' && line[2] == 'T')

class CastleGame
{
	public:
		int playGame(char *filename);           //Starts up a game of Castle
	private:
		void gameOver(FILE *fin);               //Terminates a game of Castle
		void checkStats(FILE *fin);             //Checks the version of Castle map
		void printMsg(char *line);              //prints a message out on the screen
		void getLine(FILE *fin);                //reads a line from file input
		void makeChoice(FILE *fin, char *line); //allows the user to make a choice and displays one line
		void gotoF(char *line, FILE *fin);      //goes to a particual part of the map
		int fseeker(char search[2], FILE *fin); //seeks for a certain part of the map
		void seeker(char *sstring, FILE *fin);  //part of fseeker
		enum {LINE_LEN = 80, GOT_LEN = 20};     //just some variables
		enum {FSUCCESS, FFAIL};                  //more enumerated variables
};
