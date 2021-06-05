#include "pch.h"

typedef char Body[4][5];

struct Rotation
{
	Body body; // 0
	int x, y; // x 20 y 24
	char* operator[](int i) { return body[i]; }
};

struct Figure
{
	Rotation rot[4];
	Rotation& operator[](int i) { return rot[i]; }
};

Figure figure[7] = {
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
																									//  OO
};

struct Board
{
	char* block;
	int rows, columns, size;
	int x, y;
	Board(int rows, int columns, int x, int y) :rows(rows), columns(columns), size(rows*columns), x(x), y(y)
	{
		block = new char[rows*columns];
		Clear();
	}
	~Board()
	{
		delete[] block;
	}
	void Clear()
	{
		for (int b = 0; b < size; b++) {
			block[b] = ' ';
		}
	}
	char* operator[](int y) { return &block[y * columns];  }
};

Board p1(15, 12, 10, 1);
int f;
int rot;
int x, y;
int frameSkip = 20;
int frame = 0;
int scores = 0;
int level = 1;
int levelLinesCleared = 0;

HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

void DrawScreen()
{
	for (short r = 0; r < p1.rows; r++) {
		SetConsoleCursorPosition(out, COORD(p1.x, p1.y + r));
		WriteConsoleA(out, "|", 1, 0, 0);
		WriteConsoleA(out, p1[r], p1.columns, 0, 0);
		WriteConsoleA(out, "|", 1, 0, 0);
	}
	SetConsoleCursorPosition(out, COORD(p1.x, p1.y + p1.rows) );
	for( int c = 0; c < p1.columns + 2; c++) WriteConsoleA(out, "-", 1, 0, 0);
	int f = 1;
	f = 0;
}

void DrawFigure(int num, int rot, short x, short y)
{
	for (int r = 0; r < 4 && figure[num][rot][r][0]; r++) {
		for (int c = 0; c < (int)strlen(figure[num][rot][r]); c++) {
			if (figure[num][rot][r][c] == 'O') {
				SetConsoleCursorPosition(out, COORD( p1.x + x + c + 1, p1.y + r + y) );
				WriteConsoleA(out, &figure[num][rot][r][c], 1, 0, 0);
			}
		}
	}
}

void PlaceFigure(int num, int rot, short x, short y)
{
	for (int r = 0; r < 4 && figure[num][rot][r][0]; r++) {
		for (int c = 0; c < (int)strlen(figure[num][rot][r]); c++) {
			if (figure[num][rot][r][c] != ' ') {
				p1[y + r][x + c] = figure[num][rot][r][c];
			}
		}
	}
}

int FigureHeight(int f, int rot)
{
	return !figure[f][rot][1][0] ? 1 :
		!figure[f][rot][2][0] ? 2 :
		!figure[f][rot][3][0] ? 3 :
		4;
}


bool CanPlaceFigure(int num, int rot, int x, int y)
{
	if (y > p1.rows - FigureHeight(num, rot)) {
		return false;
	}
	for (int r = 0; r < 4 && figure[num][rot][r][0]; r++) {
		for (int c = 0; c < (int)strlen(figure[num][rot][r]); c++) {
			if (figure[num][rot][r][c] != ' ' && p1[y + r][x + c] != ' ') {
				return false;
			}
		}
	}
	return true;
}

