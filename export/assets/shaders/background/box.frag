varying vec4 color;
varying vec2 texCoord;

uniform vec2 resolution;
uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;

#define s smoothstep
#define res resolution.xy
#define t time
#define pi 3.1415
#define tr start,dur,x
#define par float start, float dur, float p, float x

float c(float x){
    return max(0., min(1., (x - .0) / (1. - .0)));
}

float o(float x){
    return clamp(x, -1., .0);
}

float o2(float x){
    return clamp(x, 1., .0);
}

float ease(float easing, float start, float dur, float x){
    start += 1.;
    if (x >= start - 1. && x <= start){
        return easing;
    }else return o(x - start) + 1.;
}

float ease2(float easing, float start, float dur, float x){
    start += 1.;
    if (x >= start - 1. && x <= start){
        return easing;
    }else return o2(x - start);
}

float linear(par){
    start /= dur; x /= dur;
    return p*ease(x - start,tr);
}

float inSine(par){
    start /= dur; x /= dur;
    return p*ease(1. - cos(((x - start) * pi) / 2.),tr);
}

float outSine(par){
    start /= dur; x /= dur;
    return p*ease(sin(((x - start) * pi) / 2.),tr);
}

// I am pretty sure doing inEase + outEase would give the same result
// but whatev, I've already wrote everything out

float inOutSine(par){
    start /= dur; x /= dur;
    return p*ease(-(cos(pi * (x - start)) - 1.) / 2., tr);
}

float outInSine(par){
    start /= dur; x /= dur;
    return p*outSine(start, .5, .5, x) + p*inSine(start + .5, .5, .5, x);
}

float inQuad(par){
    start /= dur; x /= dur;
    return p*ease(pow(x - start, 2.),tr);
}

float outQuad(par){
    start /= dur; x /= dur;
    return p*ease(1. - pow(1. - (x - start), 2.),tr);
}

float inOutQuad(par){
    start /= dur; x /= dur;
    return x - (start - 1.) < 1.5
    ? p*ease(2. * ((x - (start * 1.)) * (x - (start * 1.))),tr)
    : p*ease(1. - pow(-2. * (x - (start * 1.)) + 2., 2.) / 2.,tr);
}

float outInQuad(par){
    start /= dur; x /= dur;
    return p*outQuad(start, .5, .5, x) + p*inQuad(start + .5, .5, .5, x);
}


float inCubic(par){
    start /= dur; x /= dur;
    return p*ease(pow(x - start, 3.),tr);
}

float outCubic(par){
    start /= dur; x /= dur;
    return p*ease(1. - pow( 1. - (x - start), 3.),tr);
}

float inOutCubic(par){
    start /= dur; x /= dur;
    return x - (start - 1.) < 1.5
    ? p*ease(4. * (x - start) * (x - start) * (x - start), tr)
    : p*ease(1. - pow(-2. * (x - start) + 2., 3.) / 2., tr);
}

float outInCubic(par){
    start /= dur; x /= dur;
    return p*outCubic(start, .5, .5, x) + p*inCubic(start+.5, .5, .5, x);
}

float inQuart(par){
    start /= dur; x /= dur;
    return p*ease(pow(x - start, 4.),tr);
}

float outQuart(par){
    start /= dur; x /= dur;
    return p*ease(1. - pow(1. - (x - start), 4.), tr);
}

float inOutQuart(par){
    start /= dur; x /= dur;
    return x - (start - 1.) < 1.5
    ? p*ease(8. * pow(x - start, 4.), tr)
    : p*ease(1. - pow(-2. * (x - start) + 2., 4.) / 2., tr);
}

float outInQuart(par){
    start /= dur; x /= dur;
    return p*outQuart(start, .5, .5, x) + p*inQuart(start + .5, .5, .5, x);
}

float inQuint(par){
    start /= dur; x /= dur;
    return p*ease(pow(x - start, 5.),tr);
}

float outQuint(par){
    start /= dur; x /= dur;
    return p*ease(1. - pow(1. - (x - start), 5.), tr);
}

float inOutQuint(par){
    start /= dur; x /= dur;
    return x - (start - 1.) < 1.5
    ? p*ease(16. * pow(x - start, 5.), tr)
    : p*ease(1. - pow(-2. * (x - start) + 2., 5.) / 2., tr);
}

float outInQuint(par){
    start /= dur; x /= dur;
    return p*outQuint(start, .5, .5, x) + p*inQuint(start + .5, .5, .5, x);
}

float inExpo(par){
    start /= dur; x /= dur;
    return p*ease(pow(2., 10. * (x - start) - 10.),tr);
}

float outExpo(par){
    start /= dur; x /= dur;
    return p*ease(1. - pow(2., -10. * (x - start)), tr);
}

