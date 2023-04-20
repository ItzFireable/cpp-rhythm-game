#include <states/GameState.h>

Text *timer;
Text *paused;
Text *info;
Text *judgementCounter;

GPU_Rect timer_rect;
bool loaded = false;
bool audioPlayed = false;
double startTick;

int count = 0;
int active = 0;
int prevTotal = 0;

int prevJudgement = 0;
Text *judgementPopup;
Text *msPopup;
Mix_Chunk *hitsound;

Playfield *playfield_L;

void GameState::Initialize()
{
	loaded = false;
	audioPlayed = false;
	curJudgement = 0;

	notesHit = 0;
	totalNotes = 0;
	accuracy = 1.;

	timer = new Text;
	paused = new Text;
	info = new Text;

	timer_rect.w = 0;
	timer_rect.h = 10;
	timer_rect.x = 0;
	timer_rect.y = HEIGHT - 10;

	timer->Initialize("0:00 / 0:00", con(FONT_PATH, "vcr.ttf"), 16, 10, window_height - 36, {255, 255, 255, 0});
	paused->Initialize("PAUSED\nPRESS ENTER TO GO BACK TO MENU", con(FONT_PATH, "vcr.ttf"), 32, window_width / 2 - 40, window_height / 2 - 10, {255, 255, 255, 0});
	info->Initialize(con(con("Accuracy: ", to_string(accuracy * 100).c_str()), "% - Misses: 0 - Rating: AAAAA"), con(FONT_PATH, "vcr.ttf"), 18, 10, window_height - 40, {255, 255, 255, 0});

	timer->SetOutline(1, {0, 0, 0, 0});
	paused->SetOutline(1, {0, 0, 0, 0});
	info->SetOutline(1, {0, 0, 0, 0});

	judgementPopup = new Text;
	judgementPopup->Initialize("", con(FONT_PATH, "vcr.ttf"), 32, 0, 0, {255, 255, 255, 0});
	judgementPopup->SetOutline(1, {0, 0, 0, 0});

	msPopup = new Text;
	msPopup->Initialize("", con(FONT_PATH, "vcr.ttf"), 24, 0, 0, {255, 255, 255, 0});
	msPopup->SetOutline(1, {0, 0, 0, 0});

	paused->SetVisible(false);

	paused->Center(true, true);
	info->Center(true, false);

	judgementCounter = new Text;
	judgementCounter->Initialize("", con(FONT_PATH, "vcr.ttf"), 16, 10, 0, {255, 255, 255, 0});
	judgementCounter->SetOutline(1, {0, 0, 0, 0});

	hitsound = Mix_LoadWAV(con(SOUND_PATH, "hitsound.wav"));
	audio_pos = -3;

	playfield_L = new Playfield;
	playfield_L->Initialize(400, HEIGHT, 0.5, 0);
	playfields.push_back(playfield_L);

	background = GPU_LoadImage(con(SONG_PATH, con(SONG_NAME.c_str(), "/background.jpg")));
	background->w = 1280;
	background->h = HEIGHT;

	GPU_SetRGBA(background, 255, 255, 255, 128);
	GPU_SetAnchor(background, 0.0f, 0.0f);

	// Parse data file
	std::vector<std::vector<std::string>> data = parse_data(con(SONG_PATH, con(SONG_NAME.c_str(), "/data.txt")));
	BPM = std::stod(data[0][1]);
	offset = std::stof(data[3][1]);

	for (int i = 0; i < playfield_L->GetReceptors().size(); i++)
	{
		logger->Log(DebugP, con("Receptor ", to_string(i).c_str()));
	}

	// Parse chart file
	std::vector<std::vector<std::string>> noteData = parse_chart(con(SONG_PATH, con(SONG_NAME.c_str(), "/chart.txt")));
	for (int i = 0; i < noteData.size(); i++)
	{
		std::vector<std::string> realNoteData = noteData[i];
		Note *n = new Note;

		// logger->Log(DebugP, con("Added note ", realNoteData[1].c_str()));

		for (int o = 0; o < playfields.size(); o++)
		{
			Playfield *playfield = playfields[o];
			int width = playfield->playfield_rect.w / 4;
			Receptor *rec = playfield->GetReceptors()[std::stof(realNoteData[0])];

			n->Initialize(rec->cur, std::stof(realNoteData[1]) + offset, rec, width, width);
			playfield->notes.push_back(n);
		}
	}

	conductor->Initialize();

	L = luaL_newstate();
	initLua();

	logger->Log(DebugP, "THE GAME IS LOADED");
	loaded = true;

	startTick = SDL_GetTicks();
}

