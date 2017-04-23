precision mediump float;
varying vec3 v_pos;
varying vec2 v_uv;

uniform samplerCube u_s_texture;

void main()
{

//Cube map is just upside down. 
//The solution to this situation is to rotate the cube map texture coordinates 180 degrees around the X axis 

	gl_FragColor = textureCube(u_s_texture, vec3(v_pos.x,-v_pos.yz) );
}
