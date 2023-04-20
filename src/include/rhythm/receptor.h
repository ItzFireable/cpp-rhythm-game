class Receptor
{
public:
	void Initialize(int i, float x, float y, float w, float h);
	void Render(GPU_Target *target);

	void Destroy();
	void SetPosition(int x, int y);
	void SetRotation(float new_rot);
	void SetTexture(char *text);
	void SetAlpha(float new_alpha);
	GPU_Rect GetPosRect();

	int cur;
	float direction = 1.;

private:
	GPU_Image *receptor;
	int w, h;
	float rot;
	float alpha;
	char *cur_texture;

	GPU_Rect pos_rect;
	GPU_Rect rect;
	GPU_Rect offset_rect;

	bool is_created = false;
	bool destroyed = false;
	bool rerender = false;
};