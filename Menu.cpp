#include "pch.h"

void Menu::Draw()
{
	int y = 5, x = 30;
	SDraw(x, ++y, "ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿");
	SDraw(x, ++y, "³                        ³");
	SDraw(x, ++y, "³ M E G A    T E T R I S ³");
	SDraw(x, ++y, "³                        ³");
	for (int i = 0; i < 3; i++) {
		if (auto t = item[i].text){
			SDraw(x, ++y, "³                        ³");
			SDraw(x, ++y, i == cur ? "³ -> " : "³    ");
			SDraw(x + 5, y, t);
			for (int i = strlen(t); i < 16; i++) SDraw(x + 5 + i, y, ' ');
			SDraw(x + 21, y, i == cur ? " <- ³" : "    ³");
		}
	}
	SDraw(x, ++y, "³                        ³");
	SDraw(x, ++y, "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
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
	while ( ! quit  ) {
		if (GetAsyncKeyState(VK_ESCAPE) & 1) {
			quit = true;
		}
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

