/*********
assignment 1
April 15, 2021

This program is a memory game of 16 cards, and the player guess the row/col position of two cards that match.
If the cards match, they flip over. Otherwise, the cards are momentarily revealed and flipped back down. Goal is match all cards.
*********/
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>

const int LENGTH = 4;
using namespace std;

// Function prototypes

void InitializeCards(int cards[][LENGTH]);
void SetFaceDown(bool faceup[][LENGTH]);
void ShowCards(int cards[][LENGTH], bool faceup[][LENGTH]);
void DisplayCards(const int cards[][LENGTH]);
void RecordPlayer(string fname, string lname, int numberOfTurns);
bool AllMatching(const bool faceup[][LENGTH]);
bool OutOfRange(int x, int y);
bool CardFound(int x, int y, const bool faceup[][LENGTH]);
bool RepeatedInput(int x1, int y1, int x2, int y2);

// ======================
//     main function
// ======================

int main()
{
    string fname, lname;
    int numberOfTurns = 1; //The game is initially set to turn 1
    int cards[LENGTH][LENGTH];
    bool faceup[LENGTH][LENGTH];

    //Prompts player to enter their first and last name, then greets the player.
    cout << "Enter your first name: ";
    getline(cin, fname);
    cout << "Enter your last name: ";
    getline(cin, lname);
    cout << "\nWelcome to the memory game " << fname << " " << lname << endl << endl;

    InitializeCards(cards); //Cards are initialized and shuffled
    SetFaceDown(faceup); // Cards are initialized to false/face down.

    do
    {
        int x1, x2, y1, y2;

        cout << "Turn number: " << numberOfTurns << endl << endl; //Displays Current Turn

        ShowCards(cards, faceup); //Displays the cards

        cout << "Enter card 1's row and col: ";
        cin >> x1 >> y1;
        if(OutOfRange(x1, y1) || CardFound(x1, y1, faceup)) continue; // If input is not 0-3 or card is already there, then player enters row/col again.

        cout << "Enter card 2's row and col: ";
        cin >> x2 >> y2;
        if(OutOfRange(x2, y2) || CardFound(x2, y2, faceup) || RepeatedInput(x1, y1, x2, y2)) continue;

        cout << "\n";

        if(cards[x1][y1] == cards[x2][y2])
        {
            cout << "Match found!\n\n";
            faceup[x1][y1] = true;  //Both cards are set to true/face up
            faceup[x2][y2] = true;
            ShowCards(cards, faceup);
        }else if(cards[x1][y1] != cards[x2][y2])
        {
            cout << "No match.\n\n";
            faceup[x1][y1] = true;
            faceup[x2][y2] = true;
            ShowCards(cards, faceup); //Reveal cards
            cout << "Flipping cards over again.\n\n";
            faceup[x1][y1] = false;   //When loop begins again, the cards are flipped back down.
            faceup[x2][y2] = false;
        }

        if(AllMatching(faceup)) break; //If all cards match, the program exits out of the loop.

        numberOfTurns++; //Increase turn count

        cout << "Enter 'q' to quit or press any key to continue...\n\n";
        cin.ignore(); //Ignores the /0 in keyboard buffer

    }while(cin.get() != 'q'); //Loop ends when the next character entered is 'q'.

    if(!AllMatching(faceup))
    {
        ofstream playerFile("player.txt");
        if(playerFile)
        {
            playerFile << "Don't give up!\n";
            playerFile.close();
        }else{
            cout << "File could not open!\n";
        }
    }else
    {
        RecordPlayer(fname, lname, numberOfTurns);
        cout << "All cards match! You win the game!\n";
        cout << "It took " << numberOfTurns << " turns.\n\n";
        cout << "Player recorded!\n";
    }

    return 0;

}
// InitializeCards
// Places pairs of numbers in the 2D array and then
// randomly shuffles them.
// ======================
void InitializeCards(int cards[][LENGTH])
{
	srand(time(NULL));
	int x1, y1, x2, y2;
	int i;
	int temp;

	cards[0][0] = 1; cards[0][1] = 1;
	cards[0][2] = 2; cards[0][3] = 2;
	cards[1][0] = 3; cards[1][1] = 3;
	cards[1][2] = 4; cards[1][3] = 4;
	cards[2][0] = 5; cards[2][1] = 5;
	cards[2][2] = 6; cards[2][3] = 6;
	cards[3][0] = 7; cards[3][1] = 7;
	cards[3][2] = 8; cards[3][3] = 8;


	for (i = 0; i < 30; i++) {

		y1 = rand() % LENGTH;
		x1 = rand() % LENGTH;

		y2 = rand() % LENGTH;
		x2 = rand() % LENGTH;

		temp = cards[x1][y1];
		cards[x1][y1] = cards[x2][y2];
		cards[x2][y2] = temp;
	}
}
// ======================
// ShowCards
//Passes in the 2d array that contains the cards and the 2d array that determines if they match
// Generates a display on the screen.  If faceup=false,
// an * is output, otherwise the card in that slot is output.
// ======================
void ShowCards(int cards[][LENGTH], bool faceup[][LENGTH]) {

	int i, j;

    cout << "   0  1  2  3\n";

	for(i = 0; i < LENGTH; i++)
    {
        cout << i << "|";
        for(j = 0; j < LENGTH; j++)
        {
            if(faceup[i][j] == false)
            {
                cout << " " << '*' << " ";
            }else
            {
                cout << " " << cards[i][j] << " ";
            }
        }
        cout << "|\n";
    }
    cout << "\n";
}
// ======================
// SetFaceDown
// Initialize every element in the array to false
// ======================
void SetFaceDown(bool faceup[][LENGTH])
{
    for(int i = 0; i < LENGTH; i++)
        for(int j = 0; j < LENGTH; j++)
            faceup[i][j] = false;
}

