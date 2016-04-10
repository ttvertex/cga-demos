// Fragment Shader
//
// Este Fragment Shader amostra a textura com id 'colorTextureSampler' e aplicada 
// esta amostra ao fragmento.
// Nenhum c�lculo de ilumina��o � feito.
//
// Abril 2016 - Alex Frasson - afrasson@inf.ufsm.br

#version 400

layout(location = 0) out vec4 FragColor;

uniform sampler2D colorTextureSampler;

in vec2 teTexCoord;

void main()
{
	FragColor = texture2D(colorTextureSampler, teTexCoord);
}