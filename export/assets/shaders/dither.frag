varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;
uniform vec2 resolution;

float hash13(vec3 p3) {
	p3  = fract(p3 * .1031);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}

const float SIZE = 2.0;

void main() {
  vec2 uv = texCoord;
  vec3 col = vec3(1.0,1.0, 1.0);
  float noise = 0.0;

  noise = hash13(vec3(uv, time*5.0));
  col += (noise-0.5)/SIZE*2.0;
  col = (floor(col*SIZE)+0.5)/SIZE;

  gl_FragColor = texture2D(tex,uv) * vec4(col, 1.0);
}