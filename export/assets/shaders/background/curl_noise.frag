varying vec4 color;
varying vec2 texCoord;

uniform vec2 resolution;
uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;

#define PI 3.1415926538

float sin_lerp(float a,float b,float t)
{
    float tem=(1.0-cos(t*PI))/2.0;
    return a*(1.0-tem)+b*tem;
}

float random(float seed)
{
    return fract(sin(seed+1.) * 43758.5453);
}

float curl_random(float seed,float offset,float freq)
{
    return sin_lerp(random(offset+floor(mod(seed,freq))),random(offset+floor(mod(seed+1.,freq))),abs(seed-floor(seed)));
}

float atan2(in float y, in float x)
{
    return x == 0.0? sign(y)*PI/2.0 : atan(y, x);
}

float draw(float y,float v,float size)
{
    return smoothstep(v-size,v,y)-smoothstep(v,v+size,y);
}

void main(void) {
    vec2 st = texCoord;

    st-=vec2(0.5);
    vec3 col=vec3(0.0);

    vec2 new_st=vec2((atan2(st.x,st.y*2)+PI)/PI/2.+time*.000,length(st));
    float freq=10.0;
    float noise=.1;
    float size=.3;
    float widht=.003;
    #define SIDE_DEPTH 100
    for(int i=0;i<SIDE_DEPTH;i++){
        float nor=float(i)/float(SIDE_DEPTH)/2.;
        float offset=curl_random(time*3.+float(i)*.01+new_st.x*20.,0.,20.);
        float v=(curl_random(new_st.x*freq,10.,freq)*offset*noise+size);
        col+=vec3(draw(new_st.y*.5,nor*v,widht))/float(SIDE_DEPTH-i);;
    }
    col*=vec3(0.8706, 0.9765, 0.9843);
    gl_FragColor = vec4(col,1.0);
}