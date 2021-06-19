#include "pch.h"

TextScreen::TextScreen()
{
	out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = 0;
	SetConsoleCursorInfo(out, &cursorInfo);
	SetForegroundWindow(GetActiveWindow());
}

void TextScreen::Draw(short x, short y, char c, int color)
{
	CHAR_INFO ci{ {.AsciiChar = c }, WORD(color ? color : 7) };
	SMALL_RECT r{ x,y,x,y };
	WriteConsoleOutputA(out, &ci, { 1, 1 }, { 0, 0 }, &r);
}

void TextScreen::Draw(short x, short y, const char* c, int color)
{
	SetConsoleCursorPosition(out, { x, y });
	WriteConsoleA(out, c, strlen(c), 0, 0);
}

void TextScreen::Clear()
{
	for (int x = 0; x < 80; x++) for (int y = 0; y < 25; y++) Draw(x, y, ' ');
}

Screen* Screen::cur = new TextScreen();