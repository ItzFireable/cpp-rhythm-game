#include <rhythm/inputs.h>

void checkInputs(SDL_Keysym key)
{
	for (int i = 0; i < 4; i++)
		if (key.sym == SDL_GetKeyFromName(keybinds[i].c_str()) && pressed[i] == 0)
		{
			for (Playfield *playfield : playfields)
			{
				Receptor *receptor = playfield->GetReceptors()[i];
				receptor->SetTexture((char *)"receptor_glow");
				int pressTime = audio_pos * 1000;

				std::vector<Note *> possibleHits;
				for (int n = 0; n < playfield->notes.size(); n++)
				{
					Note *note = playfield->notes[n];
					if (abs(pressTime - note->strumTime) < judgementWindows[4] && note->cur == receptor->cur && !note->destroyed)
					{
						possibleHits.push_back(note);
					}
				}

				if (possibleHits.size() == 0)
					continue;

				std::sort(possibleHits.begin(), possibleHits.end(), [](Note *a, Note *b)
						  { return a->strumTime < b->strumTime; });
				Note *hit = possibleHits[0];

				for (int j = 0; j < judgementWindows.size(); j++)
				{
					if (abs(pressTime - hit->strumTime) < judgementWindows[j])
					{
						curJudgement = j;
						judgementCount[j]++;
						break;
					}
				}

				notesHit += 1;
				totalNotes += 1;
				ms = pressTime - hit->strumTime;
				logger->Log(DebugP, con("HIT NOTE, MS: ", to_string(ms).c_str()));

				hit->Destroy();
			}
			pressed[i] = 1;
		}
}

void checkReleases(SDL_Keysym key)
{
	for (int i = 0; i < 4; i++)
		if (key.sym == SDL_GetKeyFromName(keybinds[i].c_str()) && pressed[i] == 1)
		{
			for (Playfield *playfield : playfields)
			{
				Receptor *receptor = playfield->GetReceptors()[i];
				receptor->SetTexture((char *)"receptor");
			}
			pressed[i] = 0;
		}
}
