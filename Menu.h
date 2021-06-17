struct Menu;

struct MenuItem
{
	const char* text;
	void (*action)();
	void (Menu::*menuAction)();
	MenuItem(const char* text = 0, void (*action)() = 0): text(text), action(action), menuAction(0) {}
	MenuItem(const char* text, void (Menu::* menuAction)()) : text(text), action(0), menuAction(menuAction) {}
};

struct Menu
{
	MenuItem item[3];
	int cur;
	bool quit = false;
	void Draw();
	void Play();
	void Move(int d);
	void Exit() { quit = true;  }
};