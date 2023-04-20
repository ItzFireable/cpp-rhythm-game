varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;
uniform vec2 resolution;

uniform float speed = 0.5;
#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286

float cosh(float val)
{
    float tmp = exp(val);
    float cosH = (tmp + 1.0 / tmp) / 2.0;
    return cosH;
}

float sinh(float val)
{
    float tmp = exp(val);
    float sinH = (tmp - 1.0 / tmp) / 2.0;
    return sinH;
}

float sech (float x) {
    return 2.*cosh(x)/(cosh(2.*x)+1.);
}

vec2 inv_f (vec2 z) {
    z = vec2(z.y, z.x+(time*speed)*.2)*3.;
    z = vec2(
        atan(sinh(z.x)/cos(z.y)),
        asin(sech(z.x)*sin(z.y))
    )/PI*4.;
    z = vec2(z.x+z.y, z.x-z.y);
    return vec2(fract(z.x), fract(z.y));
}

void main()
{
    //vec2 uv = texCoord;
    /*vec2 uv = texCoord;
    vec4 c = color;

    gl_FragColor = texture2D(tex, uv) * c;*/
    vec2 screen = texCoord;
    vec4 retina = texture2D(tex, inv_f(screen));

    gl_FragColor = retina * color;
}