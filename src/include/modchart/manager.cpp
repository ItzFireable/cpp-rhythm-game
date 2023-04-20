#include <modchart/manager.h>

float scale(float x, float l1, float h1, float l2, float h2)
{
	return (((x) - (l1)) * ((h2) - (l2)) / ((h1) - (l1)) + (l2));
}

float clamp(float val, float min, float max)
{
	if (val < min)
		return min;
	else if (val > max)
		return max;
	return val;
}

float getReverseForCol(int col, Mod *mod, float min, float max)
{
	float val = 0;

	if (mod->getMod() == "reverse")
		val = val + mod->value;
	else if (mod->getMod() == "split" && col > 1)
		val = val + mod->value;
	else if (mod->getMod() == "cross" && (col == 1 || col == 2))
		val = val + mod->value;
	else if (mod->getMod() == "alternate" && col % 2 == 1)
		val = val + mod->value;

	float diff = std::max(max, min) - std::min(min, max);
	if (diff == 0)
		return 0;
	else if (diff == 1)
		return val;
	else
		return (val * diff) - diff / 2;
}

float ModManager::receptorAlpha(int col, float yOffset)
{
	float alp = 1;

	for (Mod *activeMod : activeMods)
	{
		if (activeMod->value == 0)
			continue;
		if (activeMod->getMod() == "alpha")
			alp = alp * activeMod->value;
		else if (activeMod->getMod() == "dark")
			alp = alp * (1 - activeMod->value);
	}

	return alp;
}

float ModManager::arrowAlpha(int col, float yOffset)
{
	float alp = 1;

	for (Mod *activeMod : activeMods)
	{
		if (activeMod->value == 0)
			continue;
		if (activeMod->getMod() == "alpha")
			alp = alp * activeMod->value;
		else if (activeMod->getMod() == "stealth")
			alp = alp * (1 - activeMod->value);
		else if (activeMod->getMod() == "hidden")
		{
			if (yOffset < hiddenoffset && yOffset >= hiddenoffset - 200)
			{
				float hmult = -(yOffset - hiddenoffset) / 200;
				alp = alp * (1 - hmult) * activeMod->value;
			}
			else if (yOffset < hiddenoffset - 100)
				alp = alp * (1 - activeMod->value);
		}
	}

	return alp;
}

std::tuple<float, float, float, float> ModManager::arrowEffects(int col, float yOffset)
{
	float xpos = 0;
	float ypos = 0;
	float rot = 0;
	float rev = 1;

	for (Mod *activeMod : activeMods)
	{
		logger->Log(DebugP, con("Mod: ", activeMod->getMod().c_str()));
		if (activeMod->value == 0)
			continue;
		if (activeMod->column >= 0 && activeMod->column != col)
			continue;

		if (activeMod->getMod() == "beat")
		{
			float fBeatStrength = activeMod->value;

			float fAccelTime = 0.3;
			float fTotalTime = 0.7;
			float fBeat = managerBeat + fAccelTime;

			bool bEvenBeat = false;
			if (fmod(floor(fBeat), 2) != 0)
				bEvenBeat = true;

			fBeat = fBeat - floor(fBeat);
			fBeat = fBeat + 1;
			fBeat = fBeat - floor(fBeat);

			if (fBeat < fTotalTime)
			{

				float fAmount = 0;
				if (fBeat < fAccelTime)
				{
					fAmount = scale(fBeat, 0.0, fAccelTime, 0.0, 1.0);
					fAmount = fAmount * fAmount;
				}
				else
				{
					fAmount = scale(fBeat, fAccelTime, fTotalTime, 1.0, 0.0);
					fAmount = 1 - (1 - fAmount) * (1 - fAmount);
				}

				if (bEvenBeat)
					fAmount = fAmount * -1;

				float fShift = 40.0 * fAmount * sin(((yOffset / 30.0)) + (PI / 2));

				xpos = xpos + fBeatStrength * fShift;
			}
		}
		else if (activeMod->getMod() == "flip")
		{
			float fDistance = ARROW_SIZE * 2 * (1.5 - col);
			xpos = xpos + fDistance * activeMod->value;
		}
		else if (activeMod->getMod() == "confusion")
			rot = rot + activeMod->value;
		else if (activeMod->getMod() == "dizzy")
			rot = rot + activeMod->value * yOffset;
		else if (activeMod->getMod() == "drunk")
			xpos = xpos + activeMod->value * (cos((audio_pos * 1000) * 0.001 + col * (0.2) + 1 * (0.2) + yOffset * (10) / 720) * ARROW_SIZE * 0.5);
		else if (activeMod->getMod() == "tipsy")
			ypos = ypos + activeMod->value * (cos((audio_pos * 1000) * 0.001 * (1.2) + col * (2.0) + 1 * (0.2)) * ARROW_SIZE * 0.4);
		else if (activeMod->getMod() == "adrunk")
			xpos = xpos + activeMod->value * (cos((audio_pos * 1000) * 0.001 + col * (0.2) + 1 * (0.2) + yOffset * (10) / 720) * ARROW_SIZE * 0.5);
		else if (activeMod->getMod() == "atipsy")
			ypos = ypos + activeMod->value * (cos((audio_pos * 1000) * 0.001 * (1.2) + col * (2.0) + 1 * (0.2)) * ARROW_SIZE * 0.4);
		else if (activeMod->getMod() == "invert")
		{
			float fDistance = ARROW_SIZE * (col % 2 == 0 ? 1 : -1);
			xpos = xpos + fDistance * activeMod->value;
		}
		else if (activeMod->getMod() == "movex")
			xpos = xpos + activeMod->value;
		else if (activeMod->getMod() == "amovex")
			xpos = xpos + activeMod->value;
		else if (activeMod->getMod() == "movey")
			ypos = ypos + activeMod->value;
		else if (activeMod->getMod() == "amovey")
			ypos = ypos + activeMod->value;
		else if (activeMod->getMod() == "reverse" || activeMod->getMod() == "split" || activeMod->getMod() == "cross" || activeMod->getMod() == "alternate")
		{
			ypos = ypos - (getReverseForCol(col, activeMod, 0, 1) * 500);
			rev = -getReverseForCol(col, activeMod, -1, 1);
		}
		else
		{
			lua_getglobal(L, activeMod->getMod().c_str());
			lua_pcall(L, 0, 0, 0);
		}
	}

	return {xpos, ypos, rot, rev};
}

