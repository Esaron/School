uniform vec3 ambient,diffuse,specular;
uniform float exponent;
uniform float ka,kd,ks;

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
   // Transform vertex position to view space
   vec3 pos = vec3( gl_ModelViewMatrix * gl_Vertex );
   
   // Compute normal, light and view vectors in view space
   
   vNormal   = normalize(gl_NormalMatrix * gl_Normal);
   vLight    = normalize(vec3(gl_LightSource[0].position) - pos);
   vView     = normalize(-pos);
   
   // Compute the halfway vector if the halfway approximation is used   
   
   vHalfway  = normalize(vLight + vView);
   
   float lDotN = 0.5*(1+dot(vLight, vNormal));
   float nDotH = dot(vNormal, vHalfway);
   vec4 Id = lDotN*diffuse;
   vec4 Is = pow(nDotH, exponent)*specular;
   gl_FrontColor = ambient*ka + Id*kd + Is*ks;
   gl_Position = ftransform();

}