void GameState::HandleEvent(SDL_Event e)
{
	if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
	{
		if (isPaused)
		{
			Mix_ResumeMusic();
			paused->SetVisible(false);
			isPaused = false;
		}
		else
		{
			Mix_PauseMusic();
			paused->SetVisible(true);
			isPaused = true;
		}
	}

	if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_RETURN && isPaused)
	{
		curState = 1;
	}

	else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_F2)
	{
		for (int i = 0; i < shaders.size(); i++)
		{
			const char *frag = frags[i];
			const char *vert = verts[i];

			GPU_FreeShaderProgram(shaders[i]);
			shader_blocks[i] = load_shader_program(&shaders[i], con(SHADER_PATH, vert), con(SHADER_PATH, frag));
		}
	}
	else if (e.type == SDL_KEYDOWN)
	{
		checkInputs(e.key.keysym);
	}
	else if (e.type == SDL_KEYUP)
	{
		checkReleases(e.key.keysym);
	}
}

void updateAcc()
{
	if (prevTotal != totalNotes)
	{
		accuracy = notesHit / (double)totalNotes;
		accuracy = round(accuracy * 1000) / 1000;
		prevTotal = totalNotes;

		char *accLabel = con(con("Accuracy: ", to_string(accuracy * 100).c_str()), "%");
		char *missLabel = con("Misses: ", to_string(misses).c_str());
		char *ratingLabel = "Rating: ?";

		if (accuracy == 1)
			ratingLabel = "Rating: SS";
		else if (accuracy >= 0.95)
			ratingLabel = "Rating: S";
		else if (accuracy >= 0.9)
			ratingLabel = "Rating: A";
		else if (accuracy >= 0.8)
			ratingLabel = "Rating: B";
		else if (accuracy >= 0.7)
			ratingLabel = "Rating: C";
		else if (accuracy >= 0.6)
			ratingLabel = "Rating: D";
		else
			ratingLabel = "Rating: F";

		char *totalString = con(accLabel, " - ");
		totalString = con(totalString, missLabel);
		totalString = con(totalString, " - ");
		totalString = con(totalString, ratingLabel);

		info->ChangeText(totalString);

		char *judgementText = "";
		for (int i = 0; i < judgementNames.size(); i++)
		{
			if (i == judgementNames.size() - 1)
			{
				judgementText = con(judgementText, con(judgementNames[i].c_str(), ": "));
				judgementText = con(judgementText, con(to_string(misses).c_str(), "\n"));
			}
			else
			{
				judgementText = con(judgementText, con(judgementNames[i].c_str(), ": "));
				judgementText = con(judgementText, con(to_string(judgementCount[i]).c_str(), "\n"));
			}
		}

		judgementCounter->ChangeText(judgementText);

		judgementPopup->ChangeText(judgementNames[curJudgement].c_str());
		judgementPopup->SetColor(judgementColors[curJudgement]);

		if (curJudgement != 5)
			Mix_PlayChannel(2, hitsound, 0);

		int y = rand() % 35 + 140;

		judgementPopup->velocity = {0, -y};
		judgementPopup->acceleration = {0, 550};
		judgementPopup->Center(true, true);

		msPopup->ChangeText(con(to_string(ms).c_str(), "ms"));
		int msX = window_width / 2 - msPopup->GetWidth() / 2;

		msPopup->velocity = {0, -y};
		msPopup->acceleration = {0, 550};
		msPopup->SetPosition(msX, judgementPopup->GetY() + judgementPopup->GetHeight() + 10);

		prevJudgement = curJudgement;
	}
}

