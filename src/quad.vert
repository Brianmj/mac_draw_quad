#version 410

layout(location = 0) in vec4 position;

uniform mat4 mvp;

//mat4 gen_matrix(float left, float right, float bottom, float top, float near, float far)
mat4 gen_matrix()
{
    
    /*float a = (2 * near) / (right - left);
    float b = (2 * near) / (top - bottom);
    float c = (right + left) / (right - left);
    float d = (top + bottom) / (top - bottom);
    float e = -(near + far) / (far - near);
    float f = -(2 * near * far) / (far - near);
    
    return mat4(a, 0.0f, 0.0f, 0.0f,
                0.0f, b, 0.0f, 0.0f,
                c, d, e, -1.0f,
                0.0f, 0.0f, f, 0.0f);*/
    
    mat4 t;
    t[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
    t[1] = vec4(0.0f, 1.0f, 0.0f, 0.0f);
    t[2] = vec4(0.0f, 0.0f, 1.0f, 0.0f);
    t[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    return t;
}


void main()
{

    
    gl_Position =  gen_matrix() * mvp * position;
    /*mat4 perspective = (projection_mat);
    perspective = gen_matrix(-9.0f, 9.0f, -9.0f, 9.0f, 0.1f, 100.0f);
    
    //gl_Position = position * modelview_mat * orthographc; // use this for orthographic
    gl_Position = perspective * modelview_mat * position;   // use this for perspective*/
}
