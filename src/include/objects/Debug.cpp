#include <objects/Debug.h>

void Debug::Init()
{
	debugLabel = new Text();
	debugLabel->Initialize("[DEBUG]", con(FONT_PATH, "vcr.ttf"), 18, 8, 8, {255, 255, 255, 0});
	debugLabel->SetOutline(1, {0, 0, 0, 0});

	deltaTimeLabel = new Text();
	deltaTimeLabel->Initialize("0ms", con(FONT_PATH, "vcr.ttf"), 16, 8, 0, {255, 255, 255, 0});
	deltaTimeLabel->SetOutline(1, {0, 0, 0, 0});

	currentState = new Text();
	currentState->Initialize("State: ", con(FONT_PATH, "vcr.ttf"), 16, 8, 0, {255, 255, 255, 0});
	currentState->SetOutline(1, {0, 0, 0, 0});

	gpuVendor = new Text();
	gpuVendor->Initialize("Vendor: ", con(FONT_PATH, "vcr.ttf"), 16, 8, 0, {255, 255, 255, 0});
	gpuVendor->SetOutline(1, {0, 0, 0, 0});

	gpuData = new Text();
	gpuData->Initialize("GPU: ", con(FONT_PATH, "vcr.ttf"), 16, 8, 0, {255, 255, 255, 0});
	gpuData->SetOutline(1, {0, 0, 0, 0});

	vendorGl = glGetString(GL_VENDOR);
	rendererGl = glGetString(GL_RENDERER);
}

void Debug::Update()
{
	deltaTimeLabel->ChangeText(con("Elapsed: ", con(to_string(deltaTime).c_str(), "ms")));
	currentState->ChangeText(con("State: ", state->GetName()));
	gpuVendor->ChangeText(con("Vendor: ", to_string(vendorGl).c_str()));
	gpuData->ChangeText(con("GPU: ", to_string(rendererGl).c_str()));

	debugLabel->SetPosition(debugOriginRect->x + 4, debugOriginRect->y + 4);
	deltaTimeLabel->SetPosition(debugLabel->GetX(), debugLabel->GetY() + debugLabel->GetHeight() + 4);
	currentState->SetPosition(debugLabel->GetX(), deltaTimeLabel->GetY() + deltaTimeLabel->GetHeight() + 4);
	gpuVendor->SetPosition(debugLabel->GetX(), currentState->GetY() + currentState->GetHeight() + 4);
	gpuData->SetPosition(debugLabel->GetX(), gpuVendor->GetY() + gpuVendor->GetHeight() + 4);

	debugOriginRect->w = gpuData->GetX() + gpuData->GetWidth();
	debugOriginRect->h = gpuData->GetY() + gpuData->GetHeight() - debugOriginRect->y + 4;

	GPU_RectangleFilled(framebuffer->target, debugOriginRect->x, debugOriginRect->y, debugOriginRect->x + debugOriginRect->w, debugOriginRect->y + debugOriginRect->h, {0, 0, 0, 128});

	debugLabel->Render(framebuffer->target);
	deltaTimeLabel->Render(framebuffer->target);
	currentState->Render(framebuffer->target);
	gpuVendor->Render(framebuffer->target);
	gpuData->Render(framebuffer->target);
}

void Debug::Destroy()
{
	debugLabel->Destroy();
	deltaTimeLabel->Destroy();
	currentState->Destroy();
	gpuVendor->Destroy();
	gpuData->Destroy();
}