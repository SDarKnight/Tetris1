struct Screen
{
	virtual void Draw(short x, short y, char c, int color = 0) = 0;
	virtual void Draw(short x, short y, const char* c, int color = 0) = 0;
	static Screen* cur;
};

struct TextScreen : Screen
{
	HANDLE out;
	TextScreen();
	void Draw(short x, short y, char c, int color = 0);
	void Draw(short x, short y, const char* c, int color = 0);
};