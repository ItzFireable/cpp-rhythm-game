#include <rhythm/playfield.h>

void Playfield::Initialize(float w, float h, float transparency, int side)
{
	initialized = false;
	playfield_rect.x = (side == 0 ? 640 : (side == 1 ? 320 : 960)) - (w / 2);
	playfield_rect.w = w;
	playfield_rect.h = h;

	for (int i = 0; i < 4; i++)
	{
		float width = playfield_rect.w / 4;

		Lane *lane = new Lane;
		lane->Initialize(width, HEIGHT * 2, transparency);
		lanes.push_back(lane);

		Receptor *receptor = new Receptor;
		receptor->Initialize(i, playfield_rect.x + width * i, h - width - 60, width, width);
		receptors.push_back(receptor);
	}
	initialized = true;
}

void Playfield::SetAlpha(float transparency)
{
	for (Lane *lane : lanes)
	{
		lane->SetAlpha(transparency);
	}
}

void Playfield::SetPosition(float x, float y)
{
	for (Receptor *receptor : receptors)
	{
		receptor->SetPosition(x, y);
	}

	for (Lane *lane : lanes)
	{
		lane->SetPosition(x, y);
	}
}

void Playfield::Destroy()
{
	for (Receptor *receptor : receptors)
	{
		receptor->Destroy();
	}

	for (Note *note : notes)
	{
		note->Destroy();
	}

	lanes.clear();
	notes.clear();
	receptors.clear();
}

void Playfield::Render(GPU_Target *target)
{
	float width = playfield_rect.w / 4;

	for (int i = 0; i < 4; i++)
	{
		Lane *lane = lanes[i];
		lane->SetOrigin(playfield_rect.x + (width * i), -WIDTH / 2);
		lane->Render(target);

		Receptor *receptor = receptors[i];
		receptor->Render(target);
	}

	for (Note *note : notes)
	{
		note->Update(target);
	}
}

std::vector<Receptor *> Playfield::GetReceptors()
{
	return receptors;
}

std::vector<Lane *> Playfield::GetLanes()
{
	return lanes;
}