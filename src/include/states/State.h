class State
{
public:
	virtual void Initialize();
	virtual void HandleEvent(SDL_Event e);
	virtual void Update();
	virtual void Destroy();
	virtual void AfterBuffer();
	virtual char *GetName();
};