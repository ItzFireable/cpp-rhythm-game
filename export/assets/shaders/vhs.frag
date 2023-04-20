varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;
uniform vec2 resolution;

#define PI  3.14159265359

vec2 curvature = vec2(8,8);
vec2 remapUV(vec2 uv) {
    uv = uv * 2. -1.;
    vec2 offset = abs(uv.yx) / vec2(curvature.x, curvature.y);
    uv = uv + uv * offset * offset;
    uv = uv * .5 + .5;
    return uv;
}

vec4 scanLine(float uv, float res, float opacity) {
     float intensity = sin(uv * res * PI * 2.);
     intensity = ((.5 * intensity) + .5) * .9 + .1;
     return vec4(vec3(pow(intensity, opacity)), 1.);
}

vec4 vignette(vec2 uv, float opacity) {
    float intensity = uv.x * uv.y * (1. - uv.x) * (1. - uv.y);
    return vec4(vec3(clamp(pow((resolution.x / 4.) * intensity, opacity), 0.0, 1.)), 1.);
}

vec2 scanLineOpacity = vec2(.35);
void main(void)
{
    vec2 uv = texCoord;
    vec4 c = color;// * colorOverride;

    uv=(uv*1.05)-vec2(.025,.025);
    vec2 vuv = remapUV(uv);

    vec4 baseColor = texture2D(tex, vuv);

    baseColor *= vignette(vuv, .75);
    baseColor *= scanLine(vuv.x, resolution.y*.9, scanLineOpacity.x);
    baseColor *= scanLine(vuv.y, resolution.x*.9, scanLineOpacity.y);

    if (vuv.x < 0.0 || vuv.y < 0.0 || vuv.x > 1.0 || vuv.y > 1.0){
        baseColor = vec4(vec3(.0),0);
    }

	gl_FragColor = baseColor * c;

    //gl_FragColor = texture2D(tex, uv) * c * (color - (vig * vignette_intensity));

    /* baseline frag :)
    vec2 uv = texCoord;
    vec4 c = color;

    gl_FragColor = texture2D(tex, uv) * c;
    */
}