float inOutExpo(par){
    start /= dur; x /= dur;
    return x - (start - 1.) < 1.5
    ? p*ease(pow(2., 20. * (x - start) - 10.) / 2., tr)
    : p*ease((2. - pow(2., -20. * (x - start) + 10.)) / 2., tr);
}

float outInExpo(par){
    start /= dur; x /= dur;
    return p*outExpo(start, .5, .5, x) + p*inExpo(start + .5, .5, .5, x);
}

float inCirc(par){
    start /= dur; x /= dur;
    return p*ease(c(1. - sqrt(1. - pow(x - start, 2.))), tr);
}

float outCirc(par){
    start /= dur; x /= dur;
    return p*ease(c(sqrt(1. - pow(x - start - 1., 2.))), tr);
}

float inOutCirc(par){
    start /= dur; x /= dur;
    return x - (start - 1.) < 1.5
    ? p*ease((1. - sqrt(1. - pow(2. * x - start * 2., 1.))) / 2., tr)
    : p*ease((sqrt(1. - pow(-2. * x + start * 2. + 2., 2.)) + 1.) / 2., tr);
}

float outInCirc(par){
    start /= dur; x /= dur;
    return p*outCirc(start, .5, .5, x) + p*inCirc(start + .5, .5, .5, x);
}

float inBack(par, float c1){
    start /= dur; x /= dur;
    if (c1 < .0) {
        c1 = 1.70158;
    }
    float c3 = c1 + 1.;

    return p*ease(c3 * pow((x - start), 3.) - c1 * pow((x - start), 2.), tr);
}

float outBack(par, float c1){
    start /= dur; x /= dur;
    if (c1 < .0) {
        c1 = 1.70158;
    }
    float c3 = c1 + 1.;

    return p*ease(1. + c3 * pow((x - start) - 1., 3.) + c1 * pow((x - start) - 1., 2.), tr);
}

float inOutBack(par){
    start /= dur; x /= dur;
    float c1 = 1.70158;
    float c2 = c1 * 1.525;

    return x - (start - 1.) < 1.5
    ? p*ease((pow(2. * (x - start), 2.) * ((c2 + 1.) * 2. * (x - start) - c2)) / 2., tr)
    : p*ease((pow(2. * (x - start) - 2., 2.) * ((c2 + 1.) * ((x - start) * 2. - 2.) + c2) + 2.) / 2., tr);
}

float outInBack(par, float c1){
    start /= dur; x /= dur;
    return p*outBack(start, .5, .5, x, c1) + p*inBack(start + .5, .5, .5, x, c1);
}

float inElastic(par){
    start /= dur; x /= dur;
    float c4 = (2. * pi) / 3.;

    return p*ease(-pow(2., 10. * (x - start) - 10.) * sin(((x - start) * 10. - 10.75) * c4), tr);
}

float outElastic(par){
    start /= dur; x /= dur;
    float c4 = (2. * pi) / 3.;

    return p*ease(pow(2., -10. * (x - start)) * sin(((x - start) * 10. - 0.75) * c4) + 1., tr);
}

float inOutElastic(par){
    start /= dur; x /= dur;
    float c5 = (2. * pi) / 4.5;

    return x - (start - 1.) < 1.5
      ? p*ease(-(pow(2., 20. * (x - start) - 10.) * sin((20. * (x - start) - 11.125) * c5)) / 2., tr)
      : p*ease((pow(2., -20. * (x - start) + 10.) * sin((20. * (x - start) - 11.125) * c5)) / 2. + 1., tr);
}

float outInElastic(par){
    start /= dur; x /= dur;
    return p*outElastic(start, .5, .5, x) + p*inElastic(start + .5, .5, .5, x);
}

float bounce(par){
    start /= dur; x /= dur;
    return p*ease2(4. * (x - start) * (1. - (x - start)), tr);
}

float tri(par){
    start /= dur; x /= dur;
    return p*ease2(1. - abs(2. * (x - start) - 1.), tr);
}

float bell(par){
    start /= dur; x /= dur;
    return p*inOutQuint(start, 1., 1., tri(start, 1., 1., x));
}

float pop(par){
    start /= dur; x /= dur;
    return p*ease2(3.5 * (1. - (x - start)) * (1. - (x - start)) * sqrt((x - start)), tr);
}

float tap(par){
    start /= dur; x /= dur;
    return p*ease2(3.5 * (x - start) * (x - start) * sqrt(1. - (x - start)), tr);
}

float pulse(par){
    start /= dur; x /= dur;
    return p*tap(start, .5, .5, (x - start))-pop(start+.5, .5, .5, (x - start));
}

float spike(par){
    start /= dur; x /= dur;
    return p*ease2(exp(-10. * abs(2. * (x - start) - 1.)), tr);
}

float invert(par){
    start /= dur; x /= dur;
    return p*ease2((x - start) * (x - start) * (1. - (x - start)) * (1. - (x - start)) / (0.5 - (x - start)), tr);
}

