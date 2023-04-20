#include <states/MainMenuState.h>

std::vector<Text *> menuLabels;
std::vector<std::string> menus = {"Play", "Exit"};

Text *menuLabel;
Text *warningLabel;
Text *warningLabel2;
Text *warningLabel3;

Text *songTitle;
GPU_Rect timer_base_rect;
GPU_Rect menu_timer_rect;

int prevSong = -1;

void MainMenuState::Initialize()
{
	background = GPU_LoadImage(con(MENU_PATH, "/bg.jpg"));
	background->w = WIDTH;
	background->h = HEIGHT;

	GPU_SetRGBA(background, 255, 255, 255, 128);
	GPU_SetAnchor(background, 0.0f, 0.0f);

	timer_base_rect.w = 400;
	timer_base_rect.h = 10;
	timer_base_rect.x = window_width - 410;
	timer_base_rect.y = window_height - 20;

	menu_timer_rect.w = 0;
	menu_timer_rect.h = 10;
	menu_timer_rect.x = window_width - 410;
	menu_timer_rect.y = window_height - 20;

	menuLabel = new Text;
	menuLabel->Initialize("RHYTHM GAME TEST", con(FONT_PATH, "vcr.ttf"), 36, 10, 10, {255, 255, 255, 0});
	menuLabel->SetOutline(1, {0, 0, 0, 0});
	menuLabel->Align("right");

	warningLabel = new Text;
	warningLabel->Initialize("This is a test version of the game.", con(FONT_PATH, "vcr.ttf"), 18, 10, 20 + menuLabel->GetHeight(), {255, 255, 255, 0});
	warningLabel->SetOutline(1, {0, 0, 0, 0});
	warningLabel->Align("right");

	warningLabel2 = new Text;
	warningLabel2->Initialize("It is not complete and may not work properly.", con(FONT_PATH, "vcr.ttf"), 18, 10, 20 + menuLabel->GetHeight(), {255, 255, 255, 0});
	warningLabel2->SetOutline(1, {0, 0, 0, 0});
	warningLabel2->Align("right");

	warningLabel3 = new Text;
	warningLabel3->Initialize("Do not press the P key.", con(FONT_PATH, "vcr.ttf"), 18, 10, 20 + menuLabel->GetHeight(), {255, 255, 255, 0});
	warningLabel3->SetOutline(1, {0, 0, 0, 0});
	warningLabel3->Align("right");

	for (int i = 0; i < menus.size(); i++)
	{
		Text *label = new Text;
		label->Initialize(menus[i].c_str(), con(FONT_PATH, "vcr.ttf"), 24, 10, window_height - (34 * (menus.size() - i)), {255, 255, 255, 0});
		label->SetOutline(1, {0, 0, 0, 0});

		if (i == menuSelected)
			label->SetColor({255, 255, 255, 255});
		else
			label->SetColor({150, 150, 150, 255});

		menuLabels.push_back(label);
	}

	// play songs from MENU_PATH/bgMusic
	menuAudios = getFolders(to_string(con(SONG_PATH, "/")));
	for (std::string menuAudio : menuAudios)
	{
		logger->Log(TraceP, con("Found song: ", menuAudio.c_str()));
	}

	// play random song
	randomSong = rand() % menuAudios.size();

	// play song
	songTitle = new Text;
	songTitle->Initialize("Playing song: ", con(FONT_PATH, "vcr.ttf"), 18, 10, window_height - 44, {255, 255, 255, 0});
	songTitle->SetOutline(1, {0, 0, 0, 0});
	songTitle->Align("right");
}

void MainMenuState::HandleEvent(SDL_Event e)
{
	if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_SPACE)
	{
		curState = 1;
	}

	if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_p)
	{
		curState = 3;
	}

	// mouse movement
	if (e.type == SDL_MOUSEMOTION)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		int count = 0;
		for (Text *menuLabel : menuLabels)
		{
			if (menuLabel->IsMouseOver(x, y))
			{
				menuSelected = count;
				menuLabel->SetColor({255, 255, 255, 255});
				break;
			}
			else
				menuLabel->SetColor({150, 150, 150, 255});

			count++;
		}
	}

	// mouse button down
	if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		for (Text *menuLabel : menuLabels)
		{
			if (menuLabel->IsMouseOver(x, y))
			{
				logger->Log(TraceP, con("Mouse pressed over menu label: ", menuLabel->GetText().c_str()));
				if (menuLabel->GetText() == "Play")
				{
					curState = 1;
				}
				else if (menuLabel->GetText() == "Options")
				{
					curState = 4;
				}
				else if (menuLabel->GetText() == "Exit")
				{
					curState = 5;
				}
			}
		}
	}
}

void MainMenuState::Update()
{
	GPU_Blit(background, &texr, framebuffer->target, 0, 0);

	menuLabel->Render(framebuffer->target);
	warningLabel->Render(framebuffer->target);
	warningLabel->SetPosition(10, 20 + menuLabel->GetHeight());

	warningLabel2->Render(framebuffer->target);
	warningLabel2->SetPosition(10, 24 + menuLabel->GetHeight() + warningLabel->GetHeight());

	warningLabel3->Render(framebuffer->target);
	warningLabel3->SetPosition(10, 28 + menuLabel->GetHeight() + warningLabel->GetHeight() + warningLabel2->GetHeight());

	int count = 0;
	for (Text *menuLabel : menuLabels)
	{
		menuLabel->Render(framebuffer->target);
	}

	progress = (float)(Mix_GetMusicPosition(backgroundMusic) / Mix_MusicDuration(backgroundMusic));
	menu_timer_rect.w = (float)(400 * progress);

	GPU_RectangleFilled(framebuffer->target, timer_base_rect.x, timer_base_rect.y, timer_base_rect.x + timer_base_rect.w, timer_base_rect.y + timer_base_rect.h, {0, 0, 0, 255});
	GPU_RectangleFilled(framebuffer->target, menu_timer_rect.x, menu_timer_rect.y, menu_timer_rect.x + menu_timer_rect.w, menu_timer_rect.y + menu_timer_rect.h, {255, 255, 255, 255});

	if (prevSong != randomSong)
	{
		Mix_FreeMusic(backgroundMusic);

		logger->Log(TraceP, con("Playing song: ", menuAudios[randomSong].c_str()));
		songTitle->ChangeText(con("Playing song: ", menuAudios[randomSong].c_str()));

		loadAudio(backgroundMusic, con(con(SONG_PATH, menuAudios[randomSong].c_str()), "/audio.wav"), true, 0, 3);
		prevSong = randomSong;
		SONG_NAME = menuAudios[randomSong];
	}

	songTitle->Render(framebuffer->target);
}

void MainMenuState::Destroy()
{
	for (Text *menuLabel : menuLabels)
	{
		menuLabel->Destroy();
	}

	menuLabels.clear();
	menuLabel->Destroy();
	warningLabel->Destroy();
	warningLabel2->Destroy();
	warningLabel3->Destroy();
	songTitle->Destroy();

	GPU_FreeImage(background);
	Mix_FreeMusic(backgroundMusic);
}

char *MainMenuState::GetName()
{
	return "MainMenu";
}