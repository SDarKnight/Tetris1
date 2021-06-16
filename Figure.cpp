#include "pch.h"

void Block::Draw(int x, int y)
{
	if( *this ){
		int color = c << !shadow * 4;
		Screen::cur->Draw(x, y, shadow ? '°' : '[', color);
		Screen::cur->Draw(x + 1, y, shadow ? '°' : ']', color);
	}else{
		Screen::cur->Draw(x, y, ' ');
		Screen::cur->Draw(x + 1, y, ' ');
	}
}

void Block::Place(Block& b, bool figure, bool shadow)
{
	if( b ){
		c = b.c;
		this->figure = figure;
		this->shadow = shadow;
	}
}

Rotation::Rotation(int color, Rotation& r): Rotation(r)
{
	for (auto& br: block) for (auto& b : br) if( b ) b = color;
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

Figure::Figure(Board& board) : board(board), next(Random()) {}

void Figure::DrawNext()
{
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			types[next][0][r][c].Draw(board.width * 2 + board.x + 5 + c * 2 + 1, board.y + r + 10);
		}
	}
}

void Figure::Place(bool figure, bool shadow)
{
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			board[y + r][x + c].Place( types[type][rot][r][c], figure, shadow );
		}
	}
}

bool Figure::CanPlace(int rot, int x, int y)
{
	auto& f = types[type][rot];
	for( int r = 0; r < 4; r++ ){
		for( int c = 0; c < 4; c++ ){
			if( f[r][c] && board[y + r][x + c].Occupied() ){
				return false;
			}
		}
	}
	return true;
}
bool Figure::Move(int dx, int dy, int rotate)
{
	int newRot = (rot + rotate) & 3;
	auto& f = types[type][newRot];
	int newX = clamp(x + dx + (rotate ? (rotate > 0 ? f.x : -f.y) : 0), 0, board.width - f.width);
	int newY = clamp(y + dy + (rotate ? (rotate > 0 ? f.y : -f.x) : 0), 0, board.height - f.height);
	bool moved = (x != newX || y != newY || rot != newRot) && CanPlace(newRot, newX, newY);
	if( !moved && rotate ){
		int newNewX = clamp(newX + 1, 0, board.width - f.width);
		moved = newNewX != newX && CanPlace(newRot, newNewX, newY);
	}
	if( !moved && rotate ){
		int newNewX = clamp(newX - 1, 0, board.width - f.width);
		moved = newNewX != newX && CanPlace(newRot, newNewX, newY);
	}
	if( !moved && rotate && newY == 0 ){
		int newNewY = clamp(newY + 1, 0, board.height - f.height);
		moved = newNewY != newY && CanPlace(newRot, newX, newNewY);
	}
	if( moved = moved && (x != newX || y != newY || rot != newRot) ){
		x = newX;
		y = newY;
		rot = newRot;
	}
	return moved;
}
void Figure::Drop()
{
	type = next;
	next = Random();
	//type = rand() % figures;
	rot = 0;
	x = (board.width - types[type][rot].width) / 2 ;
	y = 0;
	Reset();
}

void Figure::Clear()
{
	for (auto& b : board) if (b.figure) b = 0;
}

void Figure::Reset()
{
	Clear();
	int movedY = 0;
	while( Move(0, 1) ) movedY++;
	if( movedY > 0 ){
		Place(1, 1);
		y -= movedY;
	}
	Place(1, 0);
}

int Figure::Random()
{
	unsigned short random;
	if (!_rdseed16_step(&random)) _rdrand16_step(&random);
	return random % figures;
}