void GameState::Update()
{
	if (!loaded)
		return;

	count = 0;
	active = 0;

	if (!isPaused)
	{
		if (!audioPlayed && audio_pos >= 0)
		{
			audioPlayed = true;
			loadAudio(backgroundMusic, con(SONG_PATH, con(SONG_NAME.c_str(), "/audio.wav")), true, 0, 3);
		}

		audio_pos += deltaTime / 1000;
	}

	conductor->Update();

	paused->Center(true, true);
	info->Center(true, false);
	judgementCounter->Center(false, true);

	lua_getglobal(L, "update");
	lua_pushnumber(L, deltaTime);
	lua_pushnumber(L, t - startTick);
	lua_pushnumber(L, audio_pos);
	lua_pcall(L, 3, 0, 0);

	count = 0;
	active = 0;

	for (GPU_ShaderBlock shader_block : shader_blocks)
	{
		if (enabled[count] && isBackground[count] && !isOverlay[count])
		{
			Uint32 temp_shader = shaders[count];
			int time_loc = time_uniforms[count];
			int res_loc = res_uniforms[count];

			GPU_ActivateShaderProgram(temp_shader, &shader_block);
			GPU_SetUniformf(time_loc, audio_pos);
			GPU_SetUniformfv(res_loc, 2, 1, res);
			active++;
		}
		count++;
	}

	GPU_Blit(background, &texr, framebuffer->target, 0, 0);

	for (int i = 0; i < active; i++)
	{
		GPU_ActivateShaderProgram(i, NULL);
	}

	count = 0;
	for (GPU_Rect rect : rect_underlays)
	{
		SDL_Color color = color_underlays[count];
		GPU_RectangleFilled2(framebuffer->target, rect, color);
		count++;
	}

	for (Playfield *playfield : playfields)
	{
		playfield->Render(framebuffer->target);
	}

	manager->Update();

	info->Render(framebuffer->target);
	paused->Render(framebuffer->target);
	msPopup->Render(framebuffer->target);
	judgementPopup->Render(framebuffer->target);
	judgementCounter->Render(framebuffer->target);

	updateAcc();

	if (audioPlayed)
	{
		timer->Render(framebuffer->target);
		GPU_RectangleFilled(framebuffer->target, timer_rect.x, timer_rect.y, timer_rect.x + timer_rect.w, timer_rect.y + timer_rect.h, {255, 255, 255, 255});
	}

	for (GPU_Rect flash : flashes)
	{
		flashTimers[count] -= deltaTime / 1000;

		if (flashTimers[count] <= 0)
		{
			flashes.erase(flashes.begin() + count);
			flashColors.erase(flashColors.begin() + count);
			flashTimers.erase(flashTimers.begin() + count);
			flashDurations.erase(flashDurations.begin() + count);
		}
		else
		{
			SDL_Color targetColor = flashColors[count];
			Uint8 targetAlpha = (flashTimers[count] / flashDurations[count]) * 255;
			GPU_RectangleFilled2(framebuffer->target, flash, {targetColor.r, targetColor.g, targetColor.b, targetAlpha});
		}

		count++;
	}

	if (audioPlayed)
	{
		timer->ChangeText(getTimer(Mix_GetMusicPosition(backgroundMusic), Mix_MusicDuration(backgroundMusic)));

		progress = (float)(Mix_GetMusicPosition(backgroundMusic) / Mix_MusicDuration(backgroundMusic));
		timer_rect.w = (float)(window_width * progress);
	}

	count = 0;
	active = 0;
	for (GPU_ShaderBlock shader_block : shader_blocks)
	{
		if (enabled[count] && !isBackground[count] && !isOverlay[count])
		{
			Uint32 temp_shader = shaders[count];
			int time_loc = time_uniforms[count];
			int res_loc = res_uniforms[count];

			GPU_ActivateShaderProgram(temp_shader, &shader_block);
			GPU_SetUniformf(time_loc, audio_pos);
			GPU_SetUniformfv(res_loc, 2, 1, res);
			active++;
		}
		count++;
	}
}

void GameState::AfterBuffer()
{
	for (int i = 0; i < active; i++)
	{
		GPU_ActivateShaderProgram(i, NULL);
	}

	count = 0;
	active = 0;
	for (GPU_ShaderBlock shader_block : shader_blocks)
	{
		if (enabled[count] && !isBackground[count] && isOverlay[count])
		{
			Uint32 temp_shader = shaders[count];
			int time_loc = time_uniforms[count];
			int res_loc = res_uniforms[count];

			GPU_ActivateShaderProgram(temp_shader, &shader_block);
			GPU_SetUniformf(time_loc, audio_pos);
			GPU_SetUniformfv(res_loc, 2, 1, res);
			active++;
		}
		count++;
	}

	GPU_RectangleFilled(renderer, 0, 0, 1280, 720, {0, 0, 0, 0});

	for (int i = 0; i < active; i++)
	{
		GPU_ActivateShaderProgram(i, NULL);
	}
}

void GameState::Destroy()
{
	loaded = false;
	isPaused = false;

	GPU_FreeImage(background);
	Mix_FreeMusic(backgroundMusic);

	timer->Destroy();
	paused->Destroy();
	info->Destroy();
	judgementPopup->Destroy();
	judgementCounter->Destroy();
	Mix_FreeChunk(hitsound);

	judgementCount = {0, 0, 0, 0, 0};
	accuracy = 1.;
	misses = 0;

	lua_close(L);

	float screen_pos_x = screen_width / 2 - WIDTH / 2;
	float screen_pos_y = screen_height / 2 - HEIGHT / 2;
	SDL_SetWindowPosition(window, screen_pos_x, screen_pos_y);

	for (Playfield *playfield : playfields)
	{
		playfield->Destroy();
	}
	playfields.clear();

	count = 0;
	for (GPU_ShaderBlock shader_block : shader_blocks)
	{
		GPU_FreeShaderProgram(shaders[count]);
		count++;
	}

	delete playfield_L;
}

char *GameState::GetName()
{
	return "GameState";
}