// ======================
// AllMatching
// Passes in the 2d array that determines whether each card match, and set to const to prevent changes
// Checks if there is at least one card is face down;
// if so, function returns false. Otherwise, returns true.
// ======================
bool AllMatching(const bool faceup[][LENGTH])
{
    for(int i = 0; i < LENGTH; i++)
    {
        for(int j = 0; j < LENGTH; j++)
        {
            if(faceup[i][j] == false)
            {
                return false;
            }
        }
    }
    return true;
}
// ======================
// OutOfRange
// Passes in the coordinate value the player inputs, and
// checks if the value is within the the range of 0-3 inclusive.
// ======================
bool OutOfRange(int x, int y)
{
    if(x < 0 || x > 3 || y < 0 || y > 3)
    {
        cout <<"\n" << "The row/col number is out of range!\n\n";
        return true;
    }
    return false;

}
// ======================
// CardFound
// The user's row and col input are used as the index for
// the bool array faceup to check if that location is true or false.
// If true, then the card is already found/faced up.
// ======================
bool CardFound(int x, int y, const bool faceup[][LENGTH])
{
    if(faceup[x][y] == true)
    {
        cout << "\n" << "Card already found!\n\n";
        return true;
    }
    return false;
}
// ======================
// RepeatedInput
// Checks if both inputs are the same, and if so, return true.
// ======================
bool RepeatedInput(int x1, int y1, int x2, int y2)
{
    if(x1 == x2 && y1 == y2)
    {
        cout <<"\n" << "You entered the same coordinates!\n\n";
        return true;
    }
    return false;
}
// ======================
// RecordPlayer
// Opens a file called 'player.txt'. If it can open, and if it the player wins,
// then write in it the player's name and number of turns played. If the player didn't win, then a message saying, 'don't give up' is written instead.
// ==============
void RecordPlayer(string fname, string lname, int numberOfTurns)
{
    ofstream playerFile("player.txt"); //Opens the file called 'player.txt'

    if(playerFile)
    {
        playerFile << "Name: " << fname << " " << lname << endl << endl << "Number of turns: " << numberOfTurns;
        playerFile.close(); //closes file
    }else{
        cout << "File could not open!\n\n";
    }
}
/*  Execution Results

Enter your first name: Lawrence
Enter your last name: Lu

Welcome to the memory game Lawrence Lu

Turn number: 1

   0  1  2  3
0| *  *  *  * |
1| *  *  *  * |
2| *  *  *  * |
3| *  *  *  * |

Enter card 1's row and col: 0 3
Enter card 2's row and col: 0 4

The row/col number is out of range!

Turn number: 1

   0  1  2  3
0| *  *  *  * |
1| *  *  *  * |
2| *  *  *  * |
3| *  *  *  * |

Enter card 1's row and col: 0 2
Enter card 2's row and col: 0 0

No match.

   0  1  2  3
0| 5  *  6  * |
1| *  *  *  * |
2| *  *  *  * |
3| *  *  *  * |

Flipping cards over again.

Enter 'q' to quit or press any key to continue...


Turn number: 2

   0  1  2  3
0| *  *  *  * |
1| *  *  *  * |
2| *  *  *  * |
3| *  *  *  * |

Enter card 1's row and col: 0 0
Enter card 2's row and col: 1 3

No match.

   0  1  2  3
0| 5  *  *  * |
1| *  *  *  1 |
2| *  *  *  * |
3| *  *  *  * |

Flipping cards over again.

Enter 'q' to quit or press any key to continue...


Turn number: 3

   0  1  2  3
0| *  *  *  * |
1| *  *  *  * |
2| *  *  *  * |
3| *  *  *  * |

Enter card 1's row and col: 0 0
Enter card 2's row and col: 3 0

No match.

   0  1  2  3
0| 5  *  *  * |
1| *  *  *  * |
2| *  *  *  * |
3| 2  *  *  * |

Flipping cards over again.

Enter 'q' to quit or press any key to continue...

q

Process returned 0 (0x0)   execution time : 36.018 s
Press any key to continue.
*/


