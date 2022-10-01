#version 400

in vec2 ex_TexCoord;

out vec4 out_Colour;

uniform sampler2D Texture00;

void main(void)
{
	out_Colour = texture(Texture00, ex_TexCoord.xy);
	
	if ( out_Colour.rgb == vec3(0.0f, 0.0f, 0.0f) )
	{
		discard;
	}
	
	return;
}
