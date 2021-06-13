#include "pch.h"

TextScreen::TextScreen()
{
	out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = 0;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void TextScreen::Draw(short x, short y, char c, int color)
{
	SetConsoleCursorPosition(out, { x, y });
	WriteConsoleA(out, &c, 1, 0, 0);
}

void TextScreen::Draw(short x, short y, const char* c, int color)
{
	SetConsoleCursorPosition(out, { x, y });
	WriteConsoleA(out, c, strlen(c), 0, 0);
}

Screen* Screen::cur = new TextScreen();