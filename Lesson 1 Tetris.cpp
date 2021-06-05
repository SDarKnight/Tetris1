#include "pch.h"

HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

typedef char Body[4][5];

struct Rotation
{
	Body body; // 0
	int x, y; // x 20 y 24
	char* operator[](int i) { return body[i]; }
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
	int Height();
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
	Figure figure;
	Board(int rows, int columns, int x, int y);
	~Board();
	char* operator[](int y) { return &block[y * columns]; }
	void Clear();
	void Draw();
	void ClearLine(int y);
	void CheckLines();
	void StartNewLevel();
	void DrawInfo();
};

Figure::Figure(Board& board): board(board) {}
void Figure::Draw()
{
	for (int r = 0; r < 4 && FigureType::types[type][rot][r][0]; r++) {
		for (int c = 0; c < (int)strlen(FigureType::types[type][rot][r]); c++) {
			if (FigureType::types[type][rot][r][c] == 'O') {
				SetConsoleCursorPosition(out, COORD(board.x + x + c + 1, board.y + r + y));
				WriteConsoleA(out, &FigureType::types[type][rot][r][c], 1, 0, 0);
			}
		}
	}
}
void Figure::Place()
{
	for (int r = 0; r < 4 && FigureType::types[type][rot][r][0]; r++) {
		for (int c = 0; c < (int)strlen(FigureType::types[type][rot][r]); c++) {
			if (FigureType::types[type][rot][r][c] != ' ') {
				board[y + r][x + c] = FigureType::types[type][rot][r][c];
			}
		}
	}
}
bool Figure::CanPlace(int rot, int x, int y)
{
	if (y > board.rows - Height()) {
		return false;
	}
	for (int r = 0; r < 4 && FigureType::types[type][rot][r][0]; r++) {
		for (int c = 0; c < (int)strlen(FigureType::types[type][rot][r]); c++) {
			if (FigureType::types[type][rot][r][c] != ' ' && board[y + r][x + c] != ' ') {
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
	int newX = x + (clockwise > 0 ? FigureType::types[type][newRot].x : -FigureType::types[type][rot].y);
	int newY = y + (clockwise > 0 ? FigureType::types[type][newRot].y : -FigureType::types[type][rot].x);
	if (CanPlace(newRot, newX, newY)) rot = newRot, x = newX, y = newY;
	else if (CanPlace(newRot, newX + 1, newY)) rot = newRot, x = newX + 1, y = newY;
	else if (CanPlace(newRot, newX - 1, newY)) rot = newRot, x = newX - 1, y = newY;
	else if (CanPlace(newRot, newX, newY + 1)) rot = newRot, x = newX, y = newY + 1;
	else if (CanPlace(newRot, newX, newY - 1)) rot = newRot, x = newX, y = newY - 1;
}
int Figure::Height()
{
	return !FigureType::types[type][rot][1][0] ? 1 :
		!FigureType::types[type][rot][2][0] ? 2 :
		!FigureType::types[type][rot][3][0] ? 3 :
		4;
}
void Figure::Drop()
{
	type = rand() % 7;
	rot = 0;
	x = strlen(FigureType::types[type][rot][0]) == 2 ? 4 : 3;
	y = 0;
}

Board::Board(int rows, int columns, int x, int y)
	:rows(rows), columns(columns), size(rows* columns), x(x), y(y), figure(*this)
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
	Board p1(15, 12, 10, 1);
	//Board p2(15, 12, 30, 1);
	while (!GetAsyncKeyState(VK_ESCAPE) & 1) {
		if (GetAsyncKeyState(VK_LEFT) & 1 ) p1.figure.Move(-1, 0);
		if (GetAsyncKeyState(VK_RIGHT) & 1 ) p1.figure.Move(1, 0);
		if (GetAsyncKeyState(VK_UP) & 1) p1.figure.Rotate(1);
		if (GetAsyncKeyState('C') & 1) p1.figure.Rotate(-1);
		if (GetAsyncKeyState(VK_DOWN) & 1 ) p1.figure.Move(0, 1);

		if (p1.frame % p1.frameSkip == p1.frameSkip - 1) {
			if( ! p1.figure.Move(0, 1) ){
				p1.figure.Place();
				p1.figure.Drop();
			}
		}
		p1.CheckLines();
		p1.Draw();
		p1.DrawInfo();
		if (p1.levelLinesCleared >= 10) {
			p1.StartNewLevel();
		}
		Sleep(16);
		p1.frame++;
	}
	
	//SetConsoleCursorPosition(out, { 0,15 });
	//WriteConsoleA(out, "\n", 1, 0, 0);
    return 0;
}

/* TODO:
 1. [Done] Сделать подсчёт очков.
 2. Добавить уровни сложности.
 3. [Done] Добавить функцию очистки экрана при заполнении поля или перехода к следующему уровню.
 4. Показывать следующие фигуры и откладывать фигуры.
 5. Разноцветные фигуры.
 6. [Done] Сделать динамический размер игрового поля.
 7. [Done] Коррекция координат при повороте.
 8. Разная вероятность выпадения фигур.
 9. Добавить мультиплеер на одном компьютере.
 10. Добавить игровое меню.
 11. Добавить звук в игре.

 20. Добавить мультиплеер по сети.
 21. Добавить графический режим.
 22. Добавить мобильную версию.
 23. Выложить в Play Market и Steam.
 24. Приготовить мешок и лопату для сбора денег.
 25. Пить коктейль через соломинку лёжа на гамаке в Малибу.
 26. ...
 27. Profit!!!
*/

// 1 line: 10 * 1 * 1 * level = 10 points
// 2 line: 10 * 2 * 2 * level = 40 points
// 3 line: 10 * 3 * 3 * level = 90 points
// 4 line: 10 * 4 * 4 * level = 160 points

