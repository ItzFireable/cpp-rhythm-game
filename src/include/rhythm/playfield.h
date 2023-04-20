class Playfield
{
private:
	Uint8 alpha;
	bool initialized;
	GPU_Rect offset_rect;
	std::vector<Receptor *> receptors;
	std::vector<Lane *> lanes;

public:
	std::vector<Note *> notes;
	GPU_Rect playfield_rect;
	void Initialize(float w, float h, float transparency, int side);
	void SetAlpha(float transparency);
	void SetPosition(float x, float y);
	void Render(GPU_Target *target);
	void Destroy();
	std::vector<Receptor *> GetReceptors();
	std::vector<Lane *> GetLanes();
};