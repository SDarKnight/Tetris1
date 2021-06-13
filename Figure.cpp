#include "pch.h"

void Block::Draw(int x, int y)
{
	if( *this ) Screen::cur->Draw(x, y, c, color);
}

void Block::Place(Block& b)
{
	if (b) *this = b;
}

Rotation::Rotation(const char* line, int x, int y) : x(x), y(y)
{
	int l = 0, maxBlocksInLine = 0;
	line++;
	for (; l < 4 && *line; l++) {
		while (*line == '\t') line++;
		if (!*line) break;
		int b = 0;
		for (; b < 4 && *line && *line != '\n'; b++) {
			char c = *line++;
			block[l][b] = c == ' ' ? 0 : c;
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
	for (int r = 0; r < 4; r++) {
		if (r + y >= 0) for (int c = 0; c < 4; c++) {
			types[type][rot][r][c].Draw( board.x + x + c + 1, board.y + r + y );
		}
	}
}
void Figure::Place()
{
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			board[y + r][x + c].Place( types[type][rot][r][c] );
		}
	}
}
bool Figure::CanPlace(int rot, int x, int y)
{
	auto& f = types[type][rot];
	if (y > board.rows - f.height) {
		return false;
	}
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			if (y + r >= 0 && f[r][c] && board[y + r][x + c]) {
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
	unsigned short random;
	if( !_rdseed16_step(&random) ) _rdrand16_step(&random);
	type = random % figures;
	//type = rand() % figures;
	rot = 0;
	x = (board.columns - types[type][rot].width) / 2 ;
	y = 0;
}
Block* Figure::BlockInBoard(int boardX, int boardY)
{
	auto& f = types[type][rot];
	if (boardX >= x && boardX < x + f.width && boardY >= y && boardY < y + f.height) {
		Block& b = f[boardY - y][boardX - x];
		if (b) return &b;
	}
	return 0;
}