#include <cmath>
#include <map>

#include <default/easing.h>

#ifndef PI
#define PI 3.1415926545
#endif

double easeLinear(double t, double b, double c, double d)
{
	return c * t / d + b;
}

double easeInQuad(double t, double b, double c, double d)
{
	t /= d;
	return c * t * t + b;
}

double easeOutQuad(double t, double b, double c, double d)
{
	t /= d;
	return -c * t * (t - 2) + b;
}

double easeInOutQuad(double t, double b, double c, double d)
{
	t /= d / 2;
	if (t < 1)
		return c / 2 * t * t + b;
	t--;
	return -c / 2 * (t * (t - 2) - 1) + b;
}

double easeOutInQuad(double t, double b, double c, double d)
{
	if (t < d / 2)
		return easeOutQuad(t * 2, b, c / 2, d);
	return easeInQuad((t * 2) - d, b + c / 2, c / 2, d);
}

double easeInCubic(double t, double b, double c, double d)
{
	t /= d;
	return c * t * t * t + b;
}

double easeOutCubic(double t, double b, double c, double d)
{
	t /= d;
	t--;
	return c * (t * t * t + 1) + b;
}

double easeInOutCubic(double t, double b, double c, double d)
{
	t /= d / 2;
	if (t < 1)
		return c / 2 * t * t * t + b;
	t -= 2;
	return c / 2 * (t * t * t + 2) + b;
}

double easeOutInCubic(double t, double b, double c, double d)
{
	if (t < d / 2)
		return easeOutCubic(t * 2, b, c / 2, d);
	return easeInCubic((t * 2) - d, b + c / 2, c / 2, d);
}

double easeInQuart(double t, double b, double c, double d)
{
	t /= d;
	return c * t * t * t * t + b;
}

double easeOutQuart(double t, double b, double c, double d)
{
	t /= d;
	t--;
	return -c * (t * t * t * t - 1) + b;
}

double easeInOutQuart(double t, double b, double c, double d)
{
	t /= d / 2;
	if (t < 1)
		return c / 2 * t * t * t * t + b;
	t -= 2;
	return -c / 2 * (t * t * t * t - 2) + b;
}

double easeOutInQuart(double t, double b, double c, double d)
{
	if (t < d / 2)
		return easeOutQuart(t * 2, b, c / 2, d);
	return easeInQuart((t * 2) - d, b + c / 2, c / 2, d);
}

double easeInQuint(double t, double b, double c, double d)
{
	t /= d;
	return c * t * t * t * t * t + b;
}

double easeOutQuint(double t, double b, double c, double d)
{
	t /= d;
	t--;
	return c * (t * t * t * t * t + 1) + b;
}

double easeInOutQuint(double t, double b, double c, double d)
{
	t /= d / 2;
	if (t < 1)
		return c / 2 * t * t * t * t * t + b;
	t -= 2;
	return c / 2 * (t * t * t * t * t + 2) + b;
}

double easeOutInQuint(double t, double b, double c, double d)
{
	if (t < d / 2)
		return easeOutQuint(t * 2, b, c / 2, d);
	return easeInQuint((t * 2) - d, b + c / 2, c / 2, d);
}

double easeInSine(double t, double b, double c, double d)
{
	return -c * cos(t / d * (PI / 2)) + c + b;
}

double easeOutSine(double t, double b, double c, double d)
{
	return c * sin(t / d * (PI / 2)) + b;
}

double easeInOutSine(double t, double b, double c, double d)
{
	return -c / 2 * (cos(PI * t / d) - 1) + b;
}

double easeOutInSine(double t, double b, double c, double d)
{
	if (t < d / 2)
		return easeOutSine(t * 2, b, c / 2, d);
	return easeInSine((t * 2) - d, b + c / 2, c / 2, d);
}

double easeInExpo(double t, double b, double c, double d)
{
	return c * pow(2, 10 * (t / d - 1)) + b;
}

