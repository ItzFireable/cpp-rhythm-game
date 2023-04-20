#include <rhythm/receptor.h>

void Receptor::Initialize(int i, float x, float y, float w, float h) //, json config)
{
	cur = i;
	pos_rect = {x, y, 0, 0};
	rect = {0, 0, w, h};
	cur_texture = (char *)"receptor";
	offset_rect = {0, 0, 0, 0};
	alpha = 255;

	receptor = GPU_LoadImage(con(NOTESKIN_PATH, con(NOTESKIN, con(con("/", cur_texture), ".png"))));

	is_created = true;
	rerender = true;
}

void Receptor::Render(GPU_Target *target)
{
	if (this->destroyed || !this->is_created)
		return;

	if (this->rerender == true)
	{
		if (receptor != NULL)
			GPU_FreeImage(receptor);

		receptor = GPU_LoadImage(con(NOTESKIN_PATH, con(NOTESKIN, con(con("/", cur_texture), ".png"))));
		GPU_SetImageFilter(receptor, GPU_FILTER_NEAREST);
		receptor->w = w;
		receptor->h = h;
		if (alpha != 255)
			GPU_SetRGBA(receptor, 255, 255, 255, alpha);

		// GPU_SetAnchor(receptor, 0.0f, 0.0f);
	}

	GPU_BlitRotate(receptor, &rect, target, pos_rect.x + offset_rect.x + (rect.w / 2), pos_rect.y + offset_rect.y + (rect.h / 2), get_rotation(cur) + rot);
	rerender = false;
}
void Receptor::Destroy()
{
	GPU_FreeImage(receptor);
	this->destroyed = true;
}
void Receptor::SetPosition(int x, int y)
{
	offset_rect.x = x;
	offset_rect.y = y;
}
void Receptor::SetRotation(float new_rot)
{
	if (rot == new_rot)
		return;
	rot = new_rot;
}
void Receptor::SetAlpha(float new_alpha)
{
	if (alpha == new_alpha)
		return;
	alpha = new_alpha;
	rerender = true;
}
void Receptor::SetTexture(char *text)
{
	if (cur_texture == text)
		return;
	cur_texture = text;
	rerender = true;
}
GPU_Rect Receptor::GetPosRect()
{
	if (this->destroyed || !this->is_created)
		return {0, 0, 0, 0};

	// combine pos_rect and offset_rect and return it
	return {pos_rect.x + offset_rect.x, pos_rect.y + offset_rect.y, pos_rect.w, pos_rect.h};
}