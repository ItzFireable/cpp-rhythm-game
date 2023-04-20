#include <rhythm/lane.h>

void Lane::Initialize(float w, float h, float transparency)
{
	lane_rect.w = w;
	lane_rect.h = h;
	alpha = 255 * transparency;
}

void Lane::SetAlpha(float transparency)
{
	alpha = 255 * transparency;
}

void Lane::SetPosition(float x, float y)
{
	offset_rect.x = x;
	offset_rect.y = y;
}

void Lane::SetOrigin(float x, float y)
{
	lane_rect.x = x;
	lane_rect.y = y;
}

void Lane::Render(GPU_Target *target)
{
	GPU_RectangleFilled(target, lane_rect.x + offset_rect.x, lane_rect.y + offset_rect.y, lane_rect.x + offset_rect.x + lane_rect.w, lane_rect.y + offset_rect.y + lane_rect.h, {0, 0, 0, alpha});
}