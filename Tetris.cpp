#include "pch.h"

Board players[] = {
	{15, 12, 2, 1,'A', 'D', 'S', {VK_LCONTROL,0}, {'W',0}, {VK_SPACE,0}},
	{15, 12, 32, 1,  VK_LEFT, VK_RIGHT, VK_DOWN, {VK_RCONTROL,0}, {VK_UP, 0}, {VK_RETURN,0}}
};

void DrawAsciiTable(int startX, int startY)
{
	char buf[16]{ 0 };
	for (int y = 0; y < 16; y++) {
		_itoa_s(y * 16, buf, 10);
		Screen::cur->Draw(startX - 4, startY + y, buf);
		for (int x = 0; x < 16; x++) {
			Screen::cur->Draw(startX + x, startY + y, y * 16 + x);
		}
	}
}

int main()
{
	//srand((int)time(0));
	DrawAsciiTable(63, 1);
	while (GetAsyncKeyState(VK_ESCAPE) >= 0) {
		for (auto& p : players) {
			p.Play();
		}
		Sleep(16);
	}
	return 0;
}

/* TODO:
 1. [Done] Сделать подсчёт очков.
 2. [Done] Добавить уровни сложности.
 3. [Done] Добавить функцию очистки экрана при заполнении поля или перехода к следующему уровню.
 4. Показывать следующие фигуры и откладывать фигуры.
 5. [Done] Разноцветные фигуры.
 6. [Done] Сделать динамический размер игрового поля.
 7. [Done] Коррекция координат при повороте.
 8. [Done] Разная вероятность выпадения фигур.
 9. [Done] Добавить мультиплеер на одном компьютере.
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

