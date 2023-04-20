class Text
{
public:
	void Initialize(const char *text, const char *font_path, int size, float x, float y, SDL_Color color);
	void Render(GPU_Target *target);
	void SetOutline(int size, SDL_Color color);
	void SetPosition(int x, int y);
	void SetColor(SDL_Color color);
	void SetVisible(bool visible);
	void SetAlpha(int alpha);
	void Center(bool x, bool y);
	void Destroy();
	void Align(std::string alignStr);
	void ChangeText(const char *text);
	bool IsMouseOver(int mouse_x, int mouse_y);
	std::string GetText();
	int GetWidth();
	int GetHeight();
	int GetX();
	int GetY();

	std::vector<int> velocity;
	std::vector<int> acceleration;
	std::vector<int> maxVelocity;

private:
	const char *text;
	const char *font_path;

	int alpha;
	bool visible;
	bool init = false;
	bool align = false;

	int size;
	int outline_size;

	GPU_Rect pos_dest;
	GPU_Rect outline_pos_dest;

	GPU_Rect dest;
	GPU_Rect outline_dest;

	SDL_Surface *text_surface = nullptr;
	SDL_Surface *outline_surface = nullptr;

	GPU_Image *text_texture = nullptr;
	GPU_Image *outline_texture = nullptr;

	SDL_Color color;
	SDL_Color outline_color;

	TTF_Font *font;
	TTF_Font *font_outline;

	bool is_created = false;
	bool destroyed = false;
	bool rerender = false;
};
