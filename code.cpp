#include <iostream>
#include <cstring>
#include <ctime>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;

void SaveData();
bool LoadData();
void print_board(char[][8]);
void enter_move(char[]);
int move_to_index(char[], int[]);
int white_pawn(char[][8], int[]);
int black_pawn(char[][8], int[]);
int white_rook(char[][8], int[]);
int black_rook(char[][8], int[]);
int black_knight(char[][8], int[]);
int white_knight(char[][8], int[]);
int black_bishop(char[][8], int[]);
int white_bishop(char[][8], int[]);
int black_queen(char[][8], int[]);
int white_queen(char[][8], int[]);
int black_king(char[][8], int[]);
int white_king(char[][8], int[]);
bool is_white_king_under_check(char[][8], int[]);
bool is_black_king_under_check(char[][8], int[]);
bool castling(char[][8], char[], int);
bool check_to_white_king(char[][8]);
bool check_to_black_king(char[][8]);



int last_move[4];
string savemoves[10000];
int stringsize = 0;
int delay1 = 0, delay2 = 0;
int time_format;

int main()
{

	int option;
y:	cout << "1.New Game                2.Load Game" << endl;
b:	cin >> option;
	int stringsize2 = 0;
	if (option == 2) {
		if (!LoadData()) {
			cout << "no game present at this slot" << endl;
			goto y;
		}
		stringsize2 = stringsize;


	}
	else if (option == 1) {
		cin.ignore();
		cout << "Enter the time format: " << endl;
		cout << "1.Classic    2.Rapid	3.Blitz" << endl;
	t:		cin >> time_format;
		if (time_format > 3 || time_format < 1) {
			cout << "Invalid input" << endl;
			goto t;

		}
		if (time_format == 1) {
			delay1 = 0;
			delay2 = 0;
			delay1 *= CLOCKS_PER_SEC;
			delay2 *= CLOCKS_PER_SEC;
		}
		else if (time_format == 2) {
			delay1 = 600, delay2 = 600;
			//10 minutes for each player
			delay1 *= CLOCKS_PER_SEC;

			delay2 *= CLOCKS_PER_SEC;
		}
		else if (time_format == 3)
		{
			delay1 = 300, delay2 = 300;
			//10 minutes for each player
			delay1 *= CLOCKS_PER_SEC;

			delay2 *= CLOCKS_PER_SEC;
		}
		system("CLS");
	}
	else {
		cout << "Enter 1 or 2" << endl;
		goto b;
	}


	char board[8][8] = { {'r','n','b','q','k','b','n','r'},
						{'p','p','p','p','p','p','p','p'},
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{'P','P','P','P','P','P','P','P'},
						{'R','N','B','Q','K','B','N','R'} };

	string game_end = "false";
	int player = 0;
	int result = 0;
	int save_game_move = 0;
	int time1 = 0, time2 = 0;
	cin.ignore();
	char move[6];
	do
	{   
		print_board(board);  //print board
		

		time1 = delay1 / 1000;
		time2 = delay2 / 1000;
		cout << "player 1 time : " << endl;
		cout << "minutes: " << time1 / 60 << endl;
		cout << "seconds: " << time1 % 60 << endl;

		cout << "player 2 time : " << endl;
		cout << "minutes: " << time2 / 60 << endl;
		cout << "seconds: " << time2 % 60 << endl;

		int move_index[4];
	
		if (player % 2 == 0) {
			// white player

			if (save_game_move < stringsize2) {
				Sleep(2000);
				for (int i = 0; i < savemoves[save_game_move].length(); i++) {
					move[i] = savemoves[save_game_move][i];
				}
				save_game_move++;
				result = 1;
			}

			clock_t now = clock();

			cout << "\nplayer White\n";
			if (result == 0) {


		A:			enter_move(move);
				if (time_format == 2) {
					delay1 -= clock() - now;
					if (delay1 < 0) {
						cout << "Player 1 losses" << endl;
						break;
					}
					delay1 += 5000;
				}
				else if (time_format == 3)
				{
					delay1 -= clock() - now;
					if (delay1 < 0) {
						cout << "Player 1 losses" << endl;
						break;
					}
				}
				else if (time_format == 1) {
					delay1 += clock() - now;
				}
			}
			//so next time it will ask for move
			result = 0;

			char exit[] = "exit";
			if (!strcmp(exit, move)) {
				SaveData();
				return 0;
			}

			int valid = 0;
			valid = move_to_index(move, move_index);
			if (valid == 1) { cout << "Invalid move!\n"; goto A; }
			else if (valid == 9) {
				if (castling(board, move, player) == 1) { cout << "castling not possible\n"; goto A; }
				else { goto Q; }
			}

			if (board[move_index[0]][move_index[1]] == 'P')
			{
				valid = white_pawn(board, move_index);
			}
			else if (board[move_index[0]][move_index[1]] == 'R')
			{
				valid = white_rook(board, move_index);
			}
			else if (board[move_index[0]][move_index[1]] == 'N')
			{
				valid = white_knight(board, move_index);
			}
			else if (board[move_index[0]][move_index[1]] == 'B')
			{
				valid = white_bishop(board, move_index);
			}
			else if (board[move_index[0]][move_index[1]] == 'Q')
			{
				valid = white_queen(board, move_index);
			}
			else if (board[move_index[0]][move_index[1]] == 'K')
			{
				valid = white_king(board, move_index);
			}
			else { cout << "Invalid move!\n"; goto A; }
			if (valid == 1) { cout << "Invalid move!\n"; goto A; }
			else if (valid == 8) {
				cout << "\n\n Black Wins!!\n\n";
				game_end == "true";
				break;
			}

			savemoves[stringsize] = move;
			stringsize++;
			//stale mate
			if (check_to_black_king(board) == 1)
			{
				if (check_to_black_king(board) == 1)
				{
					cout << "Check Mate\nWhite Wins";
					game_end == "true";
					break;
				}
				cout << "\n\nGAME DRAW!!";
				game_end == "true";
				break;
			}
		}
		else {
			// Black Player


			if (save_game_move < stringsize2) {
				Sleep(2000);
				for (int i = 0; i < savemoves[save_game_move].length(); i++) {
					move[i] = savemoves[save_game_move][i];
				}
				save_game_move++;
				result = 1;
			}
			clock_t now = clock();
			cout << endl;
			cout << "player Black\n";
			if (result == 0) {


			B:				enter_move(move);
				if (time_format == 2) {
					delay2 -= clock() - now;
					if (delay2 < 0) {
						cout << "Player 2 losses" << endl;
						break;
					}
					delay2 += 5000;
				}
				else if (time_format == 3)
				{
					delay2 -= clock() - now;
					if (delay2 < 0) {
						cout << "Player 2 losses" << endl;
						break;
					}
				}
				else if (time_format == 1) {
					delay2 += clock() - now;
				}
			}
			//so next time it will take input
			result = 0;

			char exit[] = "exit";
			if (!strcmp(exit, move)) {
				SaveData();
				return 0;
			}
			/*else if (!strcmp("draw", move))
			{

				cout << "black player wants to handshake\nto accept enter yes else no!\n";

			}*/
			int valid = 0;
			valid = move_to_index(move, move_index);
			if (valid == 1) { cout << "Invalid move!\n"; goto B; }
			else if (valid == 9) {
				if (castling(board, move, player) == 1) { cout << "castling not possible\n"; goto B; }
				else { goto Q; }
			}

			if (board[move_index[0]][move_index[1]] == 'p')
			{
				valid = black_pawn(board, move_index);
			}
			else if (board[move_index[0]][move_index[1]] == 'r')
			{
				valid = black_rook(board, move_index);
			}
			else if (board[move_index[0]][move_index[1]] == 'k')
			{
				valid = black_king(board, move_index);
			}
			else if (board[move_index[0]][move_index[1]] == 'n')
			{
				valid = black_knight(board, move_index);
			}
			else if (board[move_index[0]][move_index[1]] == 'b')
			{
				valid = black_bishop(board, move_index);
			}
			else if (board[move_index[0]][move_index[1]] == 'q')
			{
				valid = black_queen(board, move_index);
			}
			else { cout << "Invalid move!\n"; goto B; }
			if (valid == 1) { cout << "Invalid move!\n"; goto B; }
			else if (valid == 8) {
				cout << "\n\n White Wins!!\n\n";
				game_end == "true";
				break;
			}

			//stale mate
			if (check_to_white_king(board) == 1)
			{
				if (check_to_white_king(board) == 1)
				{
					cout << "Check Mate\nBlack Wins";
					game_end == "true";
					break;
				}
				cout << "\n\nGAME DRAW!!";
				game_end == "true";
				break;
			}
			savemoves[stringsize] = move;
			stringsize++;
		}

	Q:		player++;
		system("CLS");



		for (int i = 0; i < 4; i++)
		{
			last_move[i] = move_index[i];
		}

	} while (game_end == "false");
	SaveData();

	return 0;
}


