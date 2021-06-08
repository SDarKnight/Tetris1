struct Block
{
	char c;
	Block(char c = 0) : c(c) {}
};

struct Rotation
{
	Block block[4][4]; // 0
	int x, y; // x 20 y 24
	int height, width;
	Block* operator[](int i) { return block[i]; }
	Rotation(const char* lines, int x = 0, int y = 0) : x(x), y(y)
	{
		int l = 0, maxB = 0;
		for (; l < 4 && *lines; l++) {
			int b = 0;
			for (; b < 4 && *lines && *lines != '\n'; b++) {
				block[l][b] = *lines++;
			}
			if (b > maxB) maxB = b;
			for (; b < 4; b++) {
				block[l][b] = 0;
			}
			if (*lines == '\n') {
				lines++;
			}
		}
		height = l;
		width = maxB;
	}
};

struct FigureType
{
	Rotation rot[4];
	Rotation& operator[](int i) { return rot[i]; }
	static FigureType types[];
};

struct Figure
{
	int type, rot, x, y;
	struct Board& board;
	Figure(Board& board);
	void Draw();
	void Place();
	bool CanPlace(int rot, int x, int y);
	bool Move(int dx, int dy);
	void Rotate(int clockwise);
	void Drop();
};