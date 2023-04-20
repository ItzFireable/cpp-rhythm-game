class ModManager
{
public:
	std::vector<Mod *> mods;
	std::vector<Mod *> activeMods;

	float managerBeat = 0;
	float ARROW_SIZE = 100;
	float hiddenoffset = 0;
	std::vector<std::vector<float>> luaOffset = {{0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}};

	float receptorAlpha(int col, float yOffset);
	float arrowAlpha(int col, float yOffset);
	std::tuple<float, float, float, float> arrowEffects(int col, float yOffset);

	void me(float beat, float time, char *easing, float value, char *mod, int column);
	void set(float beat, float value, char *mod, int column);

	void Update();
	void Start();
};