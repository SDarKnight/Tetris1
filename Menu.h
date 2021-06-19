struct Menu;

struct MenuItem
{
	const char* text;
	void (*action)();
	void (Menu::*menuAction)();
	MenuItem(const char* text = 0, void (*action)() = 0, void (Menu::* menuAction)() = 0)
		: text(text), action(action), menuAction(menuAction) {}
	MenuItem(const char* text, void (Menu::* menuAction)())
		: text(text), action(0), menuAction(menuAction) {}
};

struct Menu
{
	MenuItem item[3];
	int cur = 0;
	bool quit = false;
	void Draw();
	void Play();
	void Move(int d);
	void Exit() { quit = true;  }
};