void ModManager::me(float beat, float time, char *easing, float value, char *mod, int column)
{
	float startValue = 0;
	for (Mod *oldMod : mods)
	{
		if (oldMod->getMod() == mod)
		{
			startValue = oldMod->targetValue;
			break;
		}
	}

	Mod *m = new Mod();
	m->Initialize(beat, time, easing, value, mod, startValue, column);
	mods.push_back(m);
}

void ModManager::set(float beat, float value, char *mod, int column)
{
	this->me(beat, 0, (char *)"linear", value, mod, column);
}
void ModManager::Update()
{
	managerBeat = conductor->GetDecimalBeat();

	// this loop gets called once per frame
	int mod_id = 0;		// inactive mod id
	for (Mod *m : mods) // loops through mods
	{
		if (m->beat <= managerBeat) // if mods beat is equal or past modmanagers beat
		{
			logger->Log(TraceP, con("[LINE 196] ", to_string(managerBeat).c_str())); // trace current beat
			logger->Log(TraceP, con("[LINE 197] ", to_string(m->beat).c_str()));	 // trace target beat

			int id = 0;						  // new id created inside the loop, reset for every mod automatically
			for (Mod *activeMod : activeMods) // loops through active mods
			{
				if (activeMod->getMod() == m->getMod() && m->beat > activeMod->beat) // compares previous mods to see if there's anything to remove
				{
					activeMods.erase(activeMods.begin() + id); // removes old active mod
					break;									   // breaks the loop
				}
				id++; // increments id if no break
			}
			activeMods.push_back(m);									  // adds inactive mod to active
			logger->Log(TraceP, con("[LINE 211] ", m->getMod().c_str())); // trace the modifier
			mods.erase(mods.begin() + mod_id);							  // removes inactive mod from old array
			mod_id--;													  // reduces inactive mod id by 1 to account for the removed mod
		}
		mod_id++; // increments inactive mod id
	}

	for (Mod *activeMod : activeMods)
	{
		if (activeMod->time > 0 && activeMod->isTweening)
		{
			easing_functions func = stringToEasing(activeMod->getEasing());
			easingFunction easing = getEasingFunction(func);

			float curtime = managerBeat - activeMod->beat;
			float duration = activeMod->time;
			float startstrength = activeMod->startValue;
			float diff = activeMod->targetValue - startstrength;

			activeMod->value = easing(curtime, startstrength, diff, duration);

			if (managerBeat > activeMod->beat + activeMod->time)
			{
				activeMod->value = activeMod->targetValue;
				activeMod->isTweening = false;
			}
		}
		else
		{
			activeMod->value = activeMod->targetValue;
		}
	}

	for (int col = 0; col < 4; col++)
	{
		auto [x, y, rot, rev] = this->arrowEffects(col, 0);
		float alpha = this->receptorAlpha(col, 0) * 255;

		for (Playfield *playfield : playfields)
		{
			Receptor *receptor = playfield->GetReceptors()[col];
			Lane *lane = playfield->GetLanes()[col];

			receptor->SetPosition(x + luaOffset[col][0], y + luaOffset[col][1]);
			receptor->SetRotation(rot);
			receptor->SetAlpha(alpha);
			receptor->direction = rev;

			lane->SetPosition(x, 0);
		}
	}
}