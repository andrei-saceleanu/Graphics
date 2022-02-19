#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 1) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;
out vec3 frag_color;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_normal		= v_normal;
	frag_texture	= v_texture;
	frag_color		= v_color +  vec3(sin(Time), cos(Time), sin(Time) * cos(Time));
    frag_position = v_position;

    // TODO(student): Compute gl_Position
    vec4 final_position = vec4(v_position, 1.f) + vec4(cos(Time), 0.f, sin(Time), 0.f) * 4.f;
    gl_Position = Projection * View * Model * final_position;

}
