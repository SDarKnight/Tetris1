#include "pch.h"

Board::Board(int rows, int columns, int x, int y, Control left, Control right, Control down, Control rotateLeft, Control rotateRight, Control hardDrop)
	:rows(rows), columns(columns), size(rows* columns), x(x), y(y), figure(*this),
	left(left), right(right), down(down), rotateLeft(rotateLeft), rotateRight(rotateRight), hardDrop(hardDrop)
{
	block = new char[size];
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
		block[b] = ' ';
	}
}
void Board::Draw()
{
	for (short r = 0; r < rows; r++) {
		SetConsoleCursorPosition(out, COORD(x, y + r));
		WriteConsoleA(out, "|", 1, 0, 0);
		WriteConsoleA(out, (*this)[r], columns, 0, 0);
		WriteConsoleA(out, "|", 1, 0, 0);
	}
	SetConsoleCursorPosition(out, COORD(x, y + rows));
	for (int c = 0; c < columns + 2; c++) WriteConsoleA(out, "-", 1, 0, 0);
	figure.Draw();
}
void Board::ClearLine(int y)
{
	for (int r = y; r > 0; r--) {
		for (int c = 0; c < columns; c++) {
			(*this)[r][c] = (*this)[r - 1][c];
		}
	}
	for (int c = 0; c < columns; c++) {
		(*this)[0][c] = ' ';
	}
}
void Board::CheckLines()
{
	int linesCleared = 0;
	int cellsRemoved = 0;
	for (int r = 0; r < rows; r++) {
		bool full = true;
		for (int c = 0; c < columns; c++) {
			if ((*this)[r][c] == ' ') {
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
	SetConsoleCursorPosition(out, COORD(x + columns + 5, y + 1));
	cout << "frame: " << frame % frameSkip << '\t';
	SetConsoleCursorPosition(out, COORD(x + columns + 5, y + 3));
	cout << "level: " << level << '\t';
	SetConsoleCursorPosition(out, COORD(x + columns + 5, y + 5));
	cout << "score: " << scores << '\t';
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
	if (levelLinesCleared >= 10) {
		StartNewLevel();
	}
	frame++;
}