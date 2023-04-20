varying vec4 color;
varying vec2 texCoord;

uniform vec2 resolution;
uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;

#define MAX_STEPS 96
#define MIN_DIST 0.001
#define NORMAL_SMOOTHNESS 0.02
#define PI 3.14159265359

mat2 Rotate(float angle)
{
	return mat2(cos(angle), sin(angle), -sin(angle), cos(angle));
}

float Union(float a, float b)
{
    return min(a, b);
}

float Sphere(float radius, vec3 position)
{
    return length(position) - radius;
}

float Box(float radius, vec3 position)
{
    return max(max(abs(position.x), abs(position.y)), abs(position.z)) - radius;
}

vec3 Normal(vec3 position)
{
	vec3 offset = vec3(NORMAL_SMOOTHNESS, 0, 0);

    vec3 normal = vec3
    (
        Scene(position - offset.xyz) - Scene(position + offset.xyz),
        Scene(position - offset.zxy) - Scene(position + offset.zxy),
        Scene(position - offset.yzx) - Scene(position + offset.yzx)
    );

    return normalize(normal);
}

vec3 RayMarch(vec3 origin,vec3 direction)
{
    float hitDist = 0.0;
    for(int i = 0;i < MAX_STEPS;i++)
    {
        float sceneDist = Scene(origin + direction * hitDist);

        hitDist += sceneDist;

        if(sceneDist < MIN_DIST)
        {
            break;
        }
    }

    return origin + direction * hitDist;
}

vec3 Shade(vec3 position, vec3 normal, vec3 rayOrigin,vec3 rayDirection)
{
    vec3 color = vec3(0, 0, 0);

    //Face Colors
    vec3 leftColor =  texture2D(tex, position.yz).rgb;
    vec3 frontColor = texture2D(tex, position.xy).rgb;
    vec3 topColor =   texture2D(tex, position.xz).rgb;

    color = mix(color, leftColor,  abs( dot(normal, vec3(1,0,0) ) ) );
    color = mix(color, frontColor, abs( dot(normal, vec3(0,0,1) ) ) );
    color = mix(color, topColor,   abs( dot(normal, vec3(0,1,0) ) ) );

    //Background
    color = mix(color, vec3(0.1), step(22.0, length(position)));

    return color;
}

float Scene(vec3 position)
{
    float map = -Sphere(24.0, position);

    float animTime = mod(time, 10.0);

    float xScale = smoothstep(1.0, 1.5, animTime) - smoothstep(6.0, 6.5, animTime);
    float zScale = smoothstep(2.0, 2.5, animTime) - smoothstep(7.0, 7.5, animTime);
    float yScale = smoothstep(3.0, 3.5, animTime) - smoothstep(8.0, 8.5, animTime);

    for(int i = -1; i <= 1;i++)
    {
        for(int j = -1; j <= 1;j++)
        {
            for(int k = -1; k <= 1;k++)
            {
                vec3 offset = vec3(i,j,k) * 2.0;

                offset.x *= 1.0 + xScale;
                offset.y *= 1.0 + yScale;
                offset.z *= 1.0 + zScale;

    			map = Union(map, Box(1.0, position + offset));
        	}
        }
    }

    return map;
}
void main() {
    vec2 aspect = resolution.xy / resolution.y;
	  vec2 uv = gl_FragCoord.xy / resolution.y;

    vec2 mouseAngle = vec2(0);

    vec3 rayOrigin = vec3(0 , 0, -20.0);
    vec3 rayDirection = normalize(vec3(uv - aspect / 2.0, 1.0));

    mat2 rotateX = Rotate(mouseAngle.x);
    mat2 rotateY = Rotate(mouseAngle.y);

    rayOrigin.yz *= rotateX;
    rayOrigin.xz *= rotateY;
    rayDirection.yz *= rotateX;
    rayDirection.xz *= rotateY;

    vec3 scenePosition = RayMarch(rayOrigin, rayDirection);

    vec3 outColor = Shade(scenePosition,Normal(scenePosition),rayOrigin,rayDirection);

	  gl_FragColor = vec4(outColor, 1.0);
}
