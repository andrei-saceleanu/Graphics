#version 330
#define MAX_STEPS 100
#define MAX_DIST 100.f
#define SURF_DIST .01f
const float PI = 3.14159265359;

// Input
in vec3 color;

uniform vec2 res;
uniform float iTime;
uniform vec3 cameraPos;
uniform vec3 f,r,u;

// Output
layout(location = 0) out vec4 out_color;


mat2 rotate2d(float theta) {
  float s = sin(theta), c = cos(theta);
  return mat2(c, -s, s, c);
}

// Rotation matrix around the X axis.
mat3 rotateX(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(1, 0, 0),
        vec3(0, c, -s),
        vec3(0, s, c)
    );
}

// Rotation matrix around the Y axis.
mat3 rotateY(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, 0, s),
        vec3(0, 1, 0),
        vec3(-s, 0, c)
    );
}

// Rotation matrix around the Z axis.
mat3 rotateZ(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, -s, 0),
        vec3(s, c, 0),
        vec3(0, 0, 1)
    );
}

// Identity matrix.
mat3 identity() {
    return mat3(
        vec3(1, 0, 0),
        vec3(0, 1, 0),
        vec3(0, 0, 1)
    );
}

float GetDist(vec3 p) {
	vec4 s = vec4(0, 1, 6, 1);
    
    float sphereDist =  length(p-s.xyz)-s.w;
    float planeDist = p.y;
    
    float d = min(sphereDist, planeDist);
    return d;
}

float RayMarch(vec3 ro, vec3 rd) {
	float dO=0.;
    
    for(int i=0; i<MAX_STEPS; i++) {
    	vec3 p = ro + rd*dO;
        float dS = GetDist(p);
        dO += dS;
        if(dO>MAX_DIST || dS<SURF_DIST) break;
    }
    
    return dO;
}

vec3 GetNormal(vec3 p) {
	float d = GetDist(p);
    vec2 e = vec2(.01, 0);
    
    vec3 n = d - vec3(
        GetDist(p-e.xyy),
        GetDist(p-e.yxy),
        GetDist(p-e.yyx));
    
    return normalize(n);
}



float GetLight(vec3 p) {
    vec3 lightPos = vec3(0, 5, 6);
    lightPos.xz += vec2(sin(iTime), cos(iTime))*2.;
    vec3 l = normalize(lightPos-p);
    vec3 n = GetNormal(p);
    
    float dif = clamp(dot(n, l), 0., 1.);
    float d = RayMarch(p+n*SURF_DIST*2., l);
    if(d<length(lightPos-p)) dif *= .1;
    
    return dif;
}





mat3 camera(vec3 cameraPos,vec3 lp){
    vec3 cd = normalize(lp-cameraPos);
    vec3 cr = normalize(cross(vec3(0,1,0),cd));
    vec3 cu = normalize(cross(cd,cr));
    return mat3(-cr,cu,-cd);
}



void main()
{
    vec2 uv = (gl_FragCoord.xy-0.5f*res)/res.y;
    vec3 col = vec3(0);
    
    
    vec3 ro = cameraPos;
    vec3 rd = mat3(-r,u,-f)*normalize(vec3(uv,1));
    



    float d = RayMarch(ro, rd);
    vec3 p = ro + rd * d;
    
    float dif = GetLight(p);
    col = vec3(dif);
    
    col = pow(col, vec3(.4545));

    out_color = vec4(col,1.0);

}
