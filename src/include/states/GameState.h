class GameState : public State
{
public:
	void Initialize();
	void HandleEvent(SDL_Event e);
	void Update();
	void Destroy();
	void AfterBuffer();
	char *GetName();
};