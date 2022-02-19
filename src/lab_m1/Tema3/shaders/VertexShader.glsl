#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec2 texcoord;


void main()
{
    world_position = (Model * vec4(v_position, 1)).xyz;
	world_normal = normalize((Model * vec4(v_normal, 0)).xyz);
    texcoord = v_texture_coord;
    if (time >= 0)
	{
		texcoord = vec2(texcoord.x - time / 10.f, texcoord.y);
	}

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
