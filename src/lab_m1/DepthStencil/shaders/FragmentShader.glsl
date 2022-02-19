#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 color;

// Output
layout(location = 0) out vec4 out_color;


float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
    //float depth = LinearizeDepth(gl_FragCoord.z) / far;
    //out_color = vec4(vec3(depth),1);
    out_color = vec4(color,1);
}

