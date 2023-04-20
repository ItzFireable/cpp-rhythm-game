varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;
uniform vec2 resolution;

uniform float exposure = 10.0;
uniform float gamma = 2.2;

void main(void)
{
    vec2 uv = texCoord;
    vec3 hdrColor = color.rgb;

    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));

    gl_FragColor = texture2D(tex,uv) * vec4(mapped, color.a);
}