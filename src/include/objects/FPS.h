class FPS
{
public:
	void Init();
	void Update();
	void Destroy();

	GPU_Rect *fpsOriginRect;

private:
	Text *fpsCounter;
	Text *fpsLabel;
	Text *memoryCounter;
	Text *peakMemory;
	Text *gameTitle;
	Text *gameVersion;

	float peakFPS = 0;
	float peakMem = 0;

	PROCESS_MEMORY_COUNTERS_EX pmc;
};