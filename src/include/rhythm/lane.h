class Lane
{
private:
	GPU_Rect lane_rect;
	Uint8 alpha;
	GPU_Rect offset_rect;

public:
	void Initialize(float w, float h, float transparency);
	void SetAlpha(float transparency);
	void SetPosition(float x, float y);
	void SetOrigin(float x, float y);
	void Render(GPU_Target *target);
};