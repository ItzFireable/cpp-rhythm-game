varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;
uniform vec2 resolution;

/*void main(void)
{
    vec2 uv = texCoord;
    vec4 c = color;

    uv = uv * 2.0 - 1.0;
    uv *= (sin(time * 4) * 0.5 + 0.5);
    uv = uv * 0.5 + 0.5;

    gl_FragColor = texture2D(tex, uv) * c;
}*/

#define MAX_RADIUS 2
#define DOUBLE_HASH 0

#define HASHSCALE1 .1031
#define HASHSCALE3 vec3(.1031, .1030, .0973)

float hash12(vec2 p)
{
	vec3 p3 = fract(vec3(p.xyx) * HASHSCALE1);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}

vec2 hash22(vec2 p)
{
	vec3 p3 = fract(vec3(p.xyx) * HASHSCALE3);
    p3 += dot(p3, p3.yzx+19.19);
    return fract((p3.xx+p3.yz)*p3.zy);
}

void main(void)
{
    vec4 c = color;
    float resolution = 2.0;
	vec2 uv = texCoord * resolution;
    vec2 p0 = floor(uv);

    vec2 circles = vec2(0.);
    for (int j = -MAX_RADIUS; j <= MAX_RADIUS; ++j)
    {
        for (int i = -MAX_RADIUS; i <= MAX_RADIUS; ++i)
        {
			vec2 pi = p0 + vec2(i, j);
            #if DOUBLE_HASH
            vec2 hsh = hash22(pi);
            #else
            vec2 hsh = pi;
            #endif
            vec2 p = pi + hash22(hsh);

            float t = fract(0.3*time + hash12(hsh));
            vec2 v = p - uv;
            float d = length(v) - (float(MAX_RADIUS) + 1.)*t;

            float h = 1e-3;
            float d1 = d - h;
            float d2 = d + h;
            float p1 = sin(31.*d1) * smoothstep(-0.6, -0.3, d1) * smoothstep(0., -0.3, d1);
            float p2 = sin(31.*d2) * smoothstep(-0.6, -0.3, d2) * smoothstep(0., -0.3, d2);
            circles += 0.5 * normalize(v) * ((p2 - p1) / (2. * h) * (1. - t) * (1. - t));
        }
    }
    circles /= float((MAX_RADIUS*2+1)*(MAX_RADIUS*2+1));

    float intensity = mix(0.01, 0.15, smoothstep(0.1, 0.6, abs(fract(0.05*(time*0.1) + 0.5)*2.-1.)));
    vec3 n = vec3(circles, sqrt(1. - dot(circles, circles)));
    vec3 color = texture2D(tex, uv/resolution - intensity*n.xy).rgb + 5.*pow(clamp(dot(n, normalize(vec3(1., 0.7, 0.5))), 0., 1.), 6.);
	gl_FragColor = vec4(color, 1.0);
}

/*void main(void)
{
    vec2 uv = texCoord;
    vec4 c = color;// * colorOverride;

    vec2 wobbl = vec2(wobble_intensity * rand(vec2(time, gl_FragCoord.y)), 0.);

    float t_val = tan(0.25 * time + uv.y * PI * .67 * 1.5);
    vec2 tan_off = vec2(wobbl.x * min(0., t_val), 0.);

    vec4 color1 = texture2D(tex, uv + wobbl + tan_off);
    vec4 color2 = texture2D(tex, (uv + (wobbl * 1.5) + (tan_off * 1.3)) * 1.005);
    vec4 color = vec4(color2.rg, pow(color1.b, .67), 1.);
    color.rgb = mix(texture2D(tex, uv + tan_off).rgb, color.rgb, grade_intensity);

    float s_val = ((sin(2. * PI * uv.y + time * 20.) + sin(2. * PI * uv.y)) / 2.) * .015 * sin(time);
    color += s_val;

    float ival = gl_FragCoord.y / 4.;
    float r = rand(vec2(time, gl_FragCoord.y));

    color = vec4(min(1., color.r),
                 min(1., color.g),
                 min(1., color.b), 1.);

    float vig = 1. - sin(PI * uv.x) * sin(PI * uv.y);

    gl_FragColor = texture2D(tex, uv) * c * (color - (vig * vignette_intensity));

    /* baseline frag :)
    vec2 uv = texCoord;
    vec4 c = color;

    gl_FragColor = texture2D(tex, uv) * c;

}*/