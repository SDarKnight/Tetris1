typedef char Body[4][5];

struct Rotation
{
	Body body; // 0
	int x, y; // x 20 y 24
	char* operator[](int i) { return body[i]; }
	int Height() { return !body[1][0] ? 1 : !body[2][0] ? 2 : !body[3][0] ? 3 : 4; }
};

struct FigureType
{
	Rotation rot[4];
	Rotation& operator[](int i) { return rot[i]; }
	static FigureType types[7];
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