double easeOutExpo(double t, double b, double c, double d)
{
	return c * (-pow(2, -10 * t / d) + 1) + b;
}

double easeInOutExpo(double t, double b, double c, double d)
{
	t /= d / 2;
	if (t < 1)
		return c / 2 * pow(2, 10 * (t - 1)) + b;
	t--;
	return c / 2 * (-pow(2, -10 * t) + 2) + b;
}

double easeOutInExpo(double t, double b, double c, double d)
{
	if (t < d / 2)
		return easeOutExpo(t * 2, b, c / 2, d);
	return easeInExpo((t * 2) - d, b + c / 2, c / 2, d);
}

double easeInCirc(double t, double b, double c, double d)
{
	t /= d;
	return -c * (sqrt(1 - t * t) - 1) + b;
}

double easeOutCirc(double t, double b, double c, double d)
{
	t /= d;
	t--;
	return c * sqrt(1 - t * t) + b;
}

double easeInOutCirc(double t, double b, double c, double d)
{
	t /= d / 2;
	if (t < 1)
		return -c / 2 * (sqrt(1 - t * t) - 1) + b;
	t -= 2;
	return c / 2 * (sqrt(1 - t * t) + 1) + b;
}

double easeOutInCirc(double t, double b, double c, double d)
{
	if (t < d / 2)
		return easeOutCirc(t * 2, b, c / 2, d);
	return easeInCirc((t * 2) - d, b + c / 2, c / 2, d);
}

double easeInElastic(double t, double b, double c, double d)
{
	double s = 1.70158;
	double p = 0;
	double a = c;
	if (t == 0)
		return b;
	t /= d;
	if (t == 1)
		return b + c;
	if (!p)
		p = d * .3;
	if (a < abs(c))
	{
		a = c;
		s = p / 4;
	}
	else
		s = p / (2 * PI) * asin(c / a);
	t--;
	return -(a * pow(2, 10 * t) * sin((t * d - s) * (2 * PI) / p)) + b;
}

double easeOutElastic(double t, double b, double c, double d)
{
	double s = 1.70158;
	double p = 0;
	double a = c;
	if (t == 0)
		return b;
	t /= d;
	if (t == 1)
		return b + c;
	if (!p)
		p = d * .3;
	if (a < abs(c))
	{
		a = c;
		s = p / 4;
	}
	else
		s = p / (2 * PI) * asin(c / a);
	return a * pow(2, -10 * t) * sin((t * d - s) * (2 * PI) / p) + c + b;
}

double easeInOutElastic(double t, double b, double c, double d)
{
	double s = 1.70158;
	double p = 0;
	double a = c;
	if (t == 0)
		return b;
	t /= d / 2;
	if (t == 2)
		return b + c;
	if (!p)
		p = d * (.3 * 1.5);
	if (a < abs(c))
	{
		a = c;
		s = p / 4;
	}
	else
		s = p / (2 * PI) * asin(c / a);
	if (t < 1)
	{
		t--;
		return -.5 * (a * pow(2, 10 * t) * sin((t * d - s) * (2 * PI) / p)) + b;
	}
	t--;
	return a * pow(2, -10 * t) * sin((t * d - s) * (2 * PI) / p) * .5 + c + b;
}

double easeOutInElastic(double t, double b, double c, double d)
{
	if (t < d / 2)
		return easeOutElastic(t * 2, b, c / 2, d);
	return easeInElastic((t * 2) - d, b + c / 2, c / 2, d);
}

double easeInBack(double t, double b, double c, double d)
{
	double s = 1.70158;
	t /= d;
	return c * t * t * ((s + 1) * t - s) + b;
}

double easeOutBack(double t, double b, double c, double d)
{
	double s = 1.70158;
	t /= d;
	t--;
	return c * (t * t * ((s + 1) * t + s) + 1) + b;
}

