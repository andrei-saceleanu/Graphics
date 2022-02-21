#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;
uniform float Time;
uniform int Type;

// Output value to fragment shader
out vec3 color;

float rand(float n){return fract(sin(n) * 43758.5453123);}

float noise(float p){
	float fl = floor(p);
    float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}

float my_noise(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}


void main()
{
    float n = my_noise(vec2(Time,v_position.x));
    color = n*object_color;
    vec4 final_position;
    if(Type==0){
        final_position = vec4(v_position, 1.f) + n*vec4(v_normal,0.f);
    }else if(Type==1){
        float k = 2*3.14159f/ 0.5f;
        final_position = vec4(v_position, 1.f) + vec4(0.1f*sin(k*(v_position.x-0.4f*Time)),0.f,0.1f*sin(k*(v_position.z-0.4f*Time)),0.f);
    }
    gl_Position = Projection * View * Model * final_position;
}
