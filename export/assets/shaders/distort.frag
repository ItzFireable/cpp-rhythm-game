varying vec4 color;
varying vec2 texCoord;

uniform vec2 resolution;
uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;

vec4 PostFX(vec2 uv)
{
  float radius = texCoord.x*1.4;
  float angle = sin(time);   //-1.+2.*
  vec2 center = vec2(texCoord.x*.8, texCoord.y)*1.5;

  vec2 texSize = vec2(texCoord.x/.6,texCoord.y/.5);
  vec2 tc = uv * texSize;
  tc -= center;
  float dist = length(tc*sin(time/5.));
  if (dist < radius)
  {
    float percent = (radius - dist) / radius;
    float theta = percent * percent * angle * 8.0;
    float s = sin(theta/2.);
    float c = cos(sin(theta/2.));
    tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
  }
  tc += center;
  vec3 color2 = texture2D(tex,(tc / texSize)).rgb;
  return vec4(color2, 1.0);
}


void main(void)
{
    vec2 uv = texCoord;
    vec4 c = color;

    //gl_FragColor = texture2D(tex, uv) * c;

    gl_FragColor = PostFX(uv) * c;
}