varying vec4 color;
varying vec2 texCoord;

uniform vec2 resolution;
uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;

vec2 T = vec2(0.);
float No(float x){
return fract(9667.5*sin(7983.75*(x + T.x) + 297. + T.y));
}

vec4 Rancol(vec2 x){
return vec4(No(x.x + x.y), No(x.x*x.x+ x.y), No(x.x*x.x + x.y*x.y),1.);
}

//bubbles!!
vec4 grid(vec2 uv, float t){
vec4 C1,C2;
uv *= 16.;
vec2 id = vec2(int(uv.x),int(uv.y));
uv.y += (5.*No(id.x*id.x) + 1.)*t*.4	;
uv.y += No(id.x);
  id = vec2(int(uv.x), int(uv.y));
uv = fract(uv) - .5;

//if (id == vec2(1,10)){C1 = vec4(1.);}

float d = length(uv);
t *= 10.*No(id.x + id.y);
//uv.x += No(id.x);
//if (uv.x > .46 || uv.y > .46){C1 = vec4(1.);}

float r = .1*sin(t + sin(t)*.5)+.3;
float r1 = .07*sin(2.*t + sin(2.*t)*.5) +.1*No(id.x + id.y);
  if (d<r && d>r-.1){
  C2 = .5*Rancol(id + vec2(1.)) + vec4(.5);
  C2 *= smoothstep(r-.12,r,d);
  C2 *= 1. - smoothstep(r-.05, r+.12,d);
  }

  if (d<r1){
  C2 = .5*Rancol(id + vec2(1.)) + vec4(.5);
  }

  return C2 + C1;
}
void main(void) {
    vec2 uv = texCoord;
    vec4 c = color;

    uv.y *= resolution.y/resolution.x;
    float t = time;
    gl_FragColor = vec4(grid(uv, t));

    //gl_FragColor = texture2D(tex, uv) * c;
}