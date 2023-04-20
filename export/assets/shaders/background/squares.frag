varying vec4 color;
varying vec2 texCoord;

uniform vec2 resolution;
uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;

const float tau = 6.283185307179586;

vec2 cossin(float t) {
    return vec2(cos(t), sin(t));
}

mat2 rot(float t) {
    vec2 cs = cossin(t);
    return mat2(cs.x, -cs.y, cs.y, cs.x);
}

void main(void) {
    vec2 pos = ((gl_FragCoord.xy * 2.) - resolution.xy) / resolution.y;
    int i;
    for(i = 0; i < 64; i++) {
        float depth = float(i)+1.-fract(time*3.);
        float z = float(i)+floor(time*3.);
        vec2 op = rot(z*0.2) * (pos * depth * .5 + cossin((time*3.-1.)*.2)*2.) + .25;
        if (fract(op.x) > .5 ^^ fract(op.y) > .5) break;
    }

    gl_FragColor = vec4(sqrt( (cos((vec3(0,1,2)/3. + (float(i)+floor(time*3.)) * .03) * tau) * .5 + .5) * exp(-(float(i)+1.-fract(time*3.))*.2) ),1);
}