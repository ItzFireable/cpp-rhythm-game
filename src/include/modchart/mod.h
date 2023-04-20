class Mod
{
public:
	void Initialize(float b, float t, char *e, float v, char *m, float s, int c);
	std::string getMod();
	std::string getEasing();
	float beat;
	float time;
	char *easing;
	float value;
	float startValue;
	float targetValue;
	bool isTweening;
	char *mod;
	int column;
};