double easeInOutBack(double t, double b, double c, double d)
{
	double s = 1.70158;
	t /= d / 2;
	if (t < 1)
	{
		s *= (1.525);
		return c / 2 * (t * t * ((s + 1) * t - s)) + b;
	}
	t -= 2;
	s *= (1.525);
	return c / 2 * (t * t * ((s + 1) * t + s) + 2) + b;
}

double easeOutInBack(double t, double b, double c, double d)
{
	if (t < d / 2)
		return easeOutBack(t * 2, b, c / 2, d);
	return easeInBack((t * 2) - d, b + c / 2, c / 2, d);
}

double easeOutBounce(double t, double b, double c, double d)
{
	t /= d;
	if (t < (1 / 2.75))
	{
		return c * (7.5625 * t * t) + b;
	}
	else if (t < (2 / 2.75))
	{
		t -= (1.5 / 2.75);
		return c * (7.5625 * t * t + .75) + b;
	}
	else if (t < (2.5 / 2.75))
	{
		t -= (2.25 / 2.75);
		return c * (7.5625 * t * t + .9375) + b;
	}
	else
	{
		t -= (2.625 / 2.75);
		return c * (7.5625 * t * t + .984375) + b;
	}
}

double easeInBounce(double t, double b, double c, double d)
{
	return c - easeOutBounce(d - t, 0, c, d) + b;
}

double easeInOutBounce(double t, double b, double c, double d)
{
	if (t < d / 2)
		return easeInBounce(t * 2, 0, c, d) * .5 + b;
	else
		return easeOutBounce(t * 2 - d, 0, c, d) * .5 + c * .5 + b;
}

double easeOutInBounce(double t, double b, double c, double d)
{
	if (t < d / 2)
		return easeOutBounce(t * 2, b, c / 2, d);
	return easeInBounce((t * 2) - d, b + c / 2, c / 2, d);
}

easing_functions stringToEasing(std::string easingFunction)
{
	if (easingFunction == "easeInSine")
	{
		return EaseInSine;
	}
	else if (easingFunction == "easeOutSine")
	{
		return EaseOutSine;
	}
	else if (easingFunction == "easeInOutSine")
	{
		return EaseInOutSine;
	}
	else if (easingFunction == "easeInQuad")
	{
		return EaseInQuad;
	}
	else if (easingFunction == "easeOutQuad")
	{
		return EaseOutQuad;
	}
	else if (easingFunction == "easeInOutQuad")
	{
		return EaseInOutQuad;
	}
	else if (easingFunction == "easeInCubic")
	{
		return EaseInCubic;
	}
	else if (easingFunction == "easeOutCubic")
	{
		return EaseOutCubic;
	}
	else if (easingFunction == "easeInOutCubic")
	{
		return EaseInOutCubic;
	}
	else if (easingFunction == "easeInQuart")
	{
		return EaseInQuart;
	}
	else if (easingFunction == "easeOutQuart")
	{
		return EaseOutQuart;
	}
	else if (easingFunction == "easeInOutQuart")
	{
		return EaseInOutQuart;
	}
	else if (easingFunction == "easeInQuint")
	{
		return EaseInQuint;
	}
	else if (easingFunction == "easeOutQuint")
	{
		return EaseOutQuint;
	}
	else if (easingFunction == "easeInOutQuint")
	{
		return EaseInOutQuint;
	}
	else if (easingFunction == "easeInExpo")
	{
		return EaseInExpo;
	}
	else if (easingFunction == "easeOutExpo")
	{
		return EaseOutExpo;
	}
	else if (easingFunction == "easeInOutExpo")
	{
		return EaseInOutExpo;
	}
	else if (easingFunction == "easeInCirc")
	{
		return EaseInCirc;
	}
	else if (easingFunction == "easeOutCirc")
	{
		return EaseOutCirc;
	}
	else if (easingFunction == "easeInOutCirc")
	{
		return EaseInOutCirc;
	}
	else if (easingFunction == "easeInBack")
	{
		return EaseInBack;
	}
	else if (easingFunction == "easeOutBack")
	{
		return EaseOutBack;
	}
	else if (easingFunction == "easeInOutBack")
	{
		return EaseInOutBack;
	}
	else if (easingFunction == "easeInElastic")
	{
		return EaseInElastic;
	}
	else if (easingFunction == "easeOutElastic")
	{
		return EaseOutElastic;
	}
	else if (easingFunction == "easeInOutElastic")
	{
		return EaseInOutElastic;
	}
	else if (easingFunction == "easeInBounce")
	{
		return EaseInBounce;
	}
	else if (easingFunction == "easeOutBounce")
	{
		return EaseOutBounce;
	}
	else if (easingFunction == "easeInOutBounce")
	{
		return EaseInOutBounce;
	}
	else
	{
		return EaseLinear;
	}
}

