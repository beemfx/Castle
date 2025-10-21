/*
	castle.h - Hearder file for Castle: A Text Based Adventure
*/

#define NEWLINE 2
#define PR (line[0] == 'P' && line[1] == 'R')
#define CLS (line[0] == 'C' && line[1] == 'L' && line[2] == 'S')
#define CH (line[0] == 'C' && line[1] == 'H')
#define END (line[0] == 'E' && line[1] == 'N' && line[2] == 'D')
#define GOT (line[0] == 'G' && line[1] == 'O' && line[2] == 'T')

typedef class CastleGame
{
	public:
		CastleGame() { }
		CastleGame(const char *initialmap){
			strcpy(mapfilename, initialmap);
		};
		~CastleGame(){};
		int initGame();           //Starts up a game of Castle
		int getLine();                //reads a line from file input
		void makeChoice(int choice); //allows the user to make a choice and displays one line
		void restartGame();
		char outmessage[100];
	private:
		void checkStats();             //Checks the version of Castle map
		enum {LINE_LEN = 200, GOT_LEN = 20};     //just some variables
		int fseeker(char search[2]); //seeks for a certain part of the map
		void seeker(char *sstring);  //part of fseeker
		enum {FSUCCESS, FFAIL};                  //more enumerated variables
		FILE *fptr;
		char gotowhere[GOT_LEN];
		void gotoF(char *line);      //goes to a particual part of the map
		void printMsg(const char *line);              //prints a message out on the screen
		void gameOver();               //Terminates a game of Castle
	public:
		int linetoprinton;
		int playingstatus;
		int mapVersion;
		int mapEdition;
		char map_name[20];
		char mapfilename[32000];
} CASTLEGAME;
