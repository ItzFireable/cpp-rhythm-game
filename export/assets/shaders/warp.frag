varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;
uniform vec2 resolution;

uniform float intensity = 0.5;

void main() {
    // make me a cool shader

    vec2 uv = texCoord;
    vec4 c = color;

    uv.x += sin(uv.y * 10.0 + time) * 0.01 * intensity;
    uv.y += sin(uv.x * 10.0 + time) * 0.01 * intensity;

    gl_FragColor = texture2D(tex, uv);
}