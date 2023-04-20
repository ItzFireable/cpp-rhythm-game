#include <states/SongSelectState.h>

// vector of Text
std::vector<Text *> songLabels;
int curSelected = 0;
bool init = false;

Text *bpmLabel;
Text *artistLabel;
Text *charterLabel;

// cache backgrounds
std::vector<GPU_Image *> backgrounds;
std::vector<std::vector<std::vector<std::string>>> chartData;

void updateBackground()
{
	background = backgrounds[curSelected];
	background->w = WIDTH;
	background->h = HEIGHT;

	GPU_SetRGBA(background, 255, 255, 255, 128);
	GPU_SetAnchor(background, 0.0f, 0.0f);
}

void updateData()
{
	std::vector<std::vector<std::string>> chart = chartData[curSelected];

	bpmLabel->ChangeText(con("BPM: ", chart[0][1].c_str()));
	artistLabel->ChangeText(con("Artist: ", chart[1][1].c_str()));
	charterLabel->ChangeText(con("Charter: ", chart[2][1].c_str()));
}

void SongSelectState::Initialize()
{
	Mix_HaltMusic();

	SONG_NAME = songs[curSelected];

	for (int i = 0; i < songs.size(); i++)
	{
		Text *label = new Text;
		label->Initialize(songs[i].c_str(), con(FONT_PATH, "vcr.ttf"), 18, 10, window_height - (28 * (songs.size() - i)), {255, 255, 255, 0});
		label->SetOutline(1, {0, 0, 0, 0});

		songLabels.push_back(label);

		GPU_Image *bg = GPU_LoadImage(con(SONG_PATH, con(songs[i].c_str(), "/background.jpg")));
		backgrounds.push_back(bg);
	}

	bpmLabel = new Text;
	bpmLabel->Initialize("BPM: 0", con(FONT_PATH, "vcr.ttf"), 18, 10, 10, {255, 255, 255, 0});
	bpmLabel->SetOutline(1, {0, 0, 0, 0});
	bpmLabel->Align("right");

	artistLabel = new Text;
	artistLabel->Initialize("Artist: ", con(FONT_PATH, "vcr.ttf"), 18, 10, 10, {255, 255, 255, 0});
	artistLabel->SetOutline(1, {0, 0, 0, 0});
	artistLabel->Align("right");

	charterLabel = new Text;
	charterLabel->Initialize("Charter: ", con(FONT_PATH, "vcr.ttf"), 18, 10, 10, {255, 255, 255, 0});
	charterLabel->SetOutline(1, {0, 0, 0, 0});
	charterLabel->Align("right");

	for (int i = 0; i < songs.size(); i++)
	{
		std::vector<std::vector<std::string>> chartDataForSong;
		char *path = con(SONG_PATH, con(songs[i].c_str(), "/data.txt"));

		chartDataForSong = parse_data(path);
		chartData.push_back(chartDataForSong);

		logger->Log(DebugP, con("Loaded chart data for ", path));
	}

	updateData();
	updateBackground();
	init = true;
}

void SongSelectState::HandleEvent(SDL_Event e)
{
	if (e.type == SDL_MOUSEMOTION)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		int count = 0;
		for (Text *label : songLabels)
		{
			if (label->IsMouseOver(x, y))
			{
				curSelected = count;
				SONG_NAME = songs[curSelected];
				updateBackground();
				updateData();
				break;
			}

			count++;
		}
	}

	if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_RETURN)
	{
		curState = 2;
	}
	else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
	{
		curState = 0;
	}

	if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		for (Text *label : songLabels)
		{
			if (label->IsMouseOver(x, y))
			{
				logger->Log(TraceP, con("Mouse pressed over song label: ", label->GetText().c_str()));
				curState = 2;
			}
		}
	}
}

void SongSelectState::Update()
{
	GPU_Blit(background, &texr, framebuffer->target, 0, 0);

	if (init)
	{
		int count = 0;
		for (Text *songTitle : songLabels)
		{
			if (count == curSelected)
			{
				songTitle->SetColor({255, 255, 255, 255});
				songTitle->SetPosition(20, window_height - (28 * (songs.size() - count)));
			}
			else
			{
				songTitle->SetColor({150, 150, 150, 255});
				songTitle->SetPosition(10, window_height - (28 * (songs.size() - count)));
			}

			// songTitle->Center(true, true);
			songTitle->Render(framebuffer->target);
			count++;
		}
	}

	bpmLabel->SetPosition(10, 10);
	bpmLabel->Render(framebuffer->target);

	artistLabel->SetPosition(10, bpmLabel->GetHeight() + 12);
	artistLabel->Render(framebuffer->target);

	charterLabel->SetPosition(10, bpmLabel->GetHeight() + artistLabel->GetHeight() + 14);
	charterLabel->Render(framebuffer->target);

	// int count = 0;
	// for (Text* songTitle : songLabels)
	// {
	// 	if (count == curSelected)
	// 		songTitle->SetPosition(20, window_height - (40 * count));
	// 	else
	// 		songTitle->SetPosition(10, window_height - (40 * count));

	// 	songTitle->Render(framebuffer->target);
	// 	count++;
	// }
}

void SongSelectState::Destroy()
{
	for (Text *label : songLabels)
	{
		label->Destroy();
	}

	songLabels.clear();
	for (GPU_Image *bg : backgrounds)
	{
		GPU_FreeImage(bg);
	}
	backgrounds.clear();

	bpmLabel->Destroy();
	artistLabel->Destroy();
	charterLabel->Destroy();

	for (std::vector<std::vector<std::string>> chart : chartData)
	{
		chart.clear();
	}
	chartData.clear();
}

char *SongSelectState::GetName()
{
	return "SongSelect";
}