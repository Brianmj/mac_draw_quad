#version 410

layout(location = 0) in vec4 position;

uniform mat4 projection_mat;
uniform mat4 modelview_mat;
uniform mat4 mvp;

mat4 gen_matrix(float left, float right, float bottom, float top, float near, float far)
{
    
    float a = (2 * near) / (right - left);
    float b = (2 * near) / (top - bottom);
    float c = (right + left) / (right - left);
    float d = (top + bottom) / (top - bottom);
    float e = -(near + far) / (far - near);
    float f = -(2 * near * far) / (far - near);
    
    return mat4(a, 0.0f, 0.0f, 0.0f,
                0.0f, b, 0.0f, 0.0f,
                c, d, e, -1.0f,
                0.0f, 0.0f, f, 0.0f);
}


void main()
{
    
    mat4 m = modelview_mat;
    mat4 p = projection_mat;
    
    gl_Position =   mvp * position;
    /*mat4 perspective = (projection_mat);
    perspective = gen_matrix(-9.0f, 9.0f, -9.0f, 9.0f, 0.1f, 100.0f);
    
    //gl_Position = position * modelview_mat * orthographc; // use this for orthographic
    gl_Position = perspective * modelview_mat * position;   // use this for perspective*/
}
