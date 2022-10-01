#version 400

// in from vertex shader
in vec4 ex_Position;
in vec4 ex_PositionWorld;
in vec4 ex_Normal;
in vec4 ex_UV_x2;

// Default colour output
layout ( location = 0 ) out vec4 out_Colour;

// Textures
uniform sampler2D Texture00;
uniform sampler2D Texture01;
uniform sampler2D Texture02;
uniform sampler2D Texture03;
uniform sampler2D Texture04;
uniform sampler2D Texture05;
uniform sampler2D Texture06;
uniform sampler2D Texture07;

// Material
struct MaterialDesc
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform MaterialDesc materials;

// Lights
struct LightDesc
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float attenConst;
	float attenLinear;
	float attenQuad;
};
uniform LightDesc lights;

// Camera
uniform vec3 eye;

vec3 ADSLightModel( in vec3 Normal, 
					in vec3 Position);

void main(void)
{
	vec3 color = texture(Texture00, ex_UV_x2.xy).xyz;
	vec3 lightContribution = ADSLightModel(ex_Normal.xyz, ex_PositionWorld.xyz);

	out_Colour = vec4(color.rgb * lightContribution, 1);
}


vec3 ADSLightModel( in vec3 Normal, 
					in vec3 Position)
{
	// normal, light, view, and light reflection vectors
	vec3 norm = 	normalize( Normal ); //normalize( Normal );
	vec3 lightv = 	normalize( lights.position - Position);
	vec3 viewv = 	normalize( eye - Position );
	vec3 refl = 	-( reflect( lightv, norm ) );

	// ambient light computation
	vec3 ambient = lights.ambient;
	
	// diffuse light computation
	vec3 diffuse = max(0.0f, dot(lightv, norm))
				   * lights.diffuse;// myLightDiffuse;
				   
	// Optionally you can add a diffuse attenuation term at this point
	//float dist = distance( ex_Position, sunLight.position );	
	float dist = distance( Position, lights.position );	
	float atten = 1.0f /( lights.attenConst + 
	                      lights.attenLinear * dist +
						  lights.attenQuad * dist * dist );
	
	diffuse *= atten;
	
	// specular light computation
	vec3 specular = vec3( 0.0f, 0.0f, 0.0f );	
	if( dot(lightv, norm) > 0.0f )
	{
		specular = pow( max(0.0f, dot(viewv,refl)), 128)
					* lights.specular;// // myLightSpecular;
		specular *= atten;
	}
	
	return clamp( ambient + diffuse + specular, 0.0f, 1.0f );
}