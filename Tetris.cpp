#include "pch.h"

void DrawAsciiTable(int startX, int startY)
{
	char buf[16]{ 0 };
	for (int y = 0; y < 16; y++) {
		_itoa_s(y * 16, buf, 10);
		SDraw(startX - 4, startY + y, buf);
		for (int x = 0; x < 16; x++) {
			SDraw(startX + x, startY + y, y * 16 + x);
		}
	}
}

//int* a = new int;
//int b = 3;
//int& c = b;
//c = 6;
//cout << *a;
//cout << c;
//cout << b;

//void add(int& b)
//{
//	b += 3;
//}
//
//int b = 1;
//add(b);
//cout << b; // 4
//
//a = &b;
//b = 4;
//*a = 5;
//cout << b; // 5
//cout << *a; // 5
//delete a;

//struct List
//{
//	List* next;
//	int value;
//};

//int a = 1;
//int b = 2;
//int m[1000] = { 0 };
//
//void incReference(int& a)
//{
//	a++;
//}
//
//void incPointer(int* a)
//{
//	(*a)++;
//}
//
//int incValue(int a)
//{
//	a++;
//	return a;
//}

//char h[] = "Hello !";

Board single[] = {
	{12, 18, 20, 1,  VK_LEFT, VK_RIGHT, VK_DOWN, {'C',0}, {VK_UP, 0}, {VK_SPACE,0}}
};

Board split[] = {
	{12, 18, 1, 1,'A', 'D', 'S', {VK_LCONTROL,0}, {'W',0}, {VK_SPACE,0}},
	{12, 18, 42, 1,  VK_LEFT, VK_RIGHT, VK_DOWN, {VK_RCONTROL,0}, {VK_UP, 0}, {VK_RETURN,0}}
};

bool quit = false;

void Exit()
{
	quit = true;
}

template <int n>
void Game(Board (&players)[n])
{
	Screen::cur->Clear();
	for (auto& p : players) p.Restart();
	quit = false;
	while (!quit){
		if (GetAsyncKeyState(VK_ESCAPE) & 1) {
			Menu{ { { "Exit", Exit } } }.Play();
			Screen::cur->Clear();
			for (auto& p : players) p.Continue();
		}
		for (auto& p : players) {
			p.Play();
		}
		Sleep(16);
	}
}

void Single()
{
	Game(single);
}

void Split()
{
	Game(split);
}

int main()
{
	//srand((int)time(0));
	//DrawAsciiTable(63, 1); return 0;
	Menu { { {"New 1 player", Single }, {"New 2 player", Split}, {"Exit", &Menu::Exit} } }.Play();

	return 0;
}

/* TODO:
 1. [Done] Сделать подсчёт очков.
 2. [Done] Добавить уровни сложности.
 3. [Done] Добавить функцию очистки экрана при заполнении поля или перехода к следующему уровню.
 4. [Done] Показывать следующие фигуры. 
 5. [Done] Разноцветные фигуры.
 6. [Done] Сделать динамический размер игрового поля.
 7. [Done] Коррекция координат при повороте.
 8. [Done] Разная вероятность выпадения фигур.
 9. [Done] Добавить мультиплеер на одном компьютере.
 10. [Done] Добавить игровое меню.
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

