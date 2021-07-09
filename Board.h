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
	int height, width, size;
	int x, y;
	int frame;
	int frameSkip;
	int scores;
	int level;
	int levelLinesCleared;
	Control left, right, down, rotateLeft, rotateRight, hardDrop;
	Figure figure;
	int player;
	~Board();
	Board(int width, int height, int x, int y, Control left, Control right, Control down, Control rotateLeft, Control rotateRight, Control hardDrop);
	Block* operator[](int y) { return &block[y * width]; }
	Block* begin() { return block; }
	Block* end() { return block + size; }
	void Clear();
	void Draw();
	void ClearLine(int y);
	void CheckLines();
	void Continue();
	void Restart();
	void NextLevel();
	void DrawInfo();
	void Sound(const char* name);
	void Play();
};