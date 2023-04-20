class SongSelectState : public State
{
public:
	void Initialize();
	void HandleEvent(SDL_Event e);
	void Update();
	void Destroy();
	char *GetName();
};