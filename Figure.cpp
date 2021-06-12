#include "pch.h"

Rotation::Rotation(const char* line, int x, int y) : x(x), y(y)
{
	int l = 0, maxBlocksInLine = 0;
	line++;
	for (; l < 4 && *line; l++) {
		while (*line == '\t') line++;
		if (!*line) { l--; break; }
		int b = 0;
		for (; b < 4 && *line && *line != '\n'; b++) {
			block[l][b] = *line++;
		}
		if (b > maxBlocksInLine) maxBlocksInLine = b;
		for (; b < 4; b++) {
			block[l][b] = 0;
		}
		if (*line == '\n') {
			line++;
		}
	}
	height = l;
	width = maxBlocksInLine;
}

Figure::Figure(Board& board) : board(board) {}

void Figure::Draw()
{
	auto& f = types[type][rot];
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
	auto& f = types[type][rot];
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
	auto& f = types[type][rot];
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
		x = clamp(x, 0, board.columns - types[type][rot].width);
	}
	return move;
}
void Figure::Rotate(int clockwise)
{
	int newRot = (rot + clockwise) & 3;
	auto& f = types[type][newRot];
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
	unsigned int random;
	_rdrand32_step(&random);
	type = random % figures;
	//type = rand() % figures;
	rot = 0;
	x = types[type][rot].width == 2 ? 4 : 3;
	y = 0;
}