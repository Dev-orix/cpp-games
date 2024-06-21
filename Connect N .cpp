#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MaxRows = 14;
const int MaxCols = 14;
const int MaxPlayers = 2;
const char DefaultFill = '.';

// Arrays to store data
char Board[MaxRows][MaxCols];

// Array to keep track of next row in each column
int Next_Move[MaxCols] = {0};

string PlayerNames[MaxPlayers];
char PlayerMarks[MaxPlayers] = {'o', 'x'};

// Turning player represents the player making the move
int TurningPlayer = 0;

// Game End State represents the result of the game
// -1 means game is not over, 0 means the first player has won,
// and 1 means the second player is the winner
int GameEndState = -1;

// Number of rows to be used in this game
int Rows = 6;

// Number of columns to be used in this game
int Columns = 7;

// How many consecutive positions to win
int ConnectLimit = 4;

int Menu() {
    int Choice;
    do {
        cout << "1. Play" << endl
             << "2. Save Game" << endl
             << "3. Load Game" << endl
             << "4. Exit" << endl
             << "Enter Your Choice (1 - 4): ";
        cin >> Choice;
    } while (Choice < 1 || Choice > 4);
    return Choice;
}

bool SaveGame(const string &FileName) {
    ofstream outfile(FileName, ios::out);
    if (!outfile) {
        cerr << "Error opening file for writing." << endl;
        return false;
    }

    // Save Rows, Columns, ConnectLimit
    outfile << Rows << " " << Columns << " " << ConnectLimit << endl;

    // Save PlayerNames
    outfile << PlayerNames[0] << " " << PlayerNames[1] << endl;

    // Save Board
    for (int r = 0; r < Rows; r++) {
        for (int c = 0; c < Columns; c++) {
            outfile << Board[r][c];
        }
        outfile << endl;
    }

    // Save Next_Move array
    for (int c = 0; c < Columns; c++) {
        outfile << Next_Move[c] << " ";
    }
    outfile << endl;

    // Save TurningPlayer and GameEndState
    outfile << TurningPlayer << " " << GameEndState << endl;

    // Save PlayerMarks
    outfile << PlayerMarks[0] << " " << PlayerMarks[1] << endl;

    outfile.close();
    return true;
}

bool LoadSavedGame(const std::string &FileName) {
    ifstream infile(FileName, ios::in);
    if (!infile) {
        cerr << "Error opening file for reading." << endl;
        return false;
    }

    // Load Rows, Columns, ConnectLimit
    infile >> Rows >> Columns >> ConnectLimit;

    // Load PlayerNames
    infile >> PlayerNames[0] >> PlayerNames[1];

    // Load Board
    for (int r = 0; r < Rows; r++) {
        for (int c = 0; c < Columns; c++) {
            infile >> Board[r][c];
        }
    }

    // Load Next_Move array
    for (int c = 0; c < Columns; c++) {
        infile >> Next_Move[c];
    }

    // Load TurningPlayer and GameEndState
    infile >> TurningPlayer >> GameEndState;

    // Load PlayerMarks
    infile >> PlayerMarks[0] >> PlayerMarks[1];

    infile.close();
    return true;
}

void initNewGame() {
    cout << "How Many Rows? ";
    cin >> Rows;
    cout << "How Many Cols? ";
    cin >> Columns;
    cout << "Connectivity Limit? ";
    cin >> ConnectLimit;
    cout << "First Player Name? ";
    cin >> PlayerNames[0];
    cout << "Second Player Name? ";
    cin >> PlayerNames[1];

    // Reset the Next_Move array to all zeros
    for (int i = 0; i < MaxCols; i++)
        Next_Move[i] = 0;

    // Initialize the Board with DefaultFill
    for (int r = 0; r < MaxRows; r++)
        for (int c = 0; c < MaxCols; c++)
            Board[r][c] = DefaultFill;
}

