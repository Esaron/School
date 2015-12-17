//Shader for Gouraud shading

varying vec3 vNormal, vLight, vView, vHalfway;

void main(void)
{
   gl_FragColor = gl_Color;
}