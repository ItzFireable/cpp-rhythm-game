varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;
uniform vec2 resolution;

int round(float x) {
    return int(x + 0.5);
}

float character(int n, vec2 p)
{
	p = floor(p*vec2(4.0, -4.0) + 2.5);
    if (clamp(p.x, 0.0, 4.0) == p.x)
	{
        if (clamp(p.y, 0.0, 4.0) == p.y)
		{
        	int a = int(round(p.x) + 5.0 * round(p.y));
			if ((n > a) == true) return 1.0;
		}
    }
	return 0.0;
}


void main(void)
{
    vec2 pix = gl_FragCoord.xy;
	vec3 col = texture2D(tex, floor(pix/16.0)*16.0/resolution.xy).rgb;

	float gray = 0.3 * col.r + 0.59 * col.g + 0.11 * col.b;

    int n = 0;
    if (gray > 0.1) n = 9616687; // R
    if (gray > 0.3) n = 32012382; // S
    if (gray > 0.5) n = 16303663; // D
    if (gray > 0.7) n = 9616687; // R
	vec2 p = mod(pix/4.0, 2.0) - vec2(1.0);

    col = col*character(n, p);

	gl_FragColor = vec4(col, 1.0);
}