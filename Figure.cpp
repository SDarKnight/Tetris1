#include "pch.h"

FigureType FigureType::types[7] = {
	{ { {"OO","OO"}, {"OO","OO"}, {"OO","OO"}, {"OO","OO"} } },										// OO	0
																									// OO
	{ { {"O  ","OOO"}, {{"OO","O ","O "}, 1, 0}, {{"OOO","  O"},-1, 1}, {{" O"," O","OO"},0,-1} } },// O	1
																									// OOO
	{ { {"  O","OOO"}, {{"O ","O ","OO"}, 1, 0}, {{"OOO","O  "},-1, 1}, {{"OO"," O"," O"},0,-1} } },//   O	2
																									// OOO	
	{ { {{"OOOO"},-1,1}, {{"O","O","O","O"}, 2, -1}, {{"OOOO"},-2, 2}, {{"O","O","O","O"},1,-2} } },// OOOO	3

	{ { {" O ","OOO"}, {{"O ","OO","O "}, 1, 0}, {{"OOO"," O "},-1, 1}, {{" O","OO"," O"},0,-1} } },//  O	4
																									// OOO
	{ { {" OO","OO "}, {{"O ","OO"," O"}, 1, 0}, {{" OO","OO "},-1, 1}, {{"O ","OO"," O"},0,-1} } },//  OO	5
																									// OO 
	{ { {"OO "," OO"}, {{" O","OO","O "}, 1, 0}, {{"OO "," OO"},-1, 1}, {{" O","OO","O "},0,-1} } }	// OO	6
};

Figure::Figure(Board& board) : board(board) {}
void Figure::Draw()
{
	auto& f = FigureType::types[type][rot];
	for (int r = 0; r < 4 && f[r][0]; r++) {
		for (int c = 0; c < (int)strlen(f[r]); c++) {
			if (f[r][c] == 'O') {
				SetConsoleCursorPosition(out, COORD(board.x + x + c + 1, board.y + r + y));
				WriteConsoleA(out, &f[r][c], 1, 0, 0);
			}
		}
	}
}
void Figure::Place()
{
	auto& f = FigureType::types[type][rot];
	for (int r = 0; r < 4 && f[r][0]; r++) {
		for (int c = 0; c < (int)strlen(f[r]); c++) {
			if (f[r][c] != ' ') {
				board[y + r][x + c] = f[r][c];
			}
		}
	}
}
bool Figure::CanPlace(int rot, int x, int y)
{
	auto& f = FigureType::types[type][rot];
	if (y > board.rows - f.Height()) {
		return false;
	}
	for (int r = 0; r < 4 && f[r][0]; r++) {
		for (int c = 0; c < (int)strlen(f[r]); c++) {
			if (f[r][c] != ' ' && board[y + r][x + c] != ' ') {
				return false;
			}
		}
	}
	return true;
}
bool Figure::Move(int dx, int dy)
{
	bool move = CanPlace(rot, x + dx, y + dy);
	if (move) {
		x += dx;
		y += dy;
		x = clamp(x, 0, board.columns - (int)strlen(FigureType::types[type][rot][0]));
	}
	return move;
}
void Figure::Rotate(int clockwise)
{
	int newRot = (rot + clockwise) & 3;
	auto& f = FigureType::types[type][newRot];
	int newX = x + (clockwise > 0 ? f.x : -f.y);
	int newY = y + (clockwise > 0 ? f.y : -f.x);
	if (CanPlace(newRot, newX, newY)) rot = newRot, x = newX, y = newY;
	else if (CanPlace(newRot, newX + 1, newY)) rot = newRot, x = newX + 1, y = newY;
	else if (CanPlace(newRot, newX - 1, newY)) rot = newRot, x = newX - 1, y = newY;
	//else if (CanPlace(newRot, newX, newY + 1)) rot = newRot, x = newX, y = newY + 1;
	//else if (CanPlace(newRot, newX, newY - 1)) rot = newRot, x = newX, y = newY - 1;
	x = clamp(x, 0, board.columns - (int)strlen(f[0]));
}
void Figure::Drop()
{
	type = rand() % 7;
	rot = 0;
	x = strlen(FigureType::types[type][rot][0]) == 2 ? 4 : 3;
	y = 0;
}