varying vec4 color;
varying vec2 texCoord;

uniform vec2 resolution;
uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;

#define MAX_DIST 100.
#define MIN_DIST 0.0001
#define MAX_STEPS 100


float sdCapsule(vec3 p, vec3 a, vec3 b, float r) {
    vec3 ab = b - a;
    vec3 ap = p - a;

    float t = dot(ab, ap) / dot(ab, ab);
    t = clamp(t, 0., 1.);

    vec3 c = a + t*ab;
    return length(p - c) - r;
}

float sdTorus(vec3 p, vec3 center, vec2 rs) {
    float verticalDist = p.y - center.y;
    float horizontalDist = length(center.xz - p.xz) - rs.x;
    float dist = sqrt(verticalDist*verticalDist + horizontalDist*horizontalDist);
    return dist - rs.y;

}

float sdBox(vec3 p, vec3 center, vec3 size) {
    vec3 normP = p - center;
    return length(max(abs(normP) - size, 0.));
}


float sdCylinder(vec3 p, vec3 a, vec3 b, float r) {
    vec3 ab = b - a;
    vec3 ap = p - a;

    float t = dot(ab, ap) / dot(ab, ab);
//    t = clamp(t, 0., 1.);

    vec3 c = a + t*ab;
    float d = length(p - c) - r;
    float y = (abs(t - 0.5) - 0.5)*length(ab);
    float e = length(max(vec2(d, y), 0.));
    return e;
}

float getDist(vec3 p) {
    vec4 sphere1 = vec4(0., 0.3, 4., 0.5);
    vec4 sphere2 = vec4(0., 0.3, 4., 0.5);
    sphere1.xz += vec2(sin(time), cos(time))*0.8;
    sphere2.xz += -1.*vec2(sin(time), cos(time))*0.8;

    float floorDist = p.y;
    float sphereDist1 = length(sphere1.xyz - p) - sphere1.w;
    float sphereDist2 = length(sphere2.xyz - p) - sphere2.w;


    vec3 lowCenter = vec3(0., 0., 4.);
    vec3 topCenter = vec3(0., 2., 4.);
//    lowCenter.xz += offset;
//    topCenter.xz += offset;
    float capsuleDist = sdCapsule(p, lowCenter, topCenter, .5);

    vec3 torusCenter = vec3(0., 2., 4.);
    torusCenter.y += sin(time);
//    torusCenter.xz += offset;
    float torusDist = sdTorus(p, torusCenter, vec2(0.7, .2));
//    float boxDist = sdBox(p, vec3(-2., 1., 5.), vec3(1.));





    float resDist = min(floorDist, torusDist);
    resDist = min(resDist, sphereDist1);
    resDist = min(resDist, sphereDist2);
    resDist = min(resDist, capsuleDist);
//    resDist = min(resDist, boxDist);
//    resDist = min(resDist, cylinderDist);

    return resDist;
}

float rayMarch(vec3 ro, vec3 rd) {
    float d = 0.;
    for (int i=0; i<MAX_STEPS; ++i) {
        vec3 p = ro + rd*d;
        float step = getDist(p);
        d += step;
        if (d < MIN_DIST || d > MAX_DIST) break;
    }
    return d;
}

vec3 getNormal(vec3 p) {
    float d = getDist(p);
    vec2 e = vec2(.01, 0.);


    vec3 normal = d - vec3(getDist(p - e.xyy),
                           getDist(p - e.yxy),
                           getDist(p - e.yyx));
    return normalize(normal);
}


float getLight(vec3 ro, vec3 rd) {

    vec3 lightSource = vec3(0., 3., 2.);
    lightSource += vec3(sin(time), 0., cos(time))*2.;
    float d = rayMarch(ro, rd);
    vec3 p = ro + rd*d;
    vec3 n = getNormal(p);
    vec3 l = normalize(lightSource - p);

    float light = clamp(dot(n, l), 0., 1.);
    float light_dist = rayMarch(p+n*.05, l);
    if (light_dist < length(lightSource - p)) light *= .3;

    return light;
}

void main(void)
{
    vec2 uv = (gl_FragCoord.xy - .5*resolution.xy)/resolution.y;
    uv.y = -uv.y;
    vec3 col = vec3(0.);
    float zoom = .5;

    vec3 ro = vec3(0., 1., 0.);

    vec3 lookAt = vec3(0., 1., 4.);
    vec3 f = normalize(lookAt - ro);
    vec3 r = normalize(cross(vec3(0., 1., 0.), f));
    vec3 u = cross(f, r);

    vec3 c = ro + f*zoom;
    vec3 i = c + uv.x*r + uv.y*u;
    vec3 rd = normalize(i - ro);


    float light = getLight(ro, rd);
    col = vec3(light);

    gl_FragColor = vec4(col,1.0);
}