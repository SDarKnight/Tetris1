#include "pch.h"

Board players[] = {
	{18, 12, 1, 1,'A', 'D', 'S', {VK_LCONTROL,0}, {'W',0}, {VK_SPACE,0}},
	{18, 12, 42, 1,  VK_LEFT, VK_RIGHT, VK_DOWN, {VK_RCONTROL,0}, {VK_UP, 0}, {VK_RETURN,0}}
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

int main()
{
	//*(int*)0 = 1;
	//*1 = 2;

	//*(short*)(4337666) = 'rr';
	//*h = 'M';

	//cout << h << endl;
	
	//int ar[5];
	//for (int i = 0; i <= 4; i++) {
	//	cin >> ar[i];
	//}
	//for (int i = 4; i >= 0; i--) {
	//	cout << ar[i] << ' ';
	//}

	//int bv = 123;
	//int& b = *new int(123);
	////b = new int;
	//bv++;
	//b++;
	//int* a = &b;
	//&b;
	//
	//int* a = new int(123);
	//a = new int;
	//a++;
	//*a;
	//int** c = &a;
	//**c = 3;
	//
	//a = incValue(a);
	//incPointer(&a);
	//incReference(a);
	//
	//cout << a << endl;

	//return 0;
	
	//srand((int)time(0));
	//DrawAsciiTable(63, 1);
	while (GetAsyncKeyState(VK_ESCAPE) >= 0) {
		for (auto& p : players) {
			p.Play();
		}
		Sleep(16);
	}
	return 0;
}

/* TODO:
 1. [Done] ������� ������� �����.
 2. [Done] �������� ������ ���������.
 3. [Done] �������� ������� ������� ������ ��� ���������� ���� ��� �������� � ���������� ������.
 4. [Done] ���������� ��������� ������. 
 5. [Done] ������������ ������.
 6. [Done] ������� ������������ ������ �������� ����.
 7. [Done] ��������� ��������� ��� ��������.
 8. [Done] ������ ����������� ��������� �����.
 9. [Done] �������� ����������� �� ����� ����������.
 10. �������� ������� ����.
 11. �������� ���� � ����.

 20. �������� ����������� �� ����.
 21. �������� ����������� �����.
 22. �������� ��������� ������.
 23. �������� � Play Market � Steam.
 24. ����������� ����� � ������ ��� ����� �����.
 25. ���� �������� ����� ��������� ��� �� ������ � ������.
 26. ...
 27. Profit!!!
*/

// 1 line: 10 * 1 * 1 * level = 10 points
// 2 line: 10 * 2 * 2 * level = 40 points
// 3 line: 10 * 3 * 3 * level = 90 points
// 4 line: 10 * 4 * 4 * level = 160 points

