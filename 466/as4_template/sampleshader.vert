uniform vec3 ambient,diffuse,specular;
uniform float exponent;
uniform float ka,kd,ks;
attribute vec3 pos;

varying vec3 vNormal, vLight, vView, vHalfway;

vec3 crossP(vec3 v1, vec3 v2) {
    vec3 result;
	result.x = v1.y*v2.z - v1.z*v2.y;
	result.y = v1.z*v2.x - v1.x*v2.z;
	result.z = v1.x*v2.y - v1.y*v2.x;
	return result;
}

/*
===============================================================================
   Phong Shading: Vertex Program
===============================================================================
*/

void main(void)
{
   // Compute normal, light and view vectors in view space
   
   vNormal   = normalize(gl_NormalMatrix * gl_Normal);
   vLight    = normalize(vec3(gl_LightSource[0].position) - pos);
   vView     = normalize(-pos);
   
   // Compute the halfway vector if the halfway approximation is used   
   
   vHalfway  = normalize(vLight + vView);
   
   float lDotN = 0.5*(1+dot(vLight, vNormal));
   float nDotH = dot(vNormal, vHalfway);
   vec4 Ia = vec4(ambient, 1.0);
   vec4 Id = vec4(lDotN*diffuse, 1.0);
   vec4 Is = vec4(pow(nDotH, exponent)*specular, 1.0);
   gl_FrontColor = Ia*ka + Id*kd + Is*ks;
   gl_Position = gl_Vertex;

}