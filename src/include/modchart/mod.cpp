#include <modchart/mod.h>

void Mod::Initialize(float b, float t, char *e, float v, char *m, float s, int c)
{
	beat = b;
	time = t;
	easing = e;
	value = 0;
	startValue = s;
	targetValue = v;
	mod = m;
	isTweening = true;
	column = c;
}

std::string Mod::getMod()
{
	return to_string(mod);
}

std::string Mod::getEasing()
{
	return to_string(easing);
}