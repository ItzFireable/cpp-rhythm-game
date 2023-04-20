varying vec4 color;
varying vec2 texCoord;

uniform vec2 resolution;
uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;

#define pi 3.1415
#define tr start,dur,x

#define s smoothstep
#define res resolution.xy
#define t time

#define bpm (60./132.)
#define beat (time / bpm)

float c(float x){
    return max(0., min(1., (x - .0) / (1. - .0)));
}

float o(float x){
    return clamp(x,-1., .0);
}

float ease(float easing, float start, float dur, float x){
    start += 1.;
    if (x >= start - 1. && x <= start){
        return easing;
    }else return o(x - start) + 1.;
}

float linear(float start, float dur, float x){
    start /= dur; x /= dur;
    return ease(x - start,tr);
}

float insine(float start, float dur, float x){
    start /= dur; x /= dur;
    return ease(1. - cos(((x - start) * pi) / 2.),tr);
}

float outsine(float start, float dur, float x){
    start /= dur; x /= dur;
    return ease(sin(((x - start) * pi) / 2.),tr);
}

float inoutsine(float start, float dur, float x){
    start /= dur; x /= dur;
    return ease(-(cos(pi * (x - start)) - 1.) / 2., tr);
}

float inquad(float start, float dur, float x){
    start /= dur; x /= dur;
    return ease((x - start) * (x - start),tr);
}

float outquad(float start, float dur, float x){
    start /= dur; x /= dur;
    return ease(1. - (1. - (x - start)) * (1. - (x - start)),tr);
}

float inoutquad(float start, float dur, float x){
    start /= dur; x /= dur;
    return x - (start - 1.) < 1.5
    ? ease(2. * ((x - (start * 1.)) * (x - (start * 1.))),tr)
    : ease(1. - pow(-2. * (x - (start * 1.)) + 2., 2.) / 2.,tr);
}

float inexpo(float start, float dur, float x){
    start /= dur; x /= dur;
    return x - (start - 1.) == 0. ? 0. :
    ease(pow(2., 10. * (x - start) - 10.),tr);
}

float outexpo(float start, float dur, float x){
    start /= dur; x /= dur;
    return x - (start - 1.) == 1. ? 1. : ease(1. - pow(2., -10. * (x - start)), tr);
}

float inoutexpo(float start, float dur, float x){
    start /= dur; x /= dur;
    return x - (start - 1.) == 0.
    ? 0.
    : x - (start - 1.) == 1.
    ? 1.
    : x - (start - 1.) < 1.5
    ? ease(pow(2., 20. * (x - start) - 10.) / 2., tr)
    : ease((2. - pow(2., -20. * (x - start) + 10.)) / 2., tr);
}

float incirc(float start, float dur, float x){
    start /= dur; x /= dur;
    return ease(c(1. - sqrt(1. - pow(x - start, 2.))), tr);
}

float outcirc(float start, float dur, float x){
    start /= dur; x /= dur;
    return ease(c(sqrt(1. - pow(x - start - 1., 2.))), tr);
}

float inoutcirc(float start, float dur, float x){
    start /= dur; x /= dur;
    return x - (start - 1.) < 1.5
    ? ease((1. - sqrt(1. - pow(2. * x - start * 2., 1.))) / 2., tr)
    : ease((sqrt(1. - pow(-2. * x + start * 2. + 2., 2.)) + 1.) / 2., tr);
}

float inback(float start, float dur, float x){
    start /= dur; x /= dur;
    float c1 = 1.70158;
    float c3 = c1 + 1.;

    return ease(c3 * (x - start) * (x - start) * (x - start) - c1 * (x - start) * (x - start), tr);
}

float outback(float start, float dur, float x){
    start /= dur; x /= dur;
    float c1 = 1.70158;
    float c3 = c1 + 1.;

    return ease(1. + c3 * pow((x - start) - 1., 3.) + c1 * pow((x - start) - 1., 2.), tr);
}

float inoutback(float start, float dur, float x){
    start /= dur; x /= dur;
    float c1 = 1.70158;
    float c2 = c1 * 1.525;

    return x - (start - 1.) < 1.5
    ? ease((pow(2. * (x - start), 2.) * ((c2 + 1.) * 2. * (x - start) - c2)) / 2., tr)
    : ease((pow(2. * (x - start) - 2., 2.) * ((c2 + 1.) * ((x - start) * 2. - 2.) + c2) + 2.) / 2., tr);
}

float rand1 (float st) {
    return fract(sin(dot(st*123.9385, st*532.18239)));
}

mat2 rot(float a){
    float s=sin(a),c=cos(a);
    return mat2(c,-s,s,c);
}

float box(vec2 uv, vec2 p, vec2 size, float rotation){
    uv -= p;
    uv *= rot(rotation*pi);
    float r = step(abs(uv.x), size.x);
    float b = step(abs(uv.y), size.y);
    return r*b;
}

float circle(vec2 uv, vec2 p, vec2 size){
    uv -= p;
    return step(length(uv), size.x);
}

void main(void) {
    vec2 uv = (gl_FragCoord.xy-.5*res)/res.y;
    vec2 uv2 = uv;

    float h = 1./8.;

    vec2 rp;
    vec2 rsize;
    float rrotation;
    vec2 bp;
    vec2 bsize;
    float brotation;

    rsize = vec2(.1);
    rp = vec2(0., .75);

    bsize = vec2(.1);
    bp = vec2(0., -.75);

    bp.y += .75*outexpo(0., 2., beat);
    bp.x += .5*inoutexpo(8., 3., beat);
    brotation += 2.*inoutcirc(8., 3., beat);

    rp.y -= .75*outexpo(0., 2., beat);
    rp.x -= .5*inoutexpo(8., 3., beat);
    rrotation -= 2.*inoutcirc(8., 3., beat);

    if (beat > 0.)
    {
        bsize += .025*linear(0, .001, mod(beat, 2.));
        bsize -= .025*outexpo(0.01, 2., mod(beat, 2.));

        rsize += .025*linear(0, .001, mod(beat, 2.));
        rsize -= .025*outexpo(0.01, 2., mod(beat, 2.));
    }

    float rb = circle(uv, rp, rsize);
    float bb = circle(uv, bp, bsize);
    float f = 1.;
    f = min(rb,bb);

    vec3 col;

    col += vec3(rb)*vec3(1.,0.,.5);
    col += vec3(bb)*vec3(0.,.5,1.);
    col += vec3(f);

    gl_FragColor = vec4(col,1.0);
}