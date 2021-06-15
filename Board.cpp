#include "pch.h"

Board::Board(int rows, int columns, int x, int y, Control left, Control right, Control down, Control rotateLeft, Control rotateRight, Control hardDrop)
	:rows(rows), columns(columns), size(rows* columns), x(x), y(y), figure(*this),
	left(left), right(right), down(down), rotateLeft(rotateLeft), rotateRight(rotateRight), hardDrop(hardDrop)
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
	for (int b = 0; b < size; b++) {
		block[b] = 0;
	}
}
void Board::Draw()
{
	for (int r = 0; r < rows; r++) {
		Screen::cur->Draw(x, y + r, '³', 7);
		for (int c = 0; c < columns; c++) {
			Block& b = (*this)[r][c];
			Block* f;
			(b ? b : (f = figure.BlockInBoard(c,r)) ? *f : Block(' ')).Draw(x + c + 1, y + r);
		}
		Screen::cur->Draw(x + columns + 1, y + r, '³', 7);
	}
	Screen::cur->Draw(x, y + rows, 'À', 7);
	for (int c = 0; c < columns; c++) Screen::cur->Draw(x + c + 1, y + rows, 'Ä', 7);
	Screen::cur->Draw(x + columns + 1, y + rows, 'Ù', 7);
	figure.DrawNext();
}
void Board::ClearLine(int y)
{
	for (int r = y; r > 0; r--) {
		for (int c = 0; c < columns; c++) {
			(*this)[r][c] = (*this)[r - 1][c];
		}
	}
	for (int c = 0; c < columns; c++) {
		(*this)[0][c] = 0;
	}
}
void Board::CheckLines()
{
	int linesCleared = 0;
	int cellsRemoved = 0;
	for (int r = 0; r < rows; r++) {
		bool full = true;
		for (int c = 0; c < columns; c++) {
			if( ! (*this)[r][c] ) {
				full = false;
				break;
			}
		}
		if (full) {
			ClearLine(r);
			cellsRemoved += columns;
			++linesCleared;
		}
	}
	scores += cellsRemoved * linesCleared * level;
	levelLinesCleared += linesCleared;
}
void Board::StartNewLevel()
{
	frameSkip--;
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
	Screen::cur->Draw(x + columns + 4, y + 1, s);
	sprintf_s(s, "level: %d ", level);
	Screen::cur->Draw(x + columns + 4, y + 3, s);
	sprintf_s(s, "score: %d   ", scores);
	Screen::cur->Draw(x + columns + 4, y + 5, s);
}
void Board::Play()
{
	if (left) figure.Move(-1, 0);
	if (right) figure.Move(1, 0);
	if (rotateRight) figure.Rotate(1);
	if (rotateLeft) figure.Rotate(-1);
	if (down) figure.Move(0, 1);
	if (hardDrop) {
		while (figure.Move(0, 1));
		figure.Place();
		figure.Drop();
	}

	if (frame % frameSkip == frameSkip - 1) {
		if (!figure.Move(0, 1)) {
			figure.Place();
			figure.Drop();
		}
	}
	CheckLines();
	Draw();
	DrawInfo();
	if (levelLinesCleared >= 5) {
		StartNewLevel();
	}
	frame++;
}