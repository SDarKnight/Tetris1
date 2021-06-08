#include "pch.h"

FigureType FigureType::types[] = {
	{ "OO\nOO", "OO\nOO", "OO\nOO", "OO\nOO"},												// OO	0
																							// OO
	{ "O\nOOO", {"OO\nO\nO", 1, 0}, {"OOO\n  O",-1, 1}, {" O\n O\nOO",0,-1} },				// O	1
																							// OOO
	{ "  O\nOOO", {"O \nO \nOO", 1, 0}, {"OOO\nO",-1, 1}, {"OO\n O\n O",0,-1} },			//   O	2
																							// OOO	
	{ Rotation{"OOOO",-1,1}, {"O\nO\nO\nO", 2, -1}, {"OOOO",-2, 2}, {"O\nO\nO\nO",1,-2} },	// OOOO	3
	{ Rotation{"OOOO",-1,1}, {"O\nO\nO\nO", 2, -1}, {"OOOO",-2, 2}, {"O\nO\nO\nO",1,-2} },	// OOOO	3
	{ " O\nOOO", {"O\nOO\nO ", 1, 0}, {"OOO\n O ",-1, 1}, {" O\nOO\n O",0,-1} },			//  O	4
																							// OOO
	{ " OO\nOO ", {"O\nOO\n O", 1, 0}, {" OO\nOO ",-1, 1}, {"O\nOO\n O",0,-1} },			//  OO	5
																							// OO 
	{ "OO\n OO", {" O\nOO\nO ", 1, 0}, {"OO\n OO",-1, 1}, {" O\nOO\nO",0,-1} }				// OO	6
																							//  OO
};

Figure::Figure(Board& board) : board(board) {}

void Figure::Draw()
{
	auto& f = FigureType::types[type][rot];
	for (int r = 0; r < 4 && f[r][0].c; r++) {
		for (int c = 0; c < 4 && f[r][c].c; c++) {
			if (f[r][c].c == 'O') {
				SetConsoleCursorPosition(out, COORD(board.x + x + c + 1, board.y + r + y));
				WriteConsoleA(out, &f[r][c], 1, 0, 0);
			}
		}
	}
}
void Figure::Place()
{
	auto& f = FigureType::types[type][rot];
	for (int r = 0; r < 4 && f[r][0].c; r++) {
		for (int c = 0; c < 4 && f[r][c].c; c++) {
			if (f[r][c].c != ' ') {
				board[y + r][x + c] = f[r][c];
			}
		}
	}
}
bool Figure::CanPlace(int rot, int x, int y)
{
	auto& f = FigureType::types[type][rot];
	if (y > board.rows - f.height) {
		return false;
	}
	for (int r = 0; r < 4 && f[r][0].c; r++) {
		for (int c = 0; c < 4 && f[r][c].c; c++) {
			if (f[r][c].c != ' ' && board[y + r][x + c].c != ' ') {
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
		x = clamp(x, 0, board.columns - FigureType::types[type][rot].width);
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
	x = clamp(x, 0, board.columns - f.width);
}
void Figure::Drop()
{
	type = rand() % _countof(FigureType::types);
	rot = 0;
	x = FigureType::types[type][rot].width == 2 ? 4 : 3;
	y = 0;
}