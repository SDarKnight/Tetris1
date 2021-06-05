#include "pch.h"

HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

typedef char Body[4][5];

struct Rotation
{
	Body body; // 0
	int x, y; // x 20 y 24
	char* operator[](int i) { return body[i]; }
	int Height(){ return !body[1][0] ? 1 : !body[2][0] ? 2 : !body[3][0] ? 3 : 4; }
};

struct FigureType
{
	Rotation rot[4];
	Rotation& operator[](int i) { return rot[i]; }
	static FigureType types[7];
};

FigureType FigureType::types[7]	= {
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

struct Figure
{
	int type, rot, x, y;
	struct Board& board;
	Figure(Board& board);
	void Draw();
	void Place();
	bool CanPlace(int rot, int x, int y);
	bool Move(int dx, int dy);
	void Rotate(int clockwise);
	void Drop();
};

struct Board
{
	char* block;
	int rows, columns, size;
	int x, y;
	int frame = 0;
	int frameSkip = 20;
	int scores = 0;
	int level = 1;
	int levelLinesCleared = 0;
	int left, right, down, rotateLeft, rotateRight;
	Figure figure;
	Board(int rows, int columns, int x, int y, int left, int right, int down, int rotateLeft, int rotateRight);
	~Board();
	char* operator[](int y) { return &block[y * columns]; }
	void Clear();
	void Draw();
	void ClearLine(int y);
	void CheckLines();
	void StartNewLevel();
	void DrawInfo();
	void Play();
};

Figure::Figure(Board& board): board(board) {}
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
	else if (CanPlace(newRot, newX, newY + 1)) rot = newRot, x = newX, y = newY + 1;
	else if (CanPlace(newRot, newX, newY - 1)) rot = newRot, x = newX, y = newY - 1;
	x = clamp(x, 0, board.columns - (int)strlen(f[0]));
}
void Figure::Drop()
{
	type = rand() % 7;
	rot = 0;
	x = strlen(FigureType::types[type][rot][0]) == 2 ? 4 : 3;
	y = 0;
}

Board::Board(int rows, int columns, int x, int y, int left, int right, int down, int rotateLeft, int rotateRight)
	:rows(rows), columns(columns), size(rows* columns), x(x), y(y), figure(*this),
	left(left), right(right), down(down), rotateLeft(rotateLeft), rotateRight(rotateRight)
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
	if (GetAsyncKeyState(left) & 1) figure.Move(-1, 0);
	if (GetAsyncKeyState(right) & 1) figure.Move(1, 0);
	if (GetAsyncKeyState(rotateRight) & 1) figure.Rotate(1);
	if (GetAsyncKeyState(rotateLeft) & 1) figure.Rotate(-1);
	if (GetAsyncKeyState(down) & 1) figure.Move(0, 1);

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

void CursorOff()
{
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = 0;
	SetConsoleCursorInfo(out, &cursorInfo);
}

int main()
{
	CursorOff();
	srand((int)time(0));
	Board players[] = { 
		{15, 12, 2, 1, VK_LEFT, VK_RIGHT, VK_DOWN, VK_RCONTROL, VK_UP},
		{15, 12, 32, 1, 'A', 'D', 'S', VK_LCONTROL, 'W'}
	};
	while (!GetAsyncKeyState(VK_ESCAPE) & 1) {
		for (auto& p : players) {
			p.Play();
		}
		Sleep(16);
	}
	
	//SetConsoleCursorPosition(out, { 0,15 });
	//WriteConsoleA(out, "\n", 1, 0, 0);
    return 0;
}

/* TODO:
 1. [Done] ������� ������� �����.
 2. �������� ������ ���������.
 3. [Done] �������� ������� ������� ������ ��� ���������� ���� ��� �������� � ���������� ������.
 4. ���������� ��������� ������ � ����������� ������.
 5. ������������ ������.
 6. [Done] ������� ������������ ������ �������� ����.
 7. [Done] ��������� ��������� ��� ��������.
 8. ������ ����������� ��������� �����.
 9. �������� ����������� �� ����� ����������.
 10. �������� ������� ����.
 11. �������� ���� � ����.

 20. �������� ����������� �� ����.
 21. �������� ����������� �����.
 22. �������� ��������� ������.
 23. �������� � Play Market � Steam.
 24. ����������� ����� � ������ ��� ����� �����.
 25. ���� �������� ����� ��������� ��� �� ������ � ������.
 26. ...
 27. Profit!!!
*/

// 1 line: 10 * 1 * 1 * level = 10 points
// 2 line: 10 * 2 * 2 * level = 40 points
// 3 line: 10 * 3 * 3 * level = 90 points
// 4 line: 10 * 4 * 4 * level = 160 points

