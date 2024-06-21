#include <iostream>
using namespace std;
char board[3][3] = { {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'} };
bool player1Turn = true;
void displayBoard() {
    cout << "-------------" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "| ";
        for (int j = 0; j < 3; j++) {
            cout << board[i][j] << " | ";
        }
        cout << endl << "-------------" << endl;
    }
}
bool gameOver() 
{
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return true; 
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return true; 
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return true; 
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return true; 
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] != 'X' && board[i][j] != 'O') return false;
        }
    }
    return true; 
}
void playerMove()
{
    int choice;
    char mark;
    if (player1Turn) mark = 'X';
    else mark = 'O';
    cout << "Player " << (player1Turn ? "1" : "2") << ", enter your choice: ";
    cin >> choice;
    while (choice < 1 || choice > 9 || board[(choice - 1) / 3][(choice - 1) % 3] == 'X' || board[(choice - 1) / 3][(choice - 1) % 3] == 'O') 
    {
        cout << "Invalid move. Choose an empty position (1-9): ";
        cin >> choice;
    }

    board[(choice - 1) / 3][(choice - 1) % 3] = mark;
    player1Turn = !player1Turn;
}
int main()
{
    int moves = 0;
    displayBoard();

    while (!gameOver()) {
        playerMove();
        displayBoard();
        moves++;
        if (gameOver())
        {
            if (moves == 9) {
                cout << "It's a tie!" << endl;
                break;
            }
            cout << "Player " << (player1Turn ? "2" : "1") << " wins!" << endl;
            break;
        }
    }
    return 0;
}
