#include <default/velocity.h>

float Velocity::computeVelocity(float Velocity, float Acceleration, float Drag, float Max, float Elapsed)
{
	if (Acceleration != 0)
	{
		Velocity += Acceleration * Elapsed;
	}
	else if (Drag != 0)
	{
		float drag = Drag * Elapsed;
		if (Velocity - drag > 0)
		{
			Velocity -= drag;
		}
		else if (Velocity + drag < 0)
		{
			Velocity += drag;
		}
		else
		{
			Velocity = 0;
		}
	}
	if ((Velocity != 0) && (Max != 0))
	{
		if (Velocity > Max)
		{
			Velocity = Max;
		}
		else if (Velocity < -Max)
		{
			Velocity = -Max;
		}
	}
	return Velocity;
}