void print_board(char board[8][8])
{
	cout << endl << endl << endl << endl;

	cout << "                             a   b   c   d   e   f   g   h " << endl;
	cout << endl;
	cout << "                           ---------------------------------";
	cout << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << "                         ";

		cout << 8 - i << " ";
		cout << "| ";
		for (int j = 0; j < 8; j++)
		{


			cout << board[i][j];
			cout << " | ";
		}
		cout << endl;
		cout << "                           ---------------------------------";

		cout << endl;
	}
	cout << endl;
	cout << "                             a   b   c   d   e   f   g   h " << endl;
}
void enter_move(char move[6])
{

	cout << "\nEnter move : ";
	cin.getline(move, 6);


	//cin.ignore();
}
int move_to_index(char move[6], int  move_index[4])
{

	// move_index[0] = starting row
	// move_index[1] = starting col
	// move_index[2] = ending row
	// move_index[3] = ending col
	// sencond entry in 'move' is the starting row number 
	if (move[1] == '8') { move_index[0] = 0; }
	else if (move[1] == '7') { move_index[0] = 1; }
	else if (move[1] == '6') { move_index[0] = 2; }
	else if (move[1] == '5') { move_index[0] = 3; }
	else if (move[1] == '4') { move_index[0] = 4; }
	else if (move[1] == '3') { move_index[0] = 5; }
	else if (move[1] == '2') { move_index[0] = 6; }
	else if (move[1] == '1') { move_index[0] = 7; }
	else if (move[1] == '0') { return 9; }
	else { return 1; }
	// first entry in 'move' is the starting col number
	if (move[0] == 'a') { move_index[1] = 0; }
	else if (move[0] == 'b') { move_index[1] = 1; }
	else if (move[0] == 'c') { move_index[1] = 2; }
	else if (move[0] == 'd') { move_index[1] = 3; }
	else if (move[0] == 'e') { move_index[1] = 4; }
	else if (move[0] == 'f') { move_index[1] = 5; }
	else if (move[0] == 'g') { move_index[1] = 6; }
	else if (move[0] == 'h') { move_index[1] = 7; }
	else if (move[0] == '0') { return 9; }
	else { return 1; }

	int i = 2;
	if (move[2] == ' ') { i++; }// if space on 2nd indx it will check from third and then from forth
	// 3rd entry in 'move' is ending column number
	if (move[i] == 'a') { move_index[3] = 0; }
	else if (move[i] == 'b') { move_index[3] = 1; }
	else if (move[i] == 'c') { move_index[3] = 2; }
	else if (move[i] == 'd') { move_index[3] = 3; }
	else if (move[i] == 'e') { move_index[3] = 4; }
	else if (move[i] == 'f') { move_index[3] = 5; }
	else if (move[i] == 'g') { move_index[3] = 6; }
	else if (move[i] == 'h') { move_index[3] = 7; }
	else if (move[i] == '0') { return 9; }
	else { return 1; }
	// 4th entry in 'move' is ending row number
	i++;
	if (move[i] == '8') { move_index[2] = 0; }
	else if (move[i] == '7') { move_index[2] = 1; }
	else if (move[i] == '6') { move_index[2] = 2; }
	else if (move[i] == '5') { move_index[2] = 3; }
	else if (move[i] == '4') { move_index[2] = 4; }
	else if (move[i] == '3') { move_index[2] = 5; }
	else if (move[i] == '2') { move_index[2] = 6; }
	else if (move[i] == '1') { move_index[2] = 7; }
	else if (move[i] == '0') { return 9; }
	else { return 1; }


}


void SaveData() {

	int slot;
	cout << "Enter slot you want to save in: ";
a:	cin >> slot;

	if (slot > 3 || slot < 0) {
		cout << "Enter between 1 and 3 " << endl;
		goto a;
	}

	string filename = "savegame" + to_string(slot) + ".txt";
	ofstream Savegame(filename);


	for (int i = 0; i < stringsize; i++) {
		Savegame << savemoves[i] << endl;
	}


	Savegame.close();

	Savegame.open("format" + filename);

	Savegame << delay1 << endl;
	Savegame << delay2 << endl;
	Savegame << time_format;


	Savegame.close();


}
bool LoadData() {
	stringsize = 0;
	cout << "Enter slot you want to load" << endl;
	int slot;
c:	cin >> slot;
	if (slot > 3 || slot < 1) {
		cout << "Enter between 1 and 3" << endl;
		goto c;
	}

	string filename = "savegame" + to_string(slot) + ".txt";
	string temp;
	ifstream LoadData(filename);
	while (1) {
		LoadData >> temp;
		if (LoadData.eof()) {
			break;
		}
		savemoves[stringsize++] = temp;
	}

	LoadData.close();

	if (stringsize == 0) {
		return false;
	}
	else {
		LoadData.open("format" + filename);

		LoadData >> delay1;
		LoadData >> delay2;
		LoadData >> time_format;


		LoadData.close();
		return true;
	}

}

