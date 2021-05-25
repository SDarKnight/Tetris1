#include <iostream>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

char figure[7][4][5] = {
	{
		"OO",  // 0
		"OO"
	},
	{
		"O  ", // 1
		"OOO"
	},
	{
		"  O", // 2
		"OOO"
	},
	{
		"OOOO" // 3
	},
	{
		" O ", // 4
		"OOO"
	},
	{
		" OO", // 5
		"OO "
	},
	{
		"OO ", // 6
		" OO"
	}
};

char screen[10][13] = {
	"|          |",
	"|          |",
	"|          |",
	"|          |",
	"|          |",
	"|          |",
	"|          |",
	"|          |",
	"|          |",
	"------------"
};

HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

void DrawScreen()
{
	for (short r = 0; r < 10; r++) {
		SetConsoleCursorPosition(out, { 10,r });
		WriteConsoleA(out, screen[r], 12, 0, 0);
		//cout << "\t\t" << screen[i] << '\n';
	}
}

void DrawFigure(int n, short x, short y)
{
	for (short r = 0; r < 2; r++) {
		SetConsoleCursorPosition(out, { short(11 + x), short(r + y) } );
		auto f = figure[n][r];
		WriteConsoleA(out, f, strlen(f), 0, 0);
		//cout << "\t\t" << screen[i] << '\n';
	}
}

void ClearScreen()
{
	for (short r = 0; r < 10; r++) {
		SetConsoleCursorPosition(out, { 10,r });
		WriteConsoleA(out, "            ", 12, 0, 0);
	}
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
	//srand((int)time(0));
	//for (int i = 0; i < 60; ++i) {
	//	//cout << rand() % 100 << ' ';
	//	DrawScreen(i);
	//	Sleep(50);
	//	ClearScreen(i);
	//}
	//DrawScreen();
	int x = 0, y = 0;
	int f = 6;
	while (GetAsyncKeyState(VK_ESCAPE) >= 0) {
		if (GetAsyncKeyState(VK_LEFT) < 0) x--;
		if (GetAsyncKeyState(VK_RIGHT) < 0) x++;
		x = clamp(x, 0, 10 - (int)strlen(figure[f][0]));
		if (GetAsyncKeyState(VK_UP) < 0) y--;
		if (GetAsyncKeyState(VK_DOWN) < 0) y++;
		y = clamp(y, 0, 9 - (figure[f][1][0] ? 2 : 1) );
		ClearScreen();
		DrawScreen();
		DrawFigure(f, x, y);
		Sleep(16);
	}
	
	SetConsoleCursorPosition(out, { 0,15 });
	WriteConsoleA(out, "\n", 1, 0, 0);
    return 0;
}
