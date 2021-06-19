#include "pch.h"

void Menu::Draw()
{
	int y = 5, x = 30;
	Screen::cur->Draw(x, ++y, "旼컴컴컴컴컴컴컴컴컴컴컴커");
	Screen::cur->Draw(x, ++y, "�                        �");
	Screen::cur->Draw(x, ++y, "� M E G A    T E T R I S �");
	Screen::cur->Draw(x, ++y, "�                        �");
	for (int i = 0; i < 3; i++) {
		if (auto t = item[i].text){
			Screen::cur->Draw(x, ++y, "�                        �");
			Screen::cur->Draw(x, ++y, i == cur ? "� -> " : "�    ");
			Screen::cur->Draw(x + 5, y, t);
			for (int i = strlen(t); i < 16; i++) Screen::cur->Draw(x + 5 + i, y, ' ');
			Screen::cur->Draw(x + 21, y, i == cur ? " <- �" : "    �");
		}
	}
	Screen::cur->Draw(x, ++y, "�                        �");
	Screen::cur->Draw(x, ++y, "읕컴컴컴컴컴컴컴컴컴컴컴켸");
}

void Menu::Move(int d)
{
	do{
		cur += d;
		if (cur < 0) cur = _countof(item) - 1;
		else if (cur >= _countof(item)) cur = 0;
	}while (!item[cur].action && !item[cur].menuAction);
}

void Menu::Play()
{
	while ( ! quit && !(GetAsyncKeyState(VK_ESCAPE) & 1) ) {
		if (GetAsyncKeyState(VK_RETURN) & 1) {
			if( item[cur].action ) item[cur].action();
			if( item[cur].menuAction ) (this->*item[cur].menuAction)();
		}
		if (GetAsyncKeyState(VK_UP) & 1) Move(-1);
		if (GetAsyncKeyState(VK_DOWN) & 1) Move(1);
		Draw();
		Sleep(16);
	}
}