easingFunction getEasingFunction(easing_functions function)
{
	static std::map<easing_functions, easingFunction> easingFunctions;
	if (easingFunctions.empty())
	{
		easingFunctions.insert(std::make_pair(EaseLinear, easeLinear));
		easingFunctions.insert(std::make_pair(EaseInSine, easeInSine));
		easingFunctions.insert(std::make_pair(EaseOutSine, easeOutSine));
		easingFunctions.insert(std::make_pair(EaseInOutSine, easeInOutSine));
		easingFunctions.insert(std::make_pair(EaseInQuad, easeInQuad));
		easingFunctions.insert(std::make_pair(EaseOutQuad, easeOutQuad));
		easingFunctions.insert(std::make_pair(EaseInOutQuad, easeInOutQuad));
		easingFunctions.insert(std::make_pair(EaseInCubic, easeInCubic));
		easingFunctions.insert(std::make_pair(EaseOutCubic, easeOutCubic));
		easingFunctions.insert(std::make_pair(EaseInOutCubic, easeInOutCubic));
		easingFunctions.insert(std::make_pair(EaseInQuart, easeInQuart));
		easingFunctions.insert(std::make_pair(EaseOutQuart, easeOutQuart));
		easingFunctions.insert(std::make_pair(EaseInOutQuart, easeInOutQuart));
		easingFunctions.insert(std::make_pair(EaseInQuint, easeInQuint));
		easingFunctions.insert(std::make_pair(EaseOutQuint, easeOutQuint));
		easingFunctions.insert(std::make_pair(EaseInOutQuint, easeInOutQuint));
		easingFunctions.insert(std::make_pair(EaseInExpo, easeInExpo));
		easingFunctions.insert(std::make_pair(EaseOutExpo, easeOutExpo));
		easingFunctions.insert(std::make_pair(EaseInOutExpo, easeInOutExpo));
		easingFunctions.insert(std::make_pair(EaseInCirc, easeInCirc));
		easingFunctions.insert(std::make_pair(EaseOutCirc, easeOutCirc));
		easingFunctions.insert(std::make_pair(EaseInOutCirc, easeInOutCirc));
		easingFunctions.insert(std::make_pair(EaseInBack, easeInBack));
		easingFunctions.insert(std::make_pair(EaseOutBack, easeOutBack));
		easingFunctions.insert(std::make_pair(EaseInOutBack, easeInOutBack));
		easingFunctions.insert(std::make_pair(EaseInElastic, easeInElastic));
		easingFunctions.insert(std::make_pair(EaseOutElastic, easeOutElastic));
		easingFunctions.insert(std::make_pair(EaseInOutElastic, easeInOutElastic));
		easingFunctions.insert(std::make_pair(EaseInBounce, easeInBounce));
		easingFunctions.insert(std::make_pair(EaseOutBounce, easeOutBounce));
		easingFunctions.insert(std::make_pair(EaseInOutBounce, easeInOutBounce));
	}

	auto it = easingFunctions.find(function);
	return it == easingFunctions.end() ? nullptr : it->second;
}