int white_pawn(char board[8][8], int move_index[4])
{
	if (is_white_king_under_check(board, move_index) == 1) {
		if (check_to_white_king(board) == 1) {
			return 8;
		}
		cout << "King is under attack\n"; return 1;
	}
	int diff_row = move_index[0] - move_index[2];
	int diff_col = move_index[3] - move_index[1];
	if (diff_row > 2 || diff_row < 0) { return 1; }// diff_row < 0" will check reverse move
	if (move_index[0] == move_index[2]) { return 1; }
	if (diff_col == 0)
	{
		if (move_index[0] != 6 && diff_row == 2)
		{
			return 1;
		}
		else if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'Q' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'R')
		{
			return 1;
		}
		else if (board[move_index[2]][move_index[3]] != ' ')// want to move straight but some thing is present
		{
			return 1;
		}
		else
		{

			board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
			board[(int)move_index[0]][(int)move_index[3]] = ' ';
			if (move_index[2] == 0)
			{
				char pawn_promotion;
				cout << "PAWN can be promoted to following\n(Q)ueen\n(B)ishop\n(R)ook\nk(N)ight\n";
			a:				cout << "Enter your choice : ";
				cin >> pawn_promotion;
				if (pawn_promotion == 'R') { board[(int)move_index[2]][(int)move_index[3]] = 'R'; }
				else if (pawn_promotion == 'Q') { board[(int)move_index[2]][(int)move_index[3]] = 'Q'; }
				else if (pawn_promotion == 'B') { board[(int)move_index[2]][(int)move_index[3]] = 'B'; }
				else if (pawn_promotion == 'N') { board[(int)move_index[2]][(int)move_index[3]] = 'N'; }
				else { goto a; }
			}
			return 0;
		}
	}// check en passant here
	else if (diff_col == 1 || diff_col == -1)
	{

		if (board[move_index[2]][move_index[3]] == ' ')
		{  // check en passant here  //if en passant condition proves return after swaping
			// for en passant by white, 1) last move of black must be moving pawn 2 steps forward  
			if ((last_move[0] == move_index[0] - 2) && last_move[1] == move_index[3] && last_move[3] == move_index[3] && last_move[2] == move_index[0] && ((last_move[2] - last_move[0]) == 2) && board[move_index[0]][move_index[3]] == 'p')
			{
				board[move_index[2]][move_index[3]] = board[move_index[0]][move_index[1]];
				board[move_index[0]][move_index[3]] = ' ';
				board[move_index[0]][move_index[1]] = ' ';
				return 0;
			}
			return 1;
		}
		else if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'Q' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'R')
		{
			return 1;
		}
		else
		{
			board[move_index[2]][move_index[3]] = board[move_index[0]][move_index[1]];
			board[move_index[0]][move_index[1]] = ' ';
			if (move_index[2] == 0)
			{
				char pawn_promotion;
				cout << "PAWN can be promoted to following\n(Q)ueen\n(B)ishop\n(R)ook\nk(N)ight\n ";
			b:				cout << "Enter your choice : ";
				cin >> pawn_promotion;
				if (pawn_promotion == 'R') { board[(int)move_index[2]][(int)move_index[3]] = 'R'; }
				else if (pawn_promotion == 'Q') { board[(int)move_index[2]][(int)move_index[3]] = 'Q'; }
				else if (pawn_promotion == 'B') { board[(int)move_index[2]][(int)move_index[3]] = 'B'; }
				else if (pawn_promotion == 'N') { board[(int)move_index[2]][(int)move_index[3]] = 'N'; }
				else { goto b; }
			}
			return 0;
		}
	}
	else
	{
		return 1;
	}

	return 0;
}
int black_pawn(char board[8][8], int move_index[4])
{
	if (is_black_king_under_check(board, move_index) == 1) {
		if (check_to_black_king(board) == 1) {
			return 8;
		}cout << "King is under attack\n"; return 1;
	}
	int diff_row = move_index[2] - move_index[0];
	int diff_col = move_index[3] - move_index[1];
	if (diff_col > 1 || diff_row > 2 || diff_row < 0) { return 1; }
	if (move_index[0] == move_index[2]) { return 1; }
	if (diff_col == 0)
	{
		if (move_index[0] != 1 && diff_row >= 2)
		{
			return 1;
		}
		else if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'q' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'r')
		{
			return 1;
		}
		else if (board[move_index[2]][move_index[3]] != ' ')// want to move straight but some thing is present
		{
			return 1;
		}
		else
		{
			board[move_index[2]][move_index[3]] = board[move_index[0]][move_index[1]];
			board[move_index[0]][move_index[1]] = ' ';
			if (move_index[2] == 7)
			{
				char pawn_promotion;
				cout << "PAWN can be promoted to following\n(q)ueen\n(b)ishop\n(r)ook\nk(n)ight\n";
			c:				cout << "Enter your choice : ";
				cin >> pawn_promotion;
				if (pawn_promotion == 'r') { board[(int)move_index[2]][(int)move_index[3]] = 'r'; }
				else if (pawn_promotion == 'q') { board[(int)move_index[2]][(int)move_index[3]] = 'q'; }
				else if (pawn_promotion == 'b') { board[(int)move_index[2]][(int)move_index[3]] = 'b'; }
				else if (pawn_promotion == 'n') { board[(int)move_index[2]][(int)move_index[3]] = 'n'; }
				else { goto c; }
			}
			return 0;
		}
	}
	else if (diff_col == 1 || diff_col == -1)
	{
		if (board[move_index[2]][move_index[3]] == ' ')
		{
			// en passant
			if ((last_move[0] == move_index[0] + 2) && last_move[1] == move_index[3] && last_move[3] == move_index[3] && last_move[2] == move_index[0] && ((last_move[0] - last_move[2]) == 2) && board[move_index[0]][move_index[3]] == 'P')
			{
				board[move_index[2]][move_index[3]] = board[move_index[0]][move_index[1]];
				board[move_index[0]][move_index[3]] = ' ';
				board[move_index[0]][move_index[1]] = ' ';
				return 0;
			}
			return 1;
		}
		else if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'q' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'r')
		{
			return 1;
		}
		else
		{
			board[move_index[2]][move_index[3]] = board[move_index[0]][move_index[1]];
			board[move_index[0]][move_index[1]] = ' ';
			if (move_index[2] == 7)
			{
				char pawn_promotion;
				cout << "PAWN can be promoted to following\n(q)ueen\n(b)ishop\n(r)ook\nk(n)ight\n";
			d:				cout << "Enter your choice : ";
				cin >> pawn_promotion;
				if (pawn_promotion == 'r') { board[(int)move_index[2]][(int)move_index[3]] = 'r'; }
				else if (pawn_promotion == 'q') { board[(int)move_index[2]][(int)move_index[3]] = 'q'; }
				else if (pawn_promotion == 'b') { board[(int)move_index[2]][(int)move_index[3]] = 'b'; }
				else if (pawn_promotion == 'n') { board[(int)move_index[2]][(int)move_index[3]] = 'n'; }
				else { goto d; }
			}
			return 0;
		}
	}
	else
	{
		return 1;
	}

	return 0;
}
int white_rook(char board[8][8], int move_index[4])
{

	if (is_white_king_under_check(board, move_index) == 1) {
		if (check_to_white_king(board) == 1) {
			return 8;
		}cout << "King is under attack\n"; return 1;
	}
	if (move_index[0] != move_index[2] && move_index[1] != move_index[3]) { return 1; }   // means enter move is not in same row and column

	if (move_index[1] == move_index[3])     // want to be in same col
	{
		if (move_index[0] > move_index[2]) // forward in same col
		{
			// loop to check in forward direction
			for (int i = move_index[0] - 1; i > move_index[2]; i--) // i starts from present row and check till end location
			{
				if (board[i][move_index[1]] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
		}
		else if (move_index[0] < move_index[2]) // reverse in same col
		{
			//loop if wants to move in reverse direction
			for (int i = move_index[0] + 1; i < move_index[2]; i++) // i starts from present row and check till end location
			{
				if (board[i][move_index[1]] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
		}


	}
	else if (move_index[0] == move_index[2]) // move in same row 
	{
		if (move_index[1] > move_index[3])
		{
			// loop to check in forward direction
			for (int i = move_index[1] - 1; i > move_index[2]; i--) // i starts from present row and check till end location
			{
				if (board[move_index[0]][i] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}

		}
		else if (move_index[1] < move_index[3])
		{
			//loop if wants to move in reverse direction
			for (int i = move_index[1] + 1; i < move_index[3]; i++) // i starts from present row and check till end location
			{
				if (board[move_index[0]][i] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
		}
	}

	return 0;
}
int black_rook(char board[8][8], int move_index[4])
{
	if (is_black_king_under_check(board, move_index) == 1) {
		if (check_to_black_king(board) == 1) {
			return 8;
		}cout << "King is under attack\n"; return 1;
	}
	if (move_index[0] != move_index[2] && move_index[1] != move_index[3]) { return 1; }   // means enter move is not in same row and column

	if (move_index[1] == move_index[3])     // want to be in same col
	{
		if (move_index[0] > move_index[2])//starting row > ending row // reverse in same col
		{
			// loop to check in forward direction
			for (int i = move_index[0] - 1; i > move_index[2]; i--) // i starts from present row and check till end location
			{
				if (board[i][move_index[1]] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}

		}
		else if (move_index[0] < move_index[2]) // forward in same col
		{
			//loop if wants to move in reverse direction
			for (int i = move_index[0] + 1; i < move_index[2]; i++) // i starts from present row and check till end location
			{
				if (board[i][move_index[1]] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
		}


	}
	else if (move_index[0] == move_index[2]) // move in same row 
	{
		if (move_index[1] > move_index[3])
		{
			// loop to check in forward direction
			for (int i = move_index[1] - 1; i > move_index[2]; i--) // i starts from present row and check till end location
			{
				if (board[move_index[0]][i] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}

		}
		else if (move_index[1] < move_index[3])
		{
			//loop if wants to move in reverse direction
			for (int i = move_index[3] - 1; i > move_index[1]; i--) // i starts from present row and check till end location
			{
				if (board[move_index[0]][i] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
		}
	}

	return 0;
}
int black_knight(char board[8][8], int move_index[4])
{
	if (is_black_king_under_check(board, move_index) == 1) {
		if (check_to_black_king(board) == 1) {
			return 8;
		}cout << "King is under attack\n"; return 1;
	}
	int diff_rows = abs(move_index[2] - move_index[0]);
	int diff_col = abs(move_index[3] - move_index[1]);

	if (diff_rows > 2 || diff_col > 2) { return 0; };

	if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
	{
		return 1;
	}
	else if (diff_col == 1 && diff_rows == 2)
	{
		board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
		board[(int)move_index[0]][(int)move_index[1]] = ' ';
		return 0;
	}
	else if (diff_col == 2 && diff_rows == 1)
	{
		board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
		board[(int)move_index[0]][(int)move_index[1]] = ' ';
		return 0;
	}

	return 0;
}
int white_knight(char board[8][8], int move_index[4])
{
	if (is_white_king_under_check(board, move_index) == 1) {
		if (check_to_white_king(board) == 1) {
			return 8;
		}cout << "King is under attack\n"; return 1;
	}
	int diff_rows = abs(move_index[2] - move_index[0]);
	int diff_col = abs(move_index[3] - move_index[1]);

	if (diff_rows > 2 || diff_col > 2) { return 0; };

	if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
	{
		return 1;
	}
	else if (diff_col == 1 && diff_rows == 2)
	{
		board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
		board[(int)move_index[0]][(int)move_index[1]] = ' ';
		return 0;
	}
	else if (diff_col == 2 && diff_rows == 1)
	{
		board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
		board[(int)move_index[0]][(int)move_index[1]] = ' ';
		return 0;
	}

	return 0;
}
int white_bishop(char board[8][8], int move_index[4])
{
	if (is_white_king_under_check(board, move_index) == 1) {
		if (check_to_white_king(board) == 1) {
			return 8;
		}cout << "King is under attack\n"; return 1;
	}
	//starting row ++ or --
	//col ++ or --
	if (move_index[0] == move_index[2] || move_index[1] == move_index[3]) { return 1; }

	if (move_index[0] > move_index[2])  // means row is decreasing moving upward from present location
	{
		if (move_index[1] > move_index[3]) // means col is decreasing moving left from present location
		{
			int startrow = move_index[0] - 1;
			int startcol = move_index[1] - 1;
			//main diagonal  both row and col wil decrement by same value
			for (; startrow > move_index[2] && startcol > move_index[3]; startrow--, startcol--)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if (move_index[1] < move_index[3])//col is increasing moving right from present location
		{
			// row is decreasing but col is increasing
			int startrow = move_index[0] - 1;
			int startcol = move_index[1] + 1;
			for (; startrow > move_index[2] && startcol < move_index[3]; startrow--, startcol++)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}

		}
	}
	else if (move_index[0] < move_index[2]) // row is increasing moving downward from present location
	{
		if (move_index[1] > move_index[3]) // means col is decreasing moving left from present location
		{
			// row is increasing but col is decreasing
			int startrow = move_index[0] + 1;
			int startcol = move_index[1] - 1;
			for (; startrow < move_index[2] && startcol > move_index[3]; startrow++, startcol--)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if (move_index[1] < move_index[3])//col is increasing moving right from present location
		{
			int startrow = move_index[0] + 1;
			int startcol = move_index[1] + 1;
			// main diagonal both will increment by same value
			for (; startrow < move_index[2] && startcol < move_index[3]; startrow++, startcol++)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}
	else { return 1; }
}
int black_bishop(char board[8][8], int move_index[4])
{
	if (is_black_king_under_check(board, move_index) == 1) {
		if (check_to_black_king(board) == 1) {
			return 8;
		}cout << "King is under attack\n"; return 1;
	}
	//starting row ++ or --
	//col ++ or --
	if (move_index[0] == move_index[2] || move_index[1] == move_index[3]) { return 1; }

	if (move_index[0] > move_index[2]) // means row is decreasing moving upward from present location
	{
		if (move_index[1] > move_index[3]) // means col is decreasing moving left from present location
		{
			int startrow = move_index[0] - 1;
			int startcol = move_index[1] - 1;
			//main diagonal  both row and col wil decrement by same value
			for (; startrow > move_index[2] && startcol > move_index[3]; startrow--, startcol--)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if (move_index[1] < move_index[3])//col is increasing moving right from present location
		{
			// row is decreasing but col is increasing
			int startrow = move_index[0] - 1;
			int startcol = move_index[1] + 1;
			for (; startrow > move_index[2] && startcol < move_index[3]; startrow--, startcol++)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}

		}
	}
	else if (move_index[0] < move_index[2]) // row is increasing moving downward from present location
	{
		if (move_index[1] > move_index[3]) // means col is decreasing moving left from present location
		{
			// row is increasing but col is decreasing
			int startrow = move_index[0] + 1;
			int startcol = move_index[1] - 1;
			for (; startrow < move_index[2] && startcol > move_index[3]; startrow++, startcol--)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if (move_index[1] < move_index[3])//col is increasing moving right from present location
		{
			int startrow = move_index[0] + 1;
			int startcol = move_index[1] + 1;
			// main diagonal both will increment by same value
			for (; startrow < move_index[2] && startcol < move_index[3]; startrow++, startcol++)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}

	return 0;
}
int white_queen(char board[8][8], int move_index[4])
{
	if (is_white_king_under_check(board, move_index) == 1) {
		if (check_to_white_king(board) == 1) {
			return 8;
		}cout << "King is under attack\n"; return 1;
	}
	// queen move == bishop + rook
	// check start row and end row are same , if same apply rook logic and move 
	// check start col and end col are same, if yes then apply move
	// check if he wants to move diagonaly then 

	// ROOK
	if (move_index[1] == move_index[3])     // want to be in same col
	{
		if (move_index[0] > move_index[2]) // forward in same col
		{
			// loop to check in forward direction
			for (int i = move_index[0] - 1; i > move_index[2]; i--) // i starts from present row and check till end location
			{
				if (board[i][move_index[1]] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
		}
		else if (move_index[0] < move_index[2]) // reverse in same col
		{
			//loop if wants to move in reverse direction
			for (int i = move_index[0] + 1; i < move_index[2]; i++) // i starts from present row and check till end location
			{
				if (board[i][move_index[1]] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
		}


	}
	else if (move_index[0] == move_index[2]) // move in same row 
	{
		if (move_index[1] > move_index[3])// move left 
		{
			// loop to check in forward direction
			for (int i = move_index[1] - 1; i > move_index[2]; i--) // i starts from present row and check till end location
			{
				if (board[move_index[0]][i] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}

		}
		else if (move_index[1] < move_index[3])
		{
			//loop if wants to move in reverse direction
			for (int i = move_index[1] + 1; i < move_index[3]; i++) // i starts from present row and check till end location
			{
				if (board[move_index[0]][i] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
		}
	}// bishop	
	else if (move_index[0] > move_index[2])  // means row is decreasing moving upward from present location
	{
		if (move_index[1] > move_index[3]) // means col is decreasing moving left from present location
		{
			int startrow = move_index[0] - 1;
			int startcol = move_index[1] - 1;
			//main diagonal  both row and col wil decrement by same value
			for (; startrow > move_index[2] && startcol > move_index[3]; startrow--, startcol--)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if (move_index[1] < move_index[3])//col is increasing moving right from present location
		{
			// row is decreasing but col is increasing
			int startrow = move_index[0] - 1;
			int startcol = move_index[1] + 1;
			for (; startrow > move_index[2] && startcol < move_index[3]; startrow--, startcol++)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}

		}
	}
	else if (move_index[0] < move_index[2]) // row is increasing moving downward from present location
	{
		if (move_index[1] > move_index[3]) // means col is decreasing moving left from present location
		{
			// row is increasing but col is decreasing
			int startrow = move_index[0] + 1;
			int startcol = move_index[1] - 1;
			for (; startrow < move_index[2] && startcol > move_index[3]; startrow++, startcol--)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'R' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'Q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if (move_index[1] < move_index[3])//col is increasing moving right from present location
		{
			int startrow = move_index[0] + 1;
			int startcol = move_index[1] + 1;
			// main diagonal both will increment by same value
			for (; startrow < move_index[2] && startcol < move_index[3]; startrow++, startcol++)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}

	return 0;
}
int black_queen(char board[8][8], int move_index[4])
{

	if (is_black_king_under_check(board, move_index) == 1) {
		if (check_to_black_king(board) == 1) {
			return 8;
		}cout << "King is under attack\n"; return 1;
	}


	// queen move == bishop + rook
	// check start row and end row are same , if same apply rook logic and move 
	// check start col and end col are same, if yes then apply move
	// check if he wants to move diagonaly then 

	// ROOK
	if (move_index[1] == move_index[3])     // want to be in same col
	{
		if (move_index[0] > move_index[2])//starting row > ending row // reverse in same col
		{
			// loop to check in forward direction
			for (int i = move_index[0] - 1; i > move_index[2]; i--) // i starts from present row and check till end location
			{
				if (board[i][move_index[1]] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}

		}
		else if (move_index[0] < move_index[2]) // forward in same col
		{
			//loop if wants to move in reverse direction
			for (int i = move_index[0] + 1; i < move_index[2]; i++) // i starts from present row and check till end location
			{
				if (board[i][move_index[1]] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
		}


	}
	else if (move_index[0] == move_index[2]) // move in same row 
	{
		if (move_index[1] > move_index[3])
		{
			// loop to check in forward direction
			for (int i = move_index[1] - 1; i > move_index[2]; i--) // i starts from present row and check till end location
			{
				if (board[move_index[0]][i] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}

		}
		else if (move_index[1] < move_index[3])
		{
			//loop if wants to move in reverse direction
			for (int i = move_index[3] - 1; i > move_index[1]; i--) // i starts from present row and check till end location
			{
				if (board[move_index[0]][i] != ' ')      // if path is clear start continue else invalid move as rook cannot jump
				{
					return 1;
				}
			}
			// if path is clear check ending position and then swap
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			else
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
		}
	}//bishop
	else if (move_index[0] > move_index[2]) // means row is decreasing moving upward from present location
	{
		if (move_index[1] > move_index[3]) // means col is decreasing moving left from present location
		{
			int startrow = move_index[0] - 1;
			int startcol = move_index[1] - 1;
			//main diagonal  both row and col wil decrement by same value
			for (; startrow > move_index[2] && startcol > move_index[3]; startrow--, startcol--)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if (move_index[1] < move_index[3])//col is increasing moving right from present location
		{
			// row is decreasing but col is increasing
			int startrow = move_index[0] - 1;
			int startcol = move_index[1] + 1;
			for (; startrow > move_index[2] && startcol < move_index[3]; startrow--, startcol++)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}

		}
	}
	else if (move_index[0] < move_index[2]) // row is increasing moving downward from present location
	{
		if (move_index[1] > move_index[3]) // means col is decreasing moving left from present location
		{
			// row is increasing but col is decreasing
			int startrow = move_index[0] + 1;
			int startcol = move_index[1] - 1;
			for (; startrow < move_index[2] && startcol > move_index[3]; startrow++, startcol--)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if (move_index[1] < move_index[3])//col is increasing moving right from present location
		{
			int startrow = move_index[0] + 1;
			int startcol = move_index[1] + 1;
			// main diagonal both will increment by same value
			for (; startrow < move_index[2] && startcol < move_index[3]; startrow++, startcol++)
			{
				if (board[startrow][startcol] != ' ') { return 1; }
			}
			if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'r' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'q')
			{
				return 1;
			}
			if (startrow == move_index[2] && startcol == move_index[3])//valid move
			{
				board[(int)move_index[2]][(int)move_index[3]] = board[(int)move_index[0]][(int)move_index[1]];
				board[(int)move_index[0]][(int)move_index[1]] = ' ';
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}

	return 0;
}
int white_king(char board[8][8], int move_index[4])
{
	if (is_white_king_under_check(board, move_index) == 1) {
		if (check_to_white_king(board) == 1) {
			return 8;
		}cout << "King is under attack\n"; return 1;
	}
	int diff_row = move_index[2] - move_index[0];
	int diff_col = move_index[3] - move_index[1];

	if (diff_row > 1 || diff_row < -1) { return 1; };
	if (diff_col > 1 || diff_col < -1) { return 1; };
	if (board[move_index[2]][move_index[3]] == 'P' || board[move_index[2]][move_index[3]] == 'N' || board[move_index[2]][move_index[3]] == 'B' || board[move_index[2]][move_index[3]] == 'Q' || board[move_index[2]][move_index[3]] == 'K' || board[move_index[2]][move_index[3]] == 'R')
	{
		return 1;
	}

	board[move_index[2]][move_index[3]] = board[move_index[0]][move_index[1]];
	board[move_index[0]][move_index[1]] = ' ';
	return 0;
}
int black_king(char board[8][8], int move_index[4])
{
	if (is_black_king_under_check(board, move_index) == 1) {
		if (check_to_black_king(board) == 1) {
			return 8;
		}cout << "King is under attack\n"; return 1;
	}
	int diff_row = move_index[2] - move_index[0];
	int diff_col = move_index[3] - move_index[1];

	if (diff_row > 1 || diff_row < -1) { return 1; };
	if (diff_col > 1 || diff_col < -1) { return 1; };
	if (board[move_index[2]][move_index[3]] == 'p' || board[move_index[2]][move_index[3]] == 'n' || board[move_index[2]][move_index[3]] == 'b' || board[move_index[2]][move_index[3]] == 'q' || board[move_index[2]][move_index[3]] == 'k' || board[move_index[2]][move_index[3]] == 'r')
	{
		return 1;
	}

	board[move_index[2]][move_index[3]] = board[move_index[0]][move_index[1]];
	board[move_index[0]][move_index[1]] = ' ';
	return 0;
}
bool is_white_king_under_check(char board[8][8], int move_index[4])
{
	// move board into new board swap the entered position and then check is king under check
	char board2[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board2[i][j] = board[i][j];
		}
	}
	board2[(int)move_index[2]][(int)move_index[3]] = board2[(int)move_index[0]][(int)move_index[1]];
	board2[(int)move_index[0]][(int)move_index[1]] = ' ';

	int king_position[2];// = { 1,2 };
	for (int i = 0; i < 8; i++)   // find position of king
	{
		for (int j = 0; j < 8; j++)
		{
			if (board2[i][j] == 'K')
			{
				king_position[0] = i;//row
				king_position[1] = j;//col
			}
		}
	}
	// check by rook and queen in same col
	for (int i = 0; i < 8; i++)// check in col 
	{
		if (board2[king_position[0]][i] == 'r' || board2[king_position[0]][i] == 'q')
		{
			return 1;
		}
	}
	//check by rook and queen in same row
	for (int i = 0; i < 8; i++)//check in row
	{
		if (board2[i][king_position[1]] == 'r' || board2[i][king_position[1]] == 'q')
		{
			return 1;
		}
	}
	// check by pawn
	if (board2[king_position[0] - 1][king_position[1] + 1] == 'p')  // row -- for white king 
	{
		return 1;
	}
	else if (board2[king_position[0] - 1][king_position[1] - 1] == 'p')  // row -- for white king 
	{
		return 1;
	}
	// check by knight
	if (board2[king_position[0] - 2][king_position[1] - 1] == 'n' || board2[king_position[0] - 2][king_position[1] + 1] == 'n' || board2[king_position[0] + 2][king_position[1] - 1] == 'n' || board2[king_position[0] + 2][king_position[1] + 1] == 'n')
	{
		return 1;
	}
	else if (board2[king_position[0] - 1][king_position[1] - 2] == 'n' || board2[king_position[0] + 1][king_position[1] - 2] == 'n' || board2[king_position[0] - 1][king_position[1] + 2] == 'n' || board2[king_position[0] + 1][king_position[1] + 2] == 'n')
	{
		return 1;
	}

	// check by bishop and queen in diagnol
	for (int i = king_position[0], j = king_position[1]; i >= 0 && i <= 7 && j <= 7 && j >= 0; i++, j++)
	{
		if (board2[i][j] == 'b' || board2[i][j] == 'q')
		{
			return 1;
		}
	}
	for (int i = king_position[0], j = king_position[1]; i >= 0 && i <= 7 && j <= 7 && j >= 0; i--, j--)
	{
		if (board2[i][j] == 'b' || board2[i][j] == 'q')
		{
			return 1;
		}
	}
	for (int i = king_position[0], j = king_position[1]; i >= 0 && i <= 7 && j <= 7 && j >= 0; i++, j--)
	{
		if (board2[i][j] == 'b' || board2[i][j] == 'q')
		{
			return 1;
		}
	}
	for (int i = king_position[0], j = king_position[1]; i >= 0 && i <= 7 && j <= 7 && j >= 0; i--, j++)
	{
		if (board2[i][j] == 'b' || board2[i][j] == 'q')
		{
			return 1;
		}
	}
	return 0;
}
bool is_black_king_under_check(char board[8][8], int move_index[4])
{
	// move board into new board swap the entered position and then check is king under check
	char board2[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board2[i][j] = board[i][j];
		}
	}
	board2[(int)move_index[2]][(int)move_index[3]] = board2[(int)move_index[0]][(int)move_index[1]];
	board2[(int)move_index[0]][(int)move_index[1]] = ' ';
	int king_position[2];
	for (int i = 0; i < 8; i++)   // find position of king
	{
		for (int j = 0; j < 8; j++)
		{
			if (board2[i][j] == 'k')
			{
				king_position[0] = i;//row
				king_position[1] = j;//col
			}
		}
	}
	// check by rook and queen in same col
	for (int i = 0; i < 8; i++)// check in col 
	{
		if (board2[king_position[0]][i] == 'R' || board2[king_position[0]][i] == 'Q')
		{
			return 1;
		}
	}
	//check by rook and queen in same row
	for (int i = 0; i < 8; i++)//check in row
	{
		if (board2[i][king_position[1]] == 'R' || board2[i][king_position[1]] == 'Q')
		{
			return 1;
		}
	}
	// check by pawn
	if (board2[king_position[0] + 1][king_position[1] + 1] == 'P')  // row -- for white king 
	{
		return 1;
	}
	else if (board2[king_position[0] + 1][king_position[1] - 1] == 'P')  // row -- for white king 
	{
		return 1;
	}
	// check by knight
	if (board2[king_position[0] - 2][king_position[1] - 1] == 'N' || board2[king_position[0] - 2][king_position[1] + 1] == 'N' || board2[king_position[0] + 2][king_position[1] - 1] == 'N' || board2[king_position[0] + 2][king_position[1] + 1] == 'N')
	{
		return 1;
	}
	else if (board2[king_position[0] - 1][king_position[1] - 2] == 'N' || board2[king_position[0] + 1][king_position[1] - 2] == 'N' || board2[king_position[0] - 1][king_position[1] + 2] == 'N' || board2[king_position[0] + 1][king_position[1] + 2] == 'N')
	{
		return 1;
	}

	// check by bishop and queen in diagnol
	for (int i = king_position[0], j = king_position[1]; i >= 0 && i <= 7 && j <= 7 && j >= 0; i++, j++)
	{
		if (board2[i][j] == 'B' || board2[i][j] == 'Q')
		{
			return 1;
		}
	}
	for (int i = king_position[0], j = king_position[1]; i >= 0 && i <= 7 && j <= 7 && j >= 0; i--, j--)
	{
		if (board2[i][j] == 'B' || board2[i][j] == 'Q')
		{
			return 1;
		}
	}
	for (int i = king_position[0], j = king_position[1]; i >= 0 && i <= 7 && j <= 7 && j >= 0; i--, j++)
	{
		if (board2[i][j] == 'B' || board2[i][j] == 'Q')
		{
			return 1;
		}
	}
	for (int i = king_position[0], j = king_position[1]; i >= 0 && i <= 7 && j <= 7 && j >= 0; i++, j--)
	{
		if (board2[i][j] == 'B' || board2[i][j] == 'Q')
		{
			return 1;
		}
	}
	return 0;
}
bool castling(char board[8][8], char move[5], int player)
{

	if (player % 2 == 0)//white's move
	{

		if (move[0] == '0' && move[1] == '0' && move[2] == '0')
		{
			// queen side castling
			if (board[7][4] != 'K' && board[7][0] != 'R') { return 1; }

			for (int i = 3; i > 0; i--)
			{
				if (board[7][i] != ' ') { return 1; }
			}

			for (int i = 0, j = 0; i < stringsize; i++, j + 4)
			{
				if (savemoves[j] == "e" && savemoves[j + 1] == "1")
				{
					// king move is present so cannot castle
					return 1;
				}
				if (savemoves[j] == "h" && savemoves[j + 1] == "1")
				{
					// rook move is present so cannot castle
					return 1;
				}
			}
			int move[4] = { 7,4, 7,2 };
			if (is_white_king_under_check(board, move) == 1) { return 1; }

			board[7][2] = board[7][4];
			board[7][4] = ' ';
			board[7][3] = board[7][0];
			board[7][0] = ' ';
			return 0;
		}
		else if (move[0] == '0' && move[1] == '0')
		{
			// king side castling
			if (board[7][4] != 'K' && board[7][7] != 'R') { return 1; }

			for (int i = 5; i <= 6; i++)
			{
				if (board[7][i] != ' ') { return 1; }
			}

			for (int i = 0, j = 0; i < stringsize; i++, j + 4)
			{
				if (savemoves[j] == "e" && savemoves[j + 1] == "1")
				{
					// king move is present so cannot castle
					return 1;
				}
				if (savemoves[j] == "h" && savemoves[j + 1] == "1")
				{
					// rook move is present so cannot castle
					return 1;
				}
			}
			int move[4] = { 7,4, 7,6 };
			if (is_white_king_under_check(board, move) == 1) { return 1; }

			board[7][6] = board[7][4];
			board[7][4] = ' ';
			board[7][5] = board[7][7];
			board[7][7] = ' ';
			return 0;
		}
	}
	else
	{
		if (move[0] == '0' && move[1] == '0' && move[2] == '0')
		{
			// queen side castling
			if (board[0][4] != 'k' && board[0][0] != 'r') { return 1; }

			for (int i = 3; i > 0; i--)
			{
				if (board[0][i] != ' ') { return 1; }
			}

			for (int i = 0, j = 0; i < stringsize; i++, j + 4)
			{
				if (savemoves[j] == "e" && savemoves[j + 1] == "8")
				{
					// king move is present so cannot castle
					return 1;
				}
				if (savemoves[j] == "h" && savemoves[j + 1] == "8")
				{
					// rook move is present so cannot castle
					return 1;
				}
			}
			int move[4] = { 0,4, 0,2 };
			if (is_black_king_under_check(board, move) == 1) { return 1; }

			board[0][2] = board[0][4];
			board[0][4] = ' ';
			board[0][3] = board[0][0];
			board[0][0] = ' ';
			return 0;
		}
		else if (move[0] == '0' && move[1] == '0')
		{
			// king side castling
			if (board[0][4] != 'k' && board[0][7] != 'r') { return 1; }

			for (int i = 5; i <= 6; i++)
			{
				if (board[0][i] != ' ') { return 1; }
			}

			for (int i = 0, j = 0; i < stringsize; i++, j + 4)
			{
				if (savemoves[j] == "e" && savemoves[j + 1] == "8")
				{
					// king move is present so cannot castle
					return 1;
				}
				if (savemoves[j] == "h" && savemoves[j + 1] == "8")
				{
					// rook move is present so cannot castle
					return 1;
				}
			}
			int move[4] = { 0,4, 0,6 };
			if (is_black_king_under_check(board, move) == 1) { return 1; }

			board[0][6] = board[0][4];
			board[0][4] = ' ';
			board[0][5] = board[0][7];
			board[0][7] = ' ';
			return 0;
		}
	}
}



bool Bcheck(char board[8][8])
{
	int kingrow, kingcol, check = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'k')
			{
				kingrow = i;
				kingcol = j;
			}
		}
	}
	// check by rook and queen in same col
	for (int i = 0; i < 8; i++)// check in col 
	{
		if (board[kingrow][i] == 'R' || board[kingrow][i] == 'Q')
		{
			return  1;
		}
	}
	//check by rook and queen in same row
	for (int i = 0; i < 8; i++)//check in row
	{
		if (board[i][kingcol] == 'R' || board[i][kingcol] == 'Q')
		{
			return 1;
		}
	}
	if (board[kingrow + 1][kingcol + 1] == 'P')  // row ++ for black king 
	{
		return 1;
	}
	else if (board[kingrow + 1][kingcol - 1] == 'P')  // row ++ for black king 
	{
		return 1;
	}
	// check by knight
	if (board[kingrow - 2][kingcol - 1] == 'N' || board[kingcol - 2][kingcol + 1] == 'N' || board[kingrow + 2][kingcol - 1] == 'N' || board[kingrow + 2][kingcol + 1] == 'N')
	{
		return 1;
	}
	else if (board[kingrow - 1][kingcol - 2] == 'N' || board[kingrow + 1][kingcol - 2] == 'N' || board[kingrow - 1][kingcol + 2] == 'N' || board[kingrow + 1][kingcol + 2] == 'N')
	{
		return 1;
	}

	// check by bishop and queen in diagnol
	for (int i = kingrow, k = kingcol; i < 8 && i >= 0 && k < 8 && k >= 0; i++, k++) // for  main diagnol downward
	{
		if (board[i][k] == 'B' || board[i][k] == 'Q')
		{
			return 1;
		}
	}
	for (int i = kingrow, k = kingcol; i < 8 && i >= 0 && k < 8 && k >= 0; i--, k--)// for mian diagnol upward
	{
		if (board[i][k] == 'B' || board[i][k] == 'Q')
		{
			return 1;
		}
	}
	for (int i = kingrow, k = kingcol; i < 8 && i >= 0 && k < 8 && k >= 0; i++, k--)// for diagnol downward
	{
		if (board[i][k] == 'B' || board[i][k] == 'Q')
		{
			return 1;
		}
	}
	for (int i = kingrow, k = kingcol; i < 8 && i >= 0 && k < 8 && k >= 0; i--, k++)// for diagnol upward
	{
		if (board[i][k] == 'B' || board[i][k] == 'Q')
		{
			return 1;
		}
	}

	return 0;
}
bool Wcheck(char board[8][8])
{
	int kingrow, kingcol, check = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'K')
			{
				kingrow = i;
				kingcol = j;
			}
		}
	}
	// check by rook and queen in same col
	for (int i = 0; i < 8; i++)// check in col 
	{
		if (board[kingrow][i] == 'r' || board[kingrow][i] == 'q')
		{
			return  1;
		}
	}
	//check by rook and queen in same row
	for (int i = 0; i < 8; i++)//check in row
	{
		if (board[i][kingcol] == 'r' || board[i][kingcol] == 'q')
		{
			return 1;
		}
	}
	// check by pawn
	if (board[kingrow - 1][kingcol + 1] == 'p')  // row -- for white king 
	{
		return 1;
	}
	else if (board[kingrow - 1][kingcol - 1] == 'p')  // row -- for white king 
	{
		return 1;
	}
	// check by knight
	if (board[kingrow - 2][kingcol - 1] == 'n' || board[kingrow - 2][kingcol + 1] == 'n' || board[kingrow + 2][kingcol - 1] == 'n' || board[kingrow + 2][kingcol + 1] == 'n')
	{
		return 1;
	}
	else if (board[kingrow - 1][kingcol - 2] == 'n' || board[kingrow + 1][kingcol - 2] == 'n' || board[kingrow - 1][kingcol + 2] == 'n' || board[kingrow + 1][kingcol + 2] == 'n')
	{
		return 1;
	}
	// check by bishop and queen in diagnol
	for (int i = kingrow, k = kingcol; i < 8 && i >= 0 && k < 8 && k >= 0; i++, k++) // for  main diagnol downward
	{
		if (board[i][k] == 'b' || board[i][k] == 'q')
		{
			return 1;
		}
	}
	for (int i = kingrow, k = kingcol; i < 8 && i >= 0 && k < 8 && k >= 0; i--, k--)// for mian diagnol upward
	{
		if (board[i][k] == 'b' || board[i][k] == 'q')
		{
			return 1;
		}
	}
	for (int i = kingrow, k = kingcol; i < 8 && i >= 0 && k < 8 && k >= 0; i++, k--)// for diagnol downward
	{
		if (board[i][k] == 'b' || board[i][k] == 'q')
		{
			return 1;
		}
	}
	for (int i = kingrow, k = kingcol; i < 8 && i >= 0 && k < 8 && k >= 0; i--, k++)// for diagnol upward
	{
		if (board[i][k] == 'b' || board[i][k] == 'q')
		{
			return 1;
		}
	}

	return 0;
}
bool Wpawn(int row, int col, char board[8][8])
{
	char board3[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board3[i][j] = board[i][j];
		}
	}
	for (int i = 1; i <= 2; i++) {

		if (board[row + i][col] == ' ') {
			board3[row + i][col] = board3[row][col];
			if (Wcheck(board3) == 1) { continue; }
			else if (Wcheck(board3) == 0) { return 0; }// 0 return from Wcheck means after this move check will be remove
		}
	}


}
bool Wrook(int row, int col, char board[8][8])
{
	char board3[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board3[i][j] = board[i][j];
		}
	}


	for (int i = row; i <= 8; i++) {

		if (board[1 + i][col] == ' ') {
			board3[1 + i][col] = board3[row][col];
			if (Wcheck(board3) == 1) { return 1; }
		}
	}
	for (int i = col; i <= 8; i++) {

		if (board[row][i + 1] == ' ') {
			board3[row][1 + i] = board3[row][col];
			if (Wcheck(board3) == 1) { return 1; }
		}
	}
	for (int i = row; i > 0; i--) {

		if (board[i - 1][col] == ' ') {
			board3[i - 1][col] = board3[row][col];
			if (Wcheck(board3) == 1) { return 1; }
		}
	}
	for (int i = col; i > 0; i++) {

		if (board[row][i - 1] == ' ') {
			board3[row][1 - 1] = board3[row][col];
			if (Wcheck(board3) == 1) { return 1; }
		}
	}


}
bool Wbishop(int row, int col, char board[8][8])
{
	char board2[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board2[i][j] = board[i][j];
		}
	}

	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i++, j++)
	{
		if (board[i + 1][j + 1] == ' ') {
			board2[i + 1][j + 1] = board2[row][col];
			if (Wcheck(board2) == 1) { continue; }
			else if (Wcheck(board2) == 1) { return 0; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i--, j--)
	{
		if (board[1 - i][1 - j] == ' ') {
			board2[1 - i][1 - j] = board2[row][col];
			if (Wcheck(board2) == 1) { continue; }
			else if (Wcheck(board2) == 1) { return 0; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i++, j--)
	{
		if (board[1 + i][1 - j] == ' ') {
			board2[1 + i][1 - j] = board2[row][col];
			if (Wcheck(board2) == 1) { continue; }
			else if (Wcheck(board2) == 1) { return 0; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i--, j++)
	{
		if (board[1 - i][1 + j] == ' ') {
			board2[1 - i][1 + j] = board2[row][col];
			if (Wcheck(board2) == 1) { continue; }
			else if (Wcheck(board2) == 1) { return 0; }
		}
	}

}
bool Wqueen(int row, int col, char board[8][8])
{
	char board2[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board2[i][j] = board[i][j];
		}
	}

	for (int i = row; i <= 8; i++) {

		if (board[1 + i][col] == ' ') {
			board2[1 + i][col] = board2[row][col];
			if (Wcheck(board2) == 1) { return 1; }
		}
	}
	for (int i = col; i <= 8; i++) {

		if (board[row][i + 1] == ' ') {
			board2[row][1 + i] = board2[row][col];
			if (Wcheck(board2) == 1) { return 1; }
		}
	}
	for (int i = row; i > 0; i--) {

		if (board[i - 1][col] == ' ') {
			board2[i - 1][col] = board2[row][col];
			if (Wcheck(board2) == 1) { return 1; }
		}
	}
	for (int i = col; i > 0; i++) {

		if (board[row][i - 1] == ' ') {
			board2[row][1 - 1] = board2[row][col];
			if (Wcheck(board2) == 1) { return 1; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i++, j++)
	{
		if (board[i + 1][j + 1] == ' ') {
			board2[i + 1][j + 1] = board2[row][col];
			if (Wcheck(board2) == 1) { continue; }
			else if (Wcheck(board2) == 1) { return 0; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i--, j--)
	{
		if (board[1 - i][1 - j] == ' ') {
			board2[1 - i][1 - j] = board2[row][col];
			if (Wcheck(board2) == 1) { continue; }
			else if (Wcheck(board2) == 1) { return 0; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i++, j--)
	{
		if (board[1 + i][1 - j] == ' ') {
			board2[1 + i][1 - j] = board2[row][col];
			if (Wcheck(board2) == 1) { continue; }
			else if (Wcheck(board2) == 1) { return 0; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i--, j++)
	{
		if (board[1 - i][1 + j] == ' ') {
			board2[1 - i][1 + j] = board2[row][col];
			if (Wcheck(board2) == 1) { continue; }
			else if (Wcheck(board2) == 1) { return 0; }
		}
	}

}
bool Wking(int row, int col, char board[8][8])
{
	char board3[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board3[i][j] = board[i][j];
		}
	}
	if ((row - 1) >= 0 && (col - 1) > 0) {
		if (board3[row - 1][col - 1] == ' ')
		{
			board3[row - 1][col - 1] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}

	if ((row + 1) >= 0 && (col + 1) > 0) {
		if (board3[row + 1][col + 1] == ' ')
		{
			board3[row + 1][col + 1] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}
	if ((row + 1) >= 0 && (col - 1) > 0) {
		if (board3[row + 1][col - 1] == ' ')
		{
			board3[row + 1][col - 1] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}

	if ((row - 1) >= 0 && (col + 1) > 0) {
		if (board3[row - 1][col + 1] == ' ')
		{
			board3[row - 1][col + 1] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}

	if ((col + 1) > 0) {
		if (board3[row][col + 1] == ' ')
		{
			board3[row][col + 1] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}

	if ((col - 1) > 0) {
		if (board3[row][col - 1] == ' ')
		{
			board3[row][col + 1] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}
	if ((row + 1) > 0) {
		if (board3[row + 1][col] == ' ')
		{
			board3[row + 1][col] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}
	if ((row - 1) > 0) {
		if (board3[row - 1][col] == ' ')
		{
			board3[row - 1][col] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}


}
bool Wknight(int row, int col, char board[8][8])
{
	char board3[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board3[i][j] = board[i][j];
		}
	}

	if ((row + 2 < 8) && (col - 1) >= 0)
	{
		if (board3[row + 2][col - 1] == ' ')
		{
			board3[row + 2][col - 1] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}
	if ((row + 2 < 8) && (col + 1) < 8)
	{
		if (board3[row + 2][col - 1] == ' ')
		{
			board3[row + 2][col - 1] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}
	if ((row + 1 < 8) && (col + 2) < 8)
	{
		if (board3[row + 1][col + 2] == ' ')
		{
			board3[row + 1][col + 2] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}
	if ((row + 1 < 8) && (col - 2) >= 0)
	{
		if (board3[row + 1][col - 2] == ' ')
		{
			board3[row + 1][col - 2] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}
	if ((row - 1 >= 0) && (col + 2) < 8)
	{
		if (board3[row - 1][col + 2] == ' ')
		{
			board3[row - 1][col + 2] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}
	if ((row - 1 >= 0) && (col - 2) >= 0)
	{
		if (board3[row - 1][col - 2] == ' ')
		{
			board3[row - 1][col - 2] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}
	if ((row - 2 >= 0) && (col + 1) < 8)
	{
		if (board3[row - 2][col + 1] == ' ')
		{
			board3[row - 2][col + 1] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}
	if ((row - 2 >= 0) && (col - 1) < 8)
	{
		if (board3[row - 2][col - 1] == ' ')
		{
			board3[row - 2][col - 1] = board3[row][col];
			if (Wcheck(board3) == 0) { return 0; }
		}
	}
}
bool Bpawn(int row, int col, char board[8][8])
{
	char board3[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board3[i][j] = board[i][j];
		}
	}
	for (int i = 1; i <= 2; i++) {

		if (board[row + i][col] == ' ') {
			board3[row + i][col] = board3[row][col];
			if (Bcheck(board3) == 1) { continue; }
			else if (Bcheck(board3) == 0) { return 0; }// 0 return from Wcheck means after this move check will be remove
		}
	}


}
bool Brook(int row, int col, char board[8][8])
{
	char board3[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board3[i][j] = board[i][j];
		}
	}


	for (int i = row; i <= 8; i++) {

		if (board[1 + i][col] == ' ') {
			board3[1 + i][col] = board3[row][col];
			if (Bcheck(board3) == 1) { return 1; }
		}
	}
	for (int i = col; i <= 8; i++) {

		if (board[row][i + 1] == ' ') {
			board3[row][1 + i] = board3[row][col];
			if (Bcheck(board3) == 1) { return 1; }
		}
	}
	for (int i = row; i > 0; i--) {

		if (board[i - 1][col] == ' ') {
			board3[i - 1][col] = board3[row][col];
			if (Bcheck(board3) == 1) { return 1; }
		}
	}
	for (int i = col; i > 0; i++) {

		if (board[row][i - 1] == ' ') {
			board3[row][1 - 1] = board3[row][col];
			if (Bcheck(board3) == 1) { return 1; }
		}
	}


}
bool Bbishop(int row, int col, char board[8][8])
{
	char board2[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board2[i][j] = board[i][j];
		}
	}

	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i++, j++)
	{
		if (board[i + 1][j + 1] == ' ') {
			board2[i + 1][j + 1] = board2[row][col];
			if (Bcheck(board2) == 1) { continue; }
			else if (Bcheck(board2) == 1) { return 0; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i--, j--)
	{
		if (board[1 - i][1 - j] == ' ') {
			board2[1 - i][1 - j] = board2[row][col];
			if (Bcheck(board2) == 1) { continue; }
			else if (Bcheck(board2) == 1) { return 0; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i++, j--)
	{
		if (board[1 + i][1 - j] == ' ') {
			board2[1 + i][1 - j] = board2[row][col];
			if (Bcheck(board2) == 1) { continue; }
			else if (Bcheck(board2) == 1) { return 0; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i--, j++)
	{
		if (board[1 - i][1 + j] == ' ') {
			board2[1 - i][1 + j] = board2[row][col];
			if (Bcheck(board2) == 1) { continue; }
			else if (Bcheck(board2) == 1) { return 0; }
		}
	}

}
bool Bqueen(int row, int col, char board[8][8])
{
	char board2[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board2[i][j] = board[i][j];
		}
	}

	for (int i = row; i <= 8; i++) {

		if (board[1 + i][col] == ' ') {
			board2[1 + i][col] = board2[row][col];
			if (Bcheck(board2) == 1) { return 1; }
		}
	}
	for (int i = col; i <= 8; i++) {

		if (board[row][i + 1] == ' ') {
			board2[row][1 + i] = board2[row][col];
			if (Bcheck(board2) == 1) { return 1; }
		}
	}
	for (int i = row; i > 0; i--) {

		if (board[i - 1][col] == ' ') {
			board2[i - 1][col] = board2[row][col];
			if (Bcheck(board2) == 1) { return 1; }
		}
	}
	for (int i = col; i > 0; i++) {

		if (board[row][i - 1] == ' ') {
			board2[row][1 - 1] = board2[row][col];
			if (Bcheck(board2) == 1) { return 1; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i++, j++)
	{
		if (board[i + 1][j + 1] == ' ') {
			board2[i + 1][j + 1] = board2[row][col];
			if (Bcheck(board2) == 1) { continue; }
			else if (Bcheck(board2) == 1) { return 0; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i--, j--)
	{
		if (board[1 - i][1 - j] == ' ') {
			board2[1 - i][1 - j] = board2[row][col];
			if (Bcheck(board2) == 1) { continue; }
			else if (Bcheck(board2) == 1) { return 0; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i++, j--)
	{
		if (board[1 + i][1 - j] == ' ') {
			board2[1 + i][1 - j] = board2[row][col];
			if (Bcheck(board2) == 1) { continue; }
			else if (Bcheck(board2) == 1) { return 0; }
		}
	}
	for (int i = row, j = col; i >= 0 && i <= 7 && j <= 7 && j >= 0; i--, j++)
	{
		if (board[1 - i][1 + j] == ' ') {
			board2[1 - i][1 + j] = board2[row][col];
			if (Bcheck(board2) == 1) { continue; }
			else if (Bcheck(board2) == 1) { return 0; }
		}
	}

}
bool Bking(int row, int col, char board[8][8])
{
	char board3[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board3[i][j] = board[i][j];
		}
	}
	if ((row - 1) >= 0 && (col - 1) > 0) {
		if (board3[row - 1][col - 1] == ' ')
		{
			board3[row - 1][col - 1] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}

	if ((row + 1) >= 0 && (col + 1) > 0) {
		if (board3[row + 1][col + 1] == ' ')
		{
			board3[row + 1][col + 1] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}
	if ((row + 1) >= 0 && (col - 1) > 0) {
		if (board3[row + 1][col - 1] == ' ')
		{
			board3[row + 1][col - 1] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}

	if ((row - 1) >= 0 && (col + 1) > 0) {
		if (board3[row - 1][col + 1] == ' ')
		{
			board3[row - 1][col + 1] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}

	if ((col + 1) > 0) {
		if (board3[row][col + 1] == ' ')
		{
			board3[row][col + 1] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}

	if ((col - 1) > 0) {
		if (board3[row][col - 1] == ' ')
		{
			board3[row][col + 1] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}
	if ((row + 1) > 0) {
		if (board3[row + 1][col] == ' ')
		{
			board3[row + 1][col] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}
	if ((row - 1) > 0) {
		if (board3[row - 1][col] == ' ')
		{
			board3[row - 1][col] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}


}
bool Bknight(int row, int col, char board[8][8])
{
	char board3[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board3[i][j] = board[i][j];
		}
	}

	if ((row + 2 < 8) && (col - 1) >= 0)
	{
		if (board3[row + 2][col - 1] == ' ')
		{
			board3[row + 2][col - 1] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}
	if ((row + 2 < 8) && (col + 1) < 8)
	{
		if (board3[row + 2][col - 1] == ' ')
		{
			board3[row + 2][col - 1] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}
	if ((row + 1 < 8) && (col + 2) < 8)
	{
		if (board3[row + 1][col + 2] == ' ')
		{
			board3[row + 1][col + 2] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}
	if ((row + 1 < 8) && (col - 2) >= 0)
	{
		if (board3[row + 1][col - 2] == ' ')
		{
			board3[row + 1][col - 2] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}
	if ((row - 1 >= 0) && (col + 2) < 8)
	{
		if (board3[row - 1][col + 2] == ' ')
		{
			board3[row - 1][col + 2] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}
	if ((row - 1 >= 0) && (col - 2) >= 0)
	{
		if (board3[row - 1][col - 2] == ' ')
		{
			board3[row - 1][col - 2] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}
	if ((row - 2 >= 0) && (col + 1) < 8)
	{
		if (board3[row - 2][col + 1] == ' ')
		{
			board3[row - 2][col + 1] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}
	if ((row - 2 >= 0) && (col - 1) < 8)
	{
		if (board3[row - 2][col - 1] == ' ')
		{
			board3[row - 2][col - 1] = board3[row][col];
			if (Bcheck(board3) == 0) { return 0; }
		}
	}
}


bool check_to_white_king(char board[8][8])
{

	char board2[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board2[i][j] = board[i][j];
		}
	}



	// moving every possible move and checking will the king be in check or not

	// to find different pawn make a condition in which first it find first pawn and without doing 
	// anything it starts finding another pawn if it finds pawn on same location skip and and move 
	int positions[32], positions_size = 0;

	//pawn
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'P')
			{
				positions[positions_size] = i;
				positions_size++;
				positions[positions_size] = j;
				positions_size++;
				// here call a function to find valid moves for pawn
				int x = Wpawn(positions[positions_size - 2], positions[positions_size - 1], board);
				if (x == 0) { return 0; }
			}
		}
	}
	//bishop
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'B')
			{
				positions[positions_size] = i;
				positions_size++;
				positions[positions_size] = j;
				positions_size++;
				// function call to check every possible move of bishop
				int x = Wbishop(positions[positions_size - 2], positions[positions_size - 1], board);
				if (x == 0) { return 0; }
			}
		}
	}
	// rook
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'R')
			{
				positions[positions_size] = i;
				positions_size++;
				positions[positions_size] = j;
				positions_size++;
				int x = Wrook(positions[positions_size - 2], positions[positions_size - 1], board);
				if (x == 0) { return 0; }
			}
		}
	}
	// queen
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'Q')
			{
				positions[positions_size] = i;
				positions_size++;
				positions[positions_size] = j;
				positions_size++;
				int x = Wqueen(positions[positions_size - 2], positions[positions_size - 1], board);
				if (x == 0) { return 0; }
			}
		}
	}
	// king
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'K')
			{
				positions[positions_size] = i;
				positions_size++;
				positions[positions_size] = j;
				positions_size++;
				int x = Wking(positions[positions_size - 2], positions[positions_size - 1], board);
				if (x == 0) { return 0; }
			}
		}
	}
	// knight
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'N')
			{
				positions[positions_size] = i;
				positions_size++;
				positions[positions_size] = j;
				positions_size++;
				int x = Wking(positions[positions_size - 2], positions[positions_size - 1], board);
				if (x == 0) { return 0; }
			}
		}
	}


	return 1;
}
bool check_to_black_king(char board[8][8])
{

	char board2[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board2[i][j] = board[i][j];
		}
	}

	//1st find location of king //
	// 
	//now find is king  under check 
	// 
	//now move every single piece and check will king be in check or not 
	//if after moving a piece the check of king will remove then allow user to enter move 
	//but if after moving every possible move the check will not remove , the current will lose the game



	// moving every possible move and checking will the king be in check or not

	// to find different pawn make a condition in which first it find first pawn and without doing 
	// anything it starts finding another pawn if it finds pawn on same location skip and and move 
	int positions[32], positions_size = 0;

	//pawn
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'p')
			{
				positions[positions_size] = i;
				positions_size++;
				positions[positions_size] = j;
				positions_size++;
				// here call a function to find valid moves for pawn
				int x = Bpawn(positions[positions_size - 2], positions[positions_size - 1], board);
				if (x == 0) { return 0; }
			}
		}
	}
	//bishop
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'b')
			{
				positions[positions_size] = i;
				positions_size++;
				positions[positions_size] = j;
				positions_size++;
				// function call to check every possible move of bishop
				int x = Bbishop(positions[positions_size - 2], positions[positions_size - 1], board);
				if (x == 0) { return 0; }
			}
		}
	}
	// rook
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'r')
			{
				positions[positions_size] = i;
				positions_size++;
				positions[positions_size] = j;
				positions_size++;
				int x = Brook(positions[positions_size - 2], positions[positions_size - 1], board);
				if (x == 0) { return 0; }
			}
		}
	}
	// queen
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'q')
			{
				positions[positions_size] = i;
				positions_size++;
				positions[positions_size] = j;
				positions_size++;
				int x = Bqueen(positions[positions_size - 2], positions[positions_size - 1], board);
				if (x == 0) { return 0; }
			}
		}
	}
	// king
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'k')
			{
				positions[positions_size] = i;
				positions_size++;
				positions[positions_size] = j;
				positions_size++;
				int x = Bking(positions[positions_size - 2], positions[positions_size - 1], board);
				if (x == 0) { return 0; }
			}
		}
	}
	// knight
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'n')
			{
				positions[positions_size] = i;
				positions_size++;
				positions[positions_size] = j;
				positions_size++;
				int x = Bking(positions[positions_size - 2], positions[positions_size - 1], board);
				if (x == 0) { return 0; }
			}
		}
	}


	return 1;
}