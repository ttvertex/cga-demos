// Fragment Shader
//
// Simples Fragment Shader que seta a cor de todos os fragmentos para gColor que � enviada pelo geometry shader.
// Nenhum c�lculo de ilumina��o � feito.
//
// Abril 2016 - Alex Frasson - afrasson@inf.ufsm.br

#version 400

in vec3 gColor;

layout( location = 0 ) out vec4 FragColor;

void main()
{
	FragColor = vec4(gColor, 1.0);
}