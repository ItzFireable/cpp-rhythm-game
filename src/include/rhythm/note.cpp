#include <rhythm/note.h>

void Note::Initialize(int i, float time, Receptor *strum, float w, float h)
{
	cur = i;
	pos_rect = {strum->GetPosRect().x, -100, 0, 0};
	rect = {0, 0, w, h};
	cur_texture = (char *)"note";
	offset_rect = {0, 0, 0, 0};
	alpha = 255;

	strumTime = time;
	strumLine = strum;

	note = GPU_LoadImage(con(NOTESKIN_PATH, con(NOTESKIN, con(con("/", cur_texture), ".png"))));

	is_created = true;
	rerender = true;
	destroyed = false;

	// logger->Log(DebugP, con("CREATED NOTE, ID: ", to_string(cur).c_str()));
	// logger->Log(DebugP, con("STRUM TIME: ", to_string(strumTime).c_str()));
}

void Note::Destroy()
{
	if (destroyed)
		return;

	GPU_FreeImage(note);
	this->destroyed = true;
}

void Note::Update(GPU_Target *target)
{
	if (!is_created)
		return;

	if (strumTime <= 0)
	{
		Destroy();
		return;
	}

	if (destroyed)
		return;

	if (rerender)
	{
		if (note != NULL)
			GPU_FreeImage(note);

		note = GPU_LoadImage(con(NOTESKIN_PATH, con(NOTESKIN, con(con("/", cur_texture), ".png"))));
		rerender = false;
	}

	if (note != NULL && visible == true)
	{
		GPU_SetImageFilter(note, GPU_FILTER_NEAREST);
		GPU_SetRGBA(note, 255, 255, 255, alpha);
		GPU_BlitRotate(note, &rect, target, pos_rect.x + offset_rect.x + (rect.w / 2), pos_rect.y + offset_rect.y + (rect.h / 2), get_rotation(cur) + rot);
	}

	if (audio_pos * 1000 - strumTime < 2000)
	{
		visible = true;
		pos_rect.x = strumLine->GetPosRect().x;
		pos_rect.y = strumLine->GetPosRect().y + (audio_pos * 1000 - strumTime) * ((0.45 * strumLine->direction) * 3);
	}
	else
	{
		visible = false;
	}

	if (audio_pos * 1000 > strumTime + judgementWindows[4])
	{
		totalNotes += 1;
		misses += 1;
		curJudgement = 5;
		ms = judgementWindows[4];

		Destroy();
	}
}