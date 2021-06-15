struct Block
{
	char c;
	Block(char c = 0) : c(c) {}
	void Draw(int x, int y, bool shadow = false);
	void Place(Block& b);
	explicit operator bool() { return c; }
};

struct Rotation
{
	Block block[4][4]; // 0
	int x, y; // x 20 y 24
	int height, width;
	Block* operator[](int i) { return block[i]; }
	Rotation(const char* line, int x = 0, int y = 0);
	Rotation(int color, Rotation& r);
};

struct Shape
{
	int color;
	Rotation rot[4];
	Shape(int c, Rotation r0, Rotation r1, Rotation r2, Rotation r3): rot{ {c,r0}, {c,r1}, {c,r2}, {c,r3} } {}
	Rotation& operator[](int i) { return rot[i]; }
};

struct Figure
{
	int type, next, rot, x, y;
	struct Board& board;
	Figure(Board& board);
	void Draw();
	void DrawNext();
	void Place();
	bool CanPlace(int rot, int x, int y);
	bool Move(int dx, int dy);
	void Rotate(int clockwise);
	void Drop();
	Block* BlockInCell(int x, int y, bool& shadow);
	static Shape types[];
	static const int figures;
	int Random();
};