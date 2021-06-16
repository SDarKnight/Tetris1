struct Block
{
	char c;
	bool figure;
	bool shadow;
	Block(char c = 0) : c(c), figure(0), shadow(0) {}
	explicit operator bool() { return c; }
	bool Occupied() { return c && ! figure; }
	void Draw(int x, int y);
	void Place(Block& b, bool figure = 0, bool shadow = 0);
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
	void DrawNext();
	void Place(bool figure = 0, bool shadow = 0);
	bool CanPlace(int rot, int x, int y);
	bool Move(int dx, int dy, int rotate = 0);
	void Drop();
	void Clear();
	void Reset();
	static Shape types[];
	static const int figures;
	int Random();
};