#include <default/text.h>

void Text::Initialize(const char *text, const char *font_path, int size, float x, float y, SDL_Color color)
{
	this->text = text;
	this->font_path = font_path;

	this->size = size;
	this->dest = {};

	this->pos_dest = {x, y, 0, 0};
	this->outline_pos_dest = {x, y, 0, 0};

	this->color = color;
	this->outline_color = color;

	this->font = TTF_OpenFont(font_path, size);
	this->font_outline = TTF_OpenFont(font_path, size);

	this->is_created = true;
	this->rerender = true;
	this->visible = true;
	this->alpha = 255;
	this->align = false;

	TTF_SetFontHinting(font, TTF_HINTING_LIGHT_SUBPIXEL);
	TTF_SetFontHinting(font_outline, TTF_HINTING_LIGHT_SUBPIXEL);
	this->init = true;

	this->velocity = {0, 0};
	this->acceleration = {0, 0};
	this->maxVelocity = {10000, 10000};
}
void Text::Render(GPU_Target *target)
{
	if (this->destroyed)
		return;

	if (!this->init)
		return;

	if (rerender && text != "")
	{
		if (text_texture != NULL)
			GPU_FreeImage(text_texture);

		if (outline_texture != NULL)
			GPU_FreeImage(outline_texture);

		text_surface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, 0);
		outline_surface = TTF_RenderUTF8_Blended_Wrapped(font_outline, text, outline_color, 0);

		text_texture = GPU_CopyImageFromSurface(text_surface);
		outline_texture = GPU_CopyImageFromSurface(outline_surface);

		dest.h = text_surface->h;
		dest.w = text_surface->w;

		outline_dest.h = outline_surface->h;
		outline_dest.w = outline_surface->w;

		outline_pos_dest.x = pos_dest.x - outline_size;
		outline_pos_dest.y = pos_dest.y - outline_size;

		SDL_FreeSurface(text_surface);
		SDL_FreeSurface(outline_surface);
	}

	if (visible)
	{
		if (!isPaused)
		{
			for (int j = 0; j < velocity.size(); j++)
			{
				float velocityDelta = 0.5 * (vel->computeVelocity(velocity[j], acceleration[j], 0, maxVelocity[j], deltaTime / 1000) - velocity[j]);
				velocity[j] += velocityDelta;
				float delta = velocity[j] * deltaTime / 1000;
				velocity[j] += velocityDelta;

				switch (j)
				{
				case 0:
					pos_dest.x += delta;
					outline_pos_dest.x += delta;
					break;
				case 1:
					pos_dest.y += delta;
					outline_pos_dest.y += delta;
					break;
				}
			}
		}

		if (align == true)
		{
			GPU_Blit(outline_texture, &outline_dest, target, window_width - outline_pos_dest.x + (outline_dest.w / 2) - outline_dest.w, outline_pos_dest.y + outline_dest.h / 2);
			GPU_Blit(text_texture, &dest, target, window_width - pos_dest.x + (dest.w / 2) - dest.w, pos_dest.y + dest.h / 2);
		}
		else
		{
			GPU_Blit(outline_texture, &outline_dest, target, outline_pos_dest.x + outline_dest.w / 2, outline_pos_dest.y + outline_dest.h / 2);
			GPU_Blit(text_texture, &dest, target, pos_dest.x + dest.w / 2, pos_dest.y + dest.h / 2);
		}
	}

	rerender = false;
}
void Text::SetOutline(int size, SDL_Color color)
{
	TTF_SetFontOutline(font_outline, size);

	this->outline_size = size;
	this->outline_color = color;
	this->rerender = true;
}
void Text::SetColor(SDL_Color color)
{
	this->color = color;
	this->rerender = true;
}
void Text::SetVisible(bool visible)
{
	this->visible = visible;
	this->rerender = true;
}
void Text::SetAlpha(int alpha)
{
	outline_color.a = alpha;
	color.a = alpha;

	this->alpha = alpha;
	this->rerender = true;
}
void Text::Align(std::string alignStr)
{
	if (alignStr == "left")
		this->align = false;
	else
		this->align = true;

	this->rerender = true;
}
void Text::Center(bool x, bool y)
{
	if (x)
		pos_dest.x = window_width / 2 - dest.w / 2;
	if (y)
		pos_dest.y = window_height / 2 - dest.h / 2;

	this->rerender = true;
}
void Text::Destroy()
{
	if (text_texture != NULL)
		GPU_FreeImage(text_texture);

	if (outline_texture != NULL)
		GPU_FreeImage(outline_texture);

	TTF_CloseFont(font);
	TTF_CloseFont(font_outline);
}
void Text::ChangeText(const char *newText)
{
	if (strcmp(newText, this->text) == 0)
		return;

	this->text = newText;
	rerender = true;
}
void Text::SetPosition(int x, int y)
{
	if (x == this->pos_dest.x && y == this->pos_dest.y)
		return;

	this->pos_dest.x = x;
	this->pos_dest.y = y;

	rerender = true;
}
bool Text::IsMouseOver(int mouse_x, int mouse_y)
{
	if (this->destroyed)
		return false;

	if (!this->init)
		return false;

	if (this->pos_dest.x < mouse_x && mouse_x < this->pos_dest.x + this->dest.w && this->pos_dest.y < mouse_y && mouse_y < this->pos_dest.y + this->dest.h)
		return true;

	return false;
}
std::string Text::GetText()
{
	return to_string(text);
}
int Text::GetWidth()
{
	return dest.w;
}
int Text::GetHeight()
{
	return dest.h;
}
int Text::GetX()
{
	return pos_dest.x;
}
int Text::GetY()
{
	return pos_dest.y;
}