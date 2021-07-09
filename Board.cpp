#include "pch.h"

Board::Board(int width,
			 int height,
			 int x, 
			 int y, 
			 Control left, 
			 Control right, 
			 Control down, 
			 Control rotateLeft, 
			 Control rotateRight, 
			 Control hardDrop):
	width(width),
	height(height),
	size(width * height),
	x(x), y(y), 
	figure(*this),
	left(left), 
	right(right), 
	down(down), 
	rotateLeft(rotateLeft), 
	rotateRight(rotateRight), 
	hardDrop(hardDrop)
{
	block = new Block[size];
	Clear();
	figure.Drop();
}
Board::~Board()
{
	delete[] block;
}
void Board::Clear()
{
	for( auto& b: *this ) b = 0;
}
void Board::Draw()
{
	for (int r = 0; r < height; r++) {
		SDraw(x, y + r, '³', 7);
		for (int c = 0; c < width; c++) {
			(*this)[r][c].Draw(x + c*2 + 1, y + r);
		}
		SDraw(x + width*2 + 1, y + r, '³', 7);
	}
	SDraw(x, y + height, 'À', 7);
	for (int c = 0; c < width * 2; c++) {
		SDraw(x + c + 1, y + height, 'Ä', 7);
	}
	SDraw(x + width*2 + 1, y + height, 'Ù', 7);
	figure.DrawNext();
}
void Board::ClearLine(int y)
{
	figure.Clear();
	for (int r = y; r > 0; r--) {
		for (int c = 0; c < width; c++) {
			(*this)[r][c] = (*this)[r - 1][c];
		}
	}
	for (int c = 0; c < width; c++) {
		(*this)[0][c] = 0;
	}
	if( figure.Move(0, 1) ) figure.Move(0, -1);
	figure.Reset();
}
void Board::CheckLines()
{
	int linesCleared = 0;
	int cellsRemoved = 0;
	for (int r = 0; r < height; r++) {
		bool full = true;
		for (int c = 0; c < width; c++) {
			if( !(*this)[r][c].Occupied() ) {
				full = false;
				break;
			}
		}
		if (full) {
			ClearLine(r);
			cellsRemoved += width;
			++linesCleared;
		}
	}
	scores += cellsRemoved * linesCleared * level;
	levelLinesCleared += linesCleared;
}
void Board::Continue()
{
	hardDrop.press = 1;
}
void Board::Restart()
{
	Clear();
	frame = 0;
	frameSkip = 20;
	scores = 0;
	level = 1;
	levelLinesCleared = 0;
	Continue();
}
void Board::NextLevel()
{
	frameSkip--;
	if( frameSkip < 1 ) frameSkip = 1;
	frame = 0;
	level++;
	levelLinesCleared = 0;
	Clear();
	figure.Drop();
}
void Board::DrawInfo()
{
	char s[256];
	sprintf_s(s, "frame: %d ", frame % frameSkip);
	SDraw(x + width*2 + 4, y + 1, s);
	sprintf_s(s, "level: %d ", level);
	SDraw(x + width*2 + 4, y + 3, s);
	sprintf_s(s, "score: %d   ", scores);
	SDraw(x + width*2 + 4, y + 5, s);
}
void Board::Sound(const char* name)
{
	char n[128];
	const char* p;
	switch (player) {
	case 1: p = "L"; break;
	case 2: p = "R"; break;
	default: p = ""; break;
	}
	sprintf_s(n, "%s%s.wav", name, p);
	PlaySoundA(n, NULL, SND_FILENAME | SND_ASYNC);
}
void Board::Play()
{
	int dx = 0, dy = 0, rotate = 0;
	if (left) dx--;
	if (right) dx++;
	if (rotateRight) { Sound("Rotate"); rotate++; }
	if (rotateLeft ) { Sound("Rotate"); rotate--; }
	if (down) dy++;
	if (figure.Move(dx, dy, rotate)) figure.Reset();
	if (hardDrop) {
		//Sound("Drop");
		while (figure.Move(0, 1));
		figure.Place();
		figure.Drop();
	}
	if (frame % frameSkip == frameSkip - 1) {
		if (!figure.Move(0, 1)) {
			figure.Place();
			figure.Drop();
		} else {
			figure.Reset();
		}
	}
	CheckLines();
	Draw();
	DrawInfo();
	if (levelLinesCleared >= 5) {
		NextLevel();
	}
	frame++;
}