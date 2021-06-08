struct Control
{
	int code;
	int press;
	int delay;
	Control(int code, int delay = 5) : code(code), delay(delay), press(0) {}
	operator bool()
	{
		press = GetAsyncKeyState(code) < 0 ? press + 1 : 0;
		return press == 1 || delay && press > delay;
	}
};

struct Board
{
	Block* block;
	int rows, columns, size;
	int x, y;
	int frame = 0;
	int frameSkip = 20;
	int scores = 0;
	int level = 1;
	int levelLinesCleared = 0;
	Control left, right, down, rotateLeft, rotateRight, hardDrop;
	Figure figure;
	~Board();
	Board(int rows, int columns, int x, int y, Control left, Control right, Control down, Control rotateLeft, Control rotateRight, Control hardDrop);
	Block* operator[](int y) { return &block[y * columns]; }
	void Clear();
	void Draw();
	void ClearLine(int y);
	void CheckLines();
	void StartNewLevel();
	void DrawInfo();
	void Play();
};