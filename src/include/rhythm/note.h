class Note
{
private:
	Uint8 alpha;
	GPU_Rect pos_rect;
	GPU_Rect rect;
	GPU_Rect offset_rect;
	GPU_Image *note;

	char *cur_texture;
	float rot = 0;

	bool is_created;
	bool rerender;
	bool visible = false;

	Receptor *strumLine;

public:
	int cur;
	bool destroyed;
	float strumTime;
	void Initialize(int, float, Receptor *, float, float);
	void Update(GPU_Target *);
	void Destroy();
};