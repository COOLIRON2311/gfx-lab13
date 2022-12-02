#pragma once
#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>
#include <vector>
#include <corecrt_math_defines.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "objloader.hpp"

using namespace std;

int sz;

#define red    1.0, 0.0, 0.0, 1.0
#define green  0.0, 1.0, 0.0, 1.0
#define blue   0.0, 0.0, 1.0, 1.0
#define yellow 1.0, 1.0, 0.3, 1.0
#define orange 1.0, 0.5, 0.0, 1.0
#define violet 0.5, 0.0, 1.0, 1.0
#define white  1.0, 1.0, 1.0, 1.0
#define cyan   0.0, 1.0, 1.0, 1.0

GLuint VBO;
GLuint Program;
GLuint texture1;
GLuint texture2;

GLint A1_vertex;
GLint A1_color;
GLint U1_affine;
GLint U1_proj;

// ������� ������� ��������������
glm::mat4 affine;
// ������� ��������
glm::mat4 proj;

struct Vertex
{
	//coords
	GLfloat x;
	GLfloat y;
	GLfloat z;

	//colors
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;

	// texture coords
	GLfloat s;
	GLfloat t;
};

// ������� ��� ��������� ������ ����������
void SetIcon(sf::Window& wnd);
// ������� ��� �������� ������
void checkOpenGLerror();

void ShaderLog(unsigned int shader);
// ������� ��� �������� ��������
void InitShader();
void LoadAttrib(GLuint prog, GLint& attrib, const char* attr_name);
void LoadUniform(GLuint prog, GLint& attrib, const char* attr_name);
// ������� ��� ������������� ���������� ������
void InitVBO();
// ������� ��� ������������� ��������
void InitTextures();
void Init();
// ������� ��� ���������
void Draw(sf::Window& window);
// ������� ��� ������� ��������
void ReleaseShader();
// ������� ��� ������� ���������� ������
void ReleaseVBO();
// ������� ��� ������� ��������
void Release();

#pragma once
const char* VertexShaderSource = R"(
#version 330 core
in vec3 coord;
in vec4 color;
out vec4 vcolor;
uniform mat4 affine;
uniform mat4 proj;
void main() {
    gl_Position = vec4(coord / 2, 1.0);
    vcolor = color;
})";

const char* FragShaderSource = R"(
#version 330 core
in vec4 vcolor;
void main() {
    gl_FragColor = vcolor;
})";
