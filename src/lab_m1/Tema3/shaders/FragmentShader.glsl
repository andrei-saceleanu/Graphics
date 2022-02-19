#version 330

// Input
in vec3 world_position;
in vec3 world_normal;
in vec2 texcoord;

uniform vec3 object_color;
uniform vec3 pointLightPos[9];
uniform vec3 pointLightColor[9];
uniform vec3 eye_position;
uniform vec3 discoPos;
uniform float material_kd;
uniform float material_ks;
uniform float material_ke;
uniform float material_shininess;
uniform int type;
uniform vec3 ceilPos[4],ceilDir[4],ceilColor[4];
uniform float cutoffAngle;
uniform float alpha;
uniform sampler2D texture_1,texture_2;
uniform float time;
uniform int show;

// Output
layout(location = 0) out vec4 out_color;


vec3 PointLightContribution(vec3 lightPos, vec3 lightColor,float r)
{
	vec3 color;
	vec3 N = normalize(world_normal);
	vec3 L = normalize(lightPos - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	
	float diffuse_light = material_kd * max(dot(N, L), 0.f);
    float specular_light = 0.f;
	float rad = r;
    
	
	float d = distance(lightPos,world_position);
	float att = 0.f;
	if(d<rad)
		att = pow(rad-d,2);
	

	float lum = 0.21f*lightColor.r+0.72f*lightColor.g+0.07f*lightColor.b;

	if (dot(N,L) > 0.f)
    {
        specular_light = material_ks * lum * pow(max(dot(N, H), 0.f), material_shininess);
    }
	color = att*(diffuse_light+specular_light)*lightColor;
    
	return color;
}

vec3 SpotLightContribution(vec3 lightPos,vec3 lightDir,vec3 lightColor,float ang){

	vec3 N = normalize(world_normal);
	vec3 L = normalize(lightPos - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	
	float diffuse_light = material_kd * max(dot(N, L), 0.f);
    float specular_light = 0.f;
	if (diffuse_light > 0.f)
    {
        specular_light = material_ks * pow(max(dot(N, H), 0.f), material_shininess);
    }
	float spot_light		= dot(-L, normalize(lightDir));
	float spot_light_limit	= cos(cutoffAngle);
		
	float light = 0.f;
	if (spot_light > spot_light_limit)
	{	 
		float linear_att		= (spot_light - spot_light_limit) / (1.f - spot_light_limit);
		float light_att_factor	= linear_att * linear_att;
		light					= light_att_factor * (diffuse_light+specular_light);
	}
	return lightColor*light;
}

void main()
{

	if(type==0){// floor tile
    	out_color = vec4(material_ke*object_color,1.f);
	}
	else if(type==1){// dancer,walls
		vec3 colour = object_color;
		if(show==0 || show==1){
			for(int i=0;i<9;i++){
				colour += PointLightContribution(pointLightPos[i],pointLightColor[i],1.5f);
			}
		}
		if(show==0 || show==2){
			for(int i=0;i<4;i++){
				colour += SpotLightContribution(ceilPos[i],ceilDir[i],ceilColor[i],cutoffAngle);
			}
		}

		if(show==0 || show==3){
			vec3 discoDir = world_position-discoPos;
			vec2 texC;
			texC.x = (1.0 / (2 * 3.14159)) * atan (discoDir.x, discoDir.z);
			texC.x = texC.x - time/10.f;
			texC.y = (1.0 / 3.14159) * acos (discoDir.y / length (discoDir));
			vec3 discoColor = mix(texture2D(texture_1,texC),texture2D(texture_2,texC),alpha).xyz;
			colour += PointLightContribution(discoPos,discoColor,6.2f);
		}
		out_color = vec4(colour,1.f);

	}else if(type==2){//ceilLights cones
		if(show==0||show==2){
			if(world_position.x<=0 || world_position.y<=0 || world_position.z<=0)
				discard;
			else{
				out_color = vec4(object_color,0.25f);
			}
		}
	}else{//disco ball
		if(show==0||show==3){
			vec4 color1 = texture2D(texture_1,texcoord);
			vec4 color2 = texture2D(texture_2,texcoord);
			out_color = mix(color1,color2,alpha);
		}
	}


}
