#include <objects/FPS.h>

void FPS::Init()
{
	fpsCounter = new Text();
	fpsCounter->Initialize("0", con(FONT_PATH, "vcr.ttf"), 18, 8, 8, {255, 255, 255, 0});
	fpsCounter->SetOutline(1, {0, 0, 0, 0});

	fpsLabel = new Text();
	fpsLabel->Initialize("FPS", con(FONT_PATH, "vcr.ttf"), 14, 8, 0, {255, 255, 255, 0});
	fpsLabel->SetOutline(1, {0, 0, 0, 0});

	memoryCounter = new Text();
	memoryCounter->Initialize("0MB", con(FONT_PATH, "vcr.ttf"), 16, 8, 0, {255, 255, 255, 0});
	memoryCounter->SetOutline(1, {0, 0, 0, 0});

	peakMemory = new Text();
	peakMemory->Initialize(" / 0MB", con(FONT_PATH, "vcr.ttf"), 16, 8, 0, {128, 128, 128, 0});
	peakMemory->SetOutline(1, {0, 0, 0, 0});

	gameTitle = new Text();
	gameTitle->Initialize(GAME_NAME, con(FONT_PATH, "vcr.ttf"), 16, 8, 0, {255, 255, 255, 0});
	gameTitle->SetOutline(1, {0, 0, 0, 0});

	gameVersion = new Text();
	gameVersion->Initialize(GAME_VERSION, con(FONT_PATH, "vcr.ttf"), 16, 8, 0, {255, 255, 255, 0});
	gameVersion->SetOutline(1, {0, 0, 0, 0});
}

void FPS::Update()
{
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));

	framerate = 1000.0 / (float)deltaTime;
	// logger->Log(TraceP, con("FPS: ", to_string(ceil(framerate - 0.5)).c_str()));

	if (ceil(framerate - 0.5) > peakFPS)
		peakFPS = ceil(framerate - 0.5);

	if (pmc.WorkingSetSize / 1024 / 1024 > peakMem)
		peakMem = pmc.WorkingSetSize / 1024 / 1024;

	std::string framerateString = to_string(ceil(framerate - 0.5));

	fpsCounter->ChangeText(con("", framerateString.c_str()));
	memoryCounter->ChangeText(con(to_string(pmc.WorkingSetSize / 1024 / 1024).c_str(), "MB"));
	peakMemory->ChangeText(con(con(" / ", to_string(peakMem).c_str()), "MB"));

	fpsCounter->SetPosition(fpsOriginRect->x + 4, fpsOriginRect->y + 4);
	fpsLabel->SetPosition(fpsCounter->GetX() + fpsCounter->GetWidth() + 4, fpsCounter->GetY() + fpsCounter->GetHeight() - fpsLabel->GetHeight());
	memoryCounter->SetPosition(fpsCounter->GetX(), fpsCounter->GetY() + fpsCounter->GetHeight() + 4);
	peakMemory->SetPosition(memoryCounter->GetX() + memoryCounter->GetWidth(), memoryCounter->GetY());
	gameTitle->SetPosition(memoryCounter->GetX(), memoryCounter->GetY() + memoryCounter->GetHeight() + 8);
	gameVersion->SetPosition(gameTitle->GetX(), gameTitle->GetY() + gameTitle->GetHeight() + 2);

	fpsOriginRect->w = gameTitle->GetX() + gameTitle->GetWidth();
	fpsOriginRect->h = gameVersion->GetY() + gameVersion->GetHeight() - fpsOriginRect->y + 4;

	GPU_RectangleFilled(framebuffer->target, fpsOriginRect->x, fpsOriginRect->y, fpsOriginRect->x + fpsOriginRect->w, fpsOriginRect->y + fpsOriginRect->h, {0, 0, 0, 128});
	fpsLabel->Render(framebuffer->target);
	fpsCounter->Render(framebuffer->target);
	memoryCounter->Render(framebuffer->target);
	peakMemory->Render(framebuffer->target);
	gameTitle->Render(framebuffer->target);
	gameVersion->Render(framebuffer->target);
}

void FPS::Destroy()
{
	fpsLabel->Destroy();
	fpsCounter->Destroy();
}