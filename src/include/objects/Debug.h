class Debug
{
public:
	void Init();
	void Update();
	void Destroy();

	GPU_Rect *debugOriginRect;

private:
	Text *debugLabel;
	Text *deltaTimeLabel;
	Text *currentState;
	Text *gpuVendor;
	Text *gpuData;

	const GLubyte *vendorGl;
	const GLubyte *rendererGl;
};