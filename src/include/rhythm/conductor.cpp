#include <rhythm/conductor.h>

void Conductor::Initialize()
{
	beat = -1;
	step = -1;

	crochet = ((60.0 / BPM) * 1000.0);
	stepCrochet = crochet / 4.0;

	STEPS_interval = stepCrochet;
	STEPS_start = -1;
	STEPS_iter = 1;

	BEATS_interval = crochet;
	BEATS_start = -1;
	BEATS_iter = 1;

	nextStep = STEPS_start;
	nextBeat = BEATS_start;
}

void Conductor::StepHit(int step)
{
	lua_getglobal(L, "stepHit");
	lua_pushinteger(L, step);
	lua_pushnumber(L, stepCrochet);
	lua_pcall(L, 2, 0, 0);
}

void Conductor::BeatHit(int beat)
{
	lua_getglobal(L, "beatHit");
	lua_pushinteger(L, beat);
	lua_pushnumber(L, crochet);
	lua_pcall(L, 2, 0, 0);

	if (glGetError() != 0)
	{
		logger->Log(ErrorP, con("OpenGL error: ", to_string(glGetError()).c_str()));
		logger->Log(ErrorP, con("OpenGL error: ", GPU_GetShaderMessage()));
	}
}

void Conductor::Update()
{
	if (audio_pos * 1000 >= nextStep)
	{
		step++;
		STEPS_iter++;
		nextStep = STEPS_start + (STEPS_iter * STEPS_interval);

		StepHit(step);
		if (step % 4 == 0)
		{
			beat++;
			BEATS_iter++;
			nextBeat = BEATS_start + (BEATS_iter * BEATS_interval);

			BeatHit(beat);
		}
	}
}

int Conductor::GetStep()
{
	return step;
}

int Conductor::GetBeat()
{
	return beat;
}

float Conductor::GetDecimalBeat()
{
	return audio_pos <= 0 ? 0 : ((audio_pos * 1000) / crochet);
}

float Conductor::GetStepCrochet()
{
	return stepCrochet;
}

float Conductor::GetCrochet()
{
	return crochet;
}