vec2 rotate(vec2 v, vec2 o, float a) {
    float s = sin(a);
    float c = cos(a);
    mat2 m = mat2(c, -s, s, c);
    return m * (v-o) + o;
}

vec2 TransformPlane(vec2 uv, vec2 center, float XRot, float YRot) {
    // First Rotate around Y axis
    vec2 RayDirection =  vec2(uv.x, 0.0);
    vec2 A1 = vec2(0.0, -1.0);
    vec2 B1 = RayDirection - A1;
    vec2 C1 = rotate(vec2(-1.0, 0.0), vec2(center.x, 0.0), YRot);
    vec2 D1 = rotate(vec2( 1.0, 0.0), vec2(center.x, 0.0), YRot) - C1;
    // calculate intersection point
    float u = ( (C1.y + 1.0) * D1.x - C1.x * D1.y ) / (D1.x*B1.y-D1.y*B1.x);
    // position on the plane:
    float sx = u * B1.x;
 	float sy = u * uv.y;
    // Now Rotate around X axis
    RayDirection = vec2(sy, .0);
    vec2 B2 = RayDirection - A1;
    vec2 C2 = rotate(vec2(-1.0, 0.00), vec2(center.y, 0.0), XRot);
    vec2 D2 = rotate(vec2( 1.0, 0.0), vec2(center.y, 0.0), XRot) - C2;
    // calculate intersection point
    float v = ( (C2.y + 1.0) * D2.x - C2.x * D2.y ) / (D2.x*B2.y-D2.y*B2.x);
    // final position on the plane:
    return vec2(v * sx, v * B2.x );

}

float s2(float uv, float a){
    return s(-a, -a+.01, -uv);
}

mat2 rot(float a){
    float s=sin(a),c=cos(a);
    return mat2(c,-s,s,c);
}

float box(vec2 uv, float si, float r){
    uv *= rot(pi*r);
    float b = s2(abs(uv.x), si)*s2(abs(uv.y), si);
    b *= 1.-s2(abs(uv.x), si/2.5)*s2(abs(uv.y), si/2.5);

    return b;
}

float cros(vec2 uv, float si, float r){
    uv *= rot(pi*r);
    float b = s2(abs(uv.x), si-si*.75)*s2(abs(uv.y), si);
    b += s2(abs(uv.y), si-si*.75)*s2(abs(uv.x), si);

    return min(b, 1.);
}

void main(void)
{
    vec2 uv = (gl_FragCoord.xy-.5*res)/res.y;

    vec2 uv2 = TransformPlane(abs(uv), vec2(.0, .0), pi*.25, .0);
    vec2 uv3 = TransformPlane(abs(uv), vec2(.0, .0), .0, pi*.35);

    float mr = .0;
    float loop3 = mod(t, 8.);


    mr += inCubic(0., 1., .5, loop3);
    mr += inCubic(2., 1., .5, loop3);
    mr += inCubic(4., 1., .5, loop3);
    mr += inCubic(6., 1., .5, loop3);


    uv *= rot(-mr*pi);


    vec2 uv1 = uv;
    vec2 uv4 = uv;

    vec3 bg = vec3(.0,.0,uv.y*.2+.2)*1.25;
    bg += vec3(s(-.5, 1.5, -uv.y*.25),.0,-uv.y*.1);

    float loop1 = mod(t,1.);
    float loop2 = mod(t,4.);

    float mult = 1.;


    mult += inCubic(.0, 1., 1., loop2);
    mult -= inCubic(2., 1., 1., loop2);

    uv1 *= 5.;
    uv4 *= 5.;
    float bx = .0;
    float by = .0;
    float br = .0;

    uv1.y = uv1.y-res.x/res.y*mult;
    uv4.y = uv4.y+res.x/res.y*mult;

    bx += inCubic(.0, 1., .75, loop2);
    bx += inCubic(1., 1., .75, loop2);
    bx += inCubic(2., 1., .75, loop2);
    bx += inCubic(3., 1., .75, loop2);

    br += inCubic(.0, 1., .5, loop1);
    by += inCubic(.0, 1., .13, loop1);
    by -= inCubic(.8, 1.-.8, .13, loop1);


    uv1.y -= by;
    uv1.x = fract(uv1.x-bx)-.5;
    uv4.y += by;
    uv4.x = fract(uv4.x+bx)-.5;

    uv2.y = abs(uv2.y)-t*.15;
    uv3.x = abs(uv3.x)-t*.1;
    uv2 = fract(uv2*5.)-.5;
    uv3 = fract(uv3*10.)-.5;

    bg += box(uv1, .3, br);
    bg += box(uv4, .3, br);
    bg += cros(uv2, .15, .0)*.2;
    bg += cros(uv3, .15, .0)*.1;

    gl_FragColor = vec4(bg,1.0);
}