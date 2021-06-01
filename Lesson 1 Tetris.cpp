#include "pch.h"

typedef char Body[4][5];

struct Rotation
{
	Body body; // 0
	int x, y; // x 20 y 24
};

struct Figure
{
	Rotation rot[4];
};

Figure figure[7] = {
	{ { {"OO","OO"}, {"OO","OO"}, {"OO","OO"}, {"OO","OO"} } },					// OO	0
																				// OO
	
	{ { {"O  ","OOO"}, {"OO","O ","O "}, {"OOO","  O"}, {" O"," O","OO"} } },	// O	1
																				// OOO
	
	{ { {"  O","OOO"}, {"O ","O ","OO"}, {"OOO","O  "}, {"OO"," O"," O"} } },	//   O	2
																				// OOO	
	
	{ { {{"OOOO"},-1,1}, {{"O","O","O","O"}, 1, -1}, {{"OOOO"},-1,1}, {{"O","O","O","O"},1,-1} } },		// OOOO	3
	
	{ { {" O ","OOO"}, {"O ","OO","O "}, {"OOO"," O "}, {" O","OO"," O"} } },	//  O	4
																				// OOO
	
	{ { {" OO","OO "}, {"O ","OO"," O"}, {" OO","OO "}, {"O ","OO"," O"} } },	//  OO	5
																				// OO 
	
	{ { {"OO "," OO"}, {" O","OO","O "}, {"OO "," OO"}, {" O","OO","O "} } }	// OO	6
																				//  OO
};

char screen[10][13] = {
	"|          |", // 0
	"|          |", // 1
	"|          |", // 2
	"|          |", // 3
	"|          |", // 4
	"|          |", // 5
	"|          |", // 6
	"|          |", // 7
	"|          |", // 8
	"------------"  // 9
};

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
	for (short r = 0; r < 10; r++) {
		SetConsoleCursorPosition(out, { 10,r });
		WriteConsoleA(out, screen[r], 12, 0, 0);
		//cout << "\t\t" << screen[i] << '\n';
	}
	int f = 1;
	f = 0;
}

void DrawFigure(int num, int rot, short x, short y)
{
	for (int r = 0; r < 4 && figure[num].rot[rot].body[r][0]; r++) {
		for (int c = 0; c < (int)strlen(figure[num].rot[rot].body[r]); c++) {
			if (figure[num].rot[rot].body[r][c] == 'O') {
				SetConsoleCursorPosition(out, { short(11 + x + c), short(r + y) });
				WriteConsoleA(out, &figure[num].rot[rot].body[r][c], 1, 0, 0);
			}
		}
	}
}

void PlaceFigure(int num, int rot, short x, short y)
{
	for (int r = 0; r < 4 && figure[num].rot[rot].body[r][0]; r++) {
		for (int c = 0; c < (int)strlen(figure[num].rot[rot].body[r]); c++) {
			if (figure[num].rot[rot].body[r][c] != ' ') {
				screen[y + r][x + c + 1] = figure[num].rot[rot].body[r][c];
			}
		}
	}
}

int FigureHeight(int f, int rot)
{
	return !figure[f].rot[rot].body[1][0] ? 1 :
		!figure[f].rot[rot].body[2][0] ? 2 :
		!figure[f].rot[rot].body[3][0] ? 3 :
		4;
}


bool CanPlaceFigure(int num, int rot, int x, int y)
{
	if (y >= 10 - FigureHeight(num, rot)) {
		return false;
	}
	for (int r = 0; r < 4 && figure[num].rot[rot].body[r][0]; r++) {
		for (int c = 0; c < (int)strlen(figure[num].rot[rot].body[r]); c++) {
			if (figure[num].rot[rot].body[r][c] != ' ' && screen[y + r][x + c + 1] != ' ') {
				return false;
			}
		}
	}
	return true;
}

void ClearScreen()
{
	for (int r = y; r > 0; r--) {
		for (int c = 1; c < 11; c++) {
			screen[r][c] = ' ';
		}
	}
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
	x = strlen(figure[f].rot[rot].body[0]) == 2 ? 4 : 3;
	y = 0;
	f = 3;
}

void ClearLine(int y)
{
	for (int r = y; r > 0; r--) {
		for (int c = 1; c < 11; c++) {
			screen[r][c] = screen[r - 1][c];
		}
	}
	for (int c = 1; c < 11; c++) {
		screen[0][c] = ' ';
	}
}

void CheckLines()
{
	int linesCleared = 0;
	int cellsRemoved = 0;
	for (int r = 0; r < 9; r++) {
		bool full = true;
		for (int c = 1; c < 11; c++) {
			if (screen[r][c] == ' ') {
				full = false;
				break;
			}
		}
		if (full) {
			ClearLine(r);
			cellsRemoved += 10;
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
	ClearScreen();
	DropNewFigure();
}

void DrawInfo()
{
	SetConsoleCursorPosition(out, { 30, 1 });
	cout << "frame: " << frame % frameSkip << '\t';
	SetConsoleCursorPosition(out, { 30, 3 });
	cout << "level: " << level << '\t';
	SetConsoleCursorPosition(out, { 30, 5 });
	cout << "score: " << scores << '\t';
	SetConsoleCursorPosition(out, { 30, 7 });
	cout << figure[3].rot[1].x << ' ' << figure[3].rot[1].y << '\t';
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
		if (GetAsyncKeyState(VK_UP) & 1 && CanPlaceFigure(f, (rot + 1) & 3, x, y)) {
			rot++;
			rot &= 3;
			x += figure[f].rot[rot].x;
			y += figure[f].rot[rot].y;
		}
		if (GetAsyncKeyState('C') & 1 && CanPlaceFigure(f, (rot - 1) & 3, x, y)) {
			rot--;
			rot &= 3;
			x += figure[f].rot[rot].x;
			y += figure[f].rot[rot].y;
		}
		if (GetAsyncKeyState(VK_DOWN) & 1 && CanPlaceFigure(f, rot, x, y + 1)) y++;

		x = clamp(x, 0, 10 - (int)strlen(figure[f].rot[rot].body[0]));

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
		Sleep(160);
		frame++;
	}
	
	SetConsoleCursorPosition(out, { 0,15 });
	WriteConsoleA(out, "\n", 1, 0, 0);
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

