class Conductor
{
public:
	void Initialize();
	void StepHit(int step);
	/*{
		lua_getglobal(L, "stepHit");
		lua_pushinteger(L, this->step);
		lua_pushnumber(L, this->stepCrochet);
		lua_pcall(L, 2, 0, 0);
	}*/
	void BeatHit(int beat);
	/*{
		lua_getglobal(L, "beatHit");
		lua_pushinteger(L, this->beat);
		lua_pushnumber(L, this->crochet);
		lua_pcall(L, 2, 0, 0);
	}*/
	void Update();
	int GetStep();
	int GetBeat();
	float GetDecimalBeat();
	float GetStepCrochet();
	float GetCrochet();

private:
	int beat;
	int step;

	float crochet;
	float stepCrochet;

	int STEPS_interval;
	int STEPS_start;
	int STEPS_iter;

	int BEATS_interval;
	int BEATS_start;
	int BEATS_iter;

	int nextStep;
	int nextBeat;

	float beatsPerMeasure = 4;
	float stepsPerBeat = 4;
};