void CursorOff()
{
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = 0;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void DropNewFigure()
{
	f = rand() % 7;
	rot = 0;
	x = strlen(figure[f][rot][0]) == 2 ? 4 : 3;
	y = 0;
}

void ClearLine(int y)
{
	for (int r = y; r > 0; r--) {
		for (int c = 0; c < p1.columns; c++) {
			p1[r][c] = p1[r - 1][c];
		}
	}
	for (int c = 0; c < p1.columns; c++) {
		p1[0][c] = ' ';
	}
}

void CheckLines()
{
	int linesCleared = 0;
	int cellsRemoved = 0;
	for (int r = 0; r < p1.rows; r++) {
		bool full = true;
		for (int c = 0; c < p1.columns; c++) {
			if (p1[r][c] == ' ') {
				full = false;
				break;
			}
		}
		if (full) {
			ClearLine(r);
			cellsRemoved += p1.columns;
			++linesCleared;
		}
	}
	scores += cellsRemoved * linesCleared * level;
	levelLinesCleared += linesCleared;
}

void StartNewLevel()
{
	frameSkip--;
	frame = 0;
	level++;
	levelLinesCleared = 0;
	p1.Clear();
	DropNewFigure();
}

void DrawInfo()
{
	SetConsoleCursorPosition(out, COORD(p1.x + p1.columns + 5, p1.y + 1));
	cout << "frame: " << frame % frameSkip << '\t';
	SetConsoleCursorPosition(out, COORD(p1.x + p1.columns + 5, p1.y + 3));
	cout << "level: " << level << '\t';
	SetConsoleCursorPosition(out, COORD(p1.x + p1.columns + 5, p1.y + 5));
	cout << "score: " << scores << '\t';
}

void FigureRotate(int f, int clockwise)
{
	int newRot = (rot + clockwise) & 3;
	int newX = x + (clockwise > 0 ? figure[f].rot[newRot].x : -figure[f].rot[rot].y);
	int newY = y + (clockwise > 0 ? figure[f].rot[newRot].y : -figure[f].rot[rot].x);
	if (CanPlaceFigure(f, newRot, newX, newY)) rot = newRot, x = newX, y = newY;
	else if (CanPlaceFigure(f, newRot, newX+1, newY)) rot = newRot, x = newX+1, y = newY;
	else if (CanPlaceFigure(f, newRot, newX-1, newY)) rot = newRot, x = newX-1, y = newY;
	else if (CanPlaceFigure(f, newRot, newX, newY+1)) rot = newRot, x = newX, y = newY+1;
	else if (CanPlaceFigure(f, newRot, newX, newY-1)) rot = newRot, x = newX, y = newY-1;
}

int main()
{
	CursorOff();
	srand((int)time(0));
	//for (int i = 0; i < 60; ++i) {
	//	//cout <<  << ' ';
	//	DrawScreen(i);
	//	Sleep(50);
	//	ClearScreen(i);
	//}
	//DrawScreen();
	DropNewFigure();
	while (!GetAsyncKeyState(VK_ESCAPE) & 1) {
		if (GetAsyncKeyState(VK_LEFT) & 1 && CanPlaceFigure(f, rot, x - 1, y)) x--;
		if (GetAsyncKeyState(VK_RIGHT) & 1 && CanPlaceFigure(f, rot, x + 1, y)) x++;
		if (GetAsyncKeyState(VK_UP) & 1) FigureRotate(f, 1);
		if (GetAsyncKeyState('C') & 1) FigureRotate(f, -1);
		if (GetAsyncKeyState(VK_DOWN) & 1 && CanPlaceFigure(f, rot, x, y + 1)) y++;

		x = clamp(x, 0, p1.columns - (int)strlen(figure[f][rot][0]));

		if (frame % frameSkip == frameSkip - 1) {
			if (CanPlaceFigure(f, rot, x, y + 1)) {
				y++;
			} else {
				PlaceFigure(f, rot, x, y);
				DropNewFigure();
			}
		}
		CheckLines();
		DrawScreen();
		//if( frame & 1 )
		DrawFigure(f, rot, x, y);
		DrawInfo();
		if (levelLinesCleared >= 10) {
			StartNewLevel();
		}
		Sleep(16);
		frame++;
	}
	
	//SetConsoleCursorPosition(out, { 0,15 });
	//WriteConsoleA(out, "\n", 1, 0, 0);
    return 0;
}

/* TODO:
 1. Сделать подсчёт очков.
 2. Добавить уровни сложности.
 3. Добавить функцию очистки экрана при заполнении поля или перехода к следующему уровню.
 4. Показывать следующие фигуры и откладывать фигуры.
 5. Разноцветные фигуры.
 6. Сделать динамический размер игрового поля.
 7. Коррекция координат при повороте.
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