bool CheckWin(int Row, int Col) {
    char mark = PlayerMarks[TurningPlayer];

    // Check horizontally
    int count = 0;
    for (int c = 0; c < Columns; c++) {
        if (Board[Row][c] == mark) {
            count++;
            if (count == ConnectLimit) return true;
        } else {
            count = 0;
        }
    }

    // Check vertically
    count = 0;
    for (int r = 0; r < Rows; r++) {
        if (Board[r][Col] == mark) {
            count++;
            if (count == ConnectLimit) return true;
        } else {
            count = 0;
        }
    }

    // Check diagonally (top-left to bottom-right)
    count = 0;
    for (int r = Row, c = Col; r >= 0 && c >= 0; r--, c--) {
        if (Board[r][c] == mark) {
            count++;
            if (count == ConnectLimit) return true;
        } else {
            break;
        }
    }
    for (int r = Row + 1, c = Col + 1; r < Rows && c < Columns; r++, c++) {
        if (Board[r][c] == mark) {
            count++;
            if (count == ConnectLimit) return true;
        } else {
            break;
        }
    }

    // Check diagonally (bottom-left to top-right)
    count = 0;
    for (int r = Row, c = Col; r < Rows && c >= 0; r++, c--) {
        if (Board[r][c] == mark) {
            count++;
            if (count == ConnectLimit) return true;
        } else {
            break;
        }
    }
    for (int r = Row - 1, c = Col + 1; r >= 0 && c < Columns; r--, c++) {
        if (Board[r][c] == mark) {
            count++;
            if (count == ConnectLimit) return true;
        } else {
            break;
        }
    }

    return false;
}

bool CheckDraw() {
    for (int c = 0; c < Columns; c++) {
        if (Next_Move[c] < Rows) {
            return false; // Game is not a draw
        }
    }
    return true; // All columns are full, game is draw
}

bool GameisOn() {
    return GameEndState == -1;
}

bool MakeMove(int Col) {
    int R = Next_Move[Col];
    if (R >= Rows)
        return false;
    Board[R][Col] = PlayerMarks[TurningPlayer];
    Next_Move[Col]++;
    if (CheckWin(R, Col)) {
        GameEndState = TurningPlayer;
    } else if (CheckDraw()) {
        GameEndState = 2; // Draw
    } else {
        TurningPlayer = (TurningPlayer + 1) % MaxPlayers;
    }
    return true;
}

void ShowWinner() {
    cout << "\n\n\t*************************" << endl;
    cout << "\t*                       *" << endl;
    cout << "\t*  Congratulations!     *" << endl;
    cout << "\t*  Player " << PlayerNames[GameEndState] << " wins!  *" << endl;
    cout << "\t*                       *" << endl;
    cout << "\t*************************" << endl << endl;
}

void ShowGame() {
    for (int c = 0; c < Columns; c++)
        cout << "_________";
    cout << endl;
    for (int c = 0; c < Columns; c++)
        cout << "\t" << c;
    cout << endl;
    for (int c = 0; c < Columns; c++)
        cout << "_________";

    cout << endl;

    for (int r = Rows - 1; r >= 0; r--) {
        cout << "| " << r << " |\t";
        for (int c = 0; c < Columns; c++)
            cout << Board[r][c] << "\t";
        cout << endl
             << "|   |" << endl;
    }

    for (int c = 0; c < Columns; c++)
        cout << "_________";
    cout << endl;


    if (GameisOn()) {
        cout << endl << "\tPlayer: " << PlayerNames[TurningPlayer]
             << " (" << PlayerMarks[TurningPlayer] << ")"
             << " Moving " << endl;
    } else if (GameEndState == 2) {
        cout << "\n\tIt's a draw! No one wins." << endl;
    } else {
        ShowWinner();
    }
}

void Play() {
    int Move;
    do {
        ShowGame();
        cout << "\tENTER NEGATIVE NUMBER FOR THE MAIN MENU" << endl;
        cout << "\tPlease Enter Your Move ? (0 - " << Columns << "): ";
        cin >> Move;
        if (Move < 0)
            break;
        MakeMove(Move);
    } while (GameisOn());
}

int main() {
    int Choice;
    bool GameNotLoaded = true;

    for (int i = 0; i < MaxCols; i++)
        Next_Move[i] = 0;

    do {
        Choice = Menu();
        switch (Choice) {
        case 1:
            if (GameNotLoaded) {
                initNewGame();
                GameNotLoaded = false;
            }
            Play();
            break;
        case 2:
            {
                string FileName;
                cout << "Enter the filename to save the game: ";
                cin >> FileName;
                if (SaveGame(FileName)) {
                    cout << "Game saved successfully." << endl;
                } else {
                    cout << "Error saving the game." << endl;
                }
            }
            break;
        case 3:
            {
                string FileName;
                cout << "Enter the filename to load the game: ";
                cin >> FileName;
                if (LoadSavedGame(FileName)) {
                    GameNotLoaded = false;
                    cout << "Game loaded successfully." << endl;
                } else {
                    cout << "Error loading the game." << endl;
                }
            }
            break;
        }
    } while (Choice != 4);
    return 0;
}

//Created with <3 by Ali Saleem & Haroon Osman.
