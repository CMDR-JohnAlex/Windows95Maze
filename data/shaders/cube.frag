#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// Texture sampler
uniform sampler2D texture1;

uniform bool transparent;

void main()
{
	FragColor = texture(texture1, TexCoord);

	if(FragColor.a < 0.9)
		discard;
	
	if (transparent == true)
		FragColor.a = 0.75;
}
