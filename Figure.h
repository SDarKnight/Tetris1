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
	Rotation(const char* line, int x = 0, int y = 0);
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
	static Rotation types[][4];
	static const int figures;
};