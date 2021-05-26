#include <iostream>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

char figure[7][4][4][5] = {
	{ {"OO","OO"}, {"OO","OO"}, {"OO","OO"}, {"OO","OO"} },					// OO	0
																			// OO

	{ {"O  ","OOO"}, {"OO","O ","O "}, {"OOO","  O"}, {" O"," O","OO"} },	// O	1
																			// OOO

	{ {"  O","OOO"}, {"O ","O ","OO"}, {"OOO","O  "}, {"OO"," O"," O"} },	//   O	2
																			// OOO	

	{ {	"OOOO"}, {"O","O","O","O"}, {"OOOO"}, {"O","O","O","O"} },			// OOOO	3

	{ {" O ","OOO"}, {"O ","OO","O "}, {"OOO"," O "}, {" O","OO"," O"} },	//  O	4
																			// OOO

	{ {" OO","OO "}, {"O ","OO"," O"}, {" OO","OO "}, {"O ","OO"," O"} },	//  OO	5
																			// OO 
	
	{ {"OO "," OO"}, {" O","OO","O "}, {"OO "," OO"}, {" O","OO","O "} }	// OO	6
																			//  OO
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
	"| O        |",
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

void DrawFigure(int n, int rot, short x, short y)
{
	for (short r = 0; r < 4 && figure[n][rot][r][0]; r++) {
		SetConsoleCursorPosition(out, { short(11 + x), short(r + y) } );
		auto f = figure[n][rot][r];
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
	srand((int)time(0));
	//for (int i = 0; i < 60; ++i) {
	//	//cout <<  << ' ';
	//	DrawScreen(i);
	//	Sleep(50);
	//	ClearScreen(i);
	//}
	//DrawScreen();
	int f = rand() % 7;
	int rot = 0;
	int len = strlen(figure[f][rot][0]);
	int x = len == 2 ? 4 : 3, y = 0;
	int i = 0;
	while (GetAsyncKeyState(VK_ESCAPE) >= 0) {
		len = strlen(figure[f][rot][0]);
		int h = !figure[f][rot][1][0] ? 1 :
			    !figure[f][rot][2][0] ? 2 :
				!figure[f][rot][3][0] ? 3 :
										4 ;
		if (GetAsyncKeyState(VK_LEFT) < 0) x--;
		if (GetAsyncKeyState(VK_RIGHT) < 0) x++;
		x = clamp(x, 0, 10 - len);
		if (GetAsyncKeyState(VK_UP) & 1) rot++;
		if (GetAsyncKeyState('C') & 1) rot--;
		rot &= 3;

		if (GetAsyncKeyState(VK_DOWN) < 0) y++;
		y = clamp(y, 0, 9 - h);
		ClearScreen();
		DrawScreen();
		DrawFigure(f, rot, x, y);
		Sleep(16);
		if( i % 10 == 9) y++;
		i++;
	}
	
	SetConsoleCursorPosition(out, { 0,15 });
	WriteConsoleA(out, "\n", 1, 0, 0);
    return 0;
	
}
