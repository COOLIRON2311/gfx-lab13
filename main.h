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
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>

using namespace std;

int VERTICES;

GLuint VBO;
GLuint Program;
GLuint texture_sun;
GLuint texture_mercury;
GLuint texture_venus;
GLuint texture_earth;
GLuint texture_mars;
GLuint texture_jupiter;
GLuint texture_saturn;
GLuint texture_uranus;
GLuint texture_neptune;

GLint A_vertex;
GLint A_uvs;
GLint U_affine;
GLint U_proj;

GLint U_offsets;

// Матрицы аффиных преобразований
glm::mat4 affine;
// Матрица проекции
glm::mat4 proj;

vector<glm::vec4> offsets;

vector<float> speeds_sun;
vector<float> speeds_axis;


struct Vertex
{
	//coords
	GLfloat x;
	GLfloat y;
	GLfloat z;

	// texture coords
	GLfloat s;
	GLfloat t;
};

// Функция для установки иконки приложения
void SetIcon(sf::Window& wnd);
// Функция для проверки ошибок
void checkOpenGLerror();

void ShaderLog(unsigned int shader);
// Функция для загрузки шейдеров
void InitShader();
void LoadAttrib(GLuint prog, GLint& attrib, const char* attr_name);
void LoadUniform(GLuint prog, GLint& attrib, const char* attr_name);
void LoadTexture(GLenum tex_enum, GLuint& tex, const char* path);
// Функция для инициализации вершинного буфера
void InitVBO();
// Функция для инициализации ресурсов
void InitTextures();
void Init();
// Функция для отрисовки
void Draw(sf::Window& window);
// Функция для очистки шейдеров
void ReleaseShader();
// Функция для очистки вершинного буфера
void ReleaseVBO();
// Функция для очистки ресурсов
void Release();

void load_obj(const char* filename, vector<Vertex>& out)
{
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> uvs;
	
    ifstream in(filename, ios::in);
    if (!in)
    {
        cerr << "Can't open obj " << filename << endl;
        return;
    }

    string line;
    while (getline(in, line))
    {
		string s = line.substr(0, 2);
		if (s == "v ")
		{
			istringstream s(line.substr(2));
			glm::vec3 v;
			s >> v.x;
			s >> v.y;
			s >> v.z;
			vertices.push_back(v);
		}
		else if (s == "vt")
		{
			istringstream s(line.substr(3));
			glm::vec2 uv;
			s >> uv.x;
			s >> uv.y;
			uvs.push_back(uv);
		}
		else if (s == "vn")
		{
			istringstream s(line.substr(3));
			glm::vec3 n;
			s >> n.x;
			s >> n.y;
			s >> n.z;
			normals.push_back(n);
		}
		else if (s == "f ")
		{
			istringstream s(line.substr(2));
			string s1, s2, s3;
			s >> s1;
			s >> s2;
			s >> s3;
			unsigned int v1, v2, v3, uv1, uv2, uv3, n1, n2, n3;
			sscanf_s(s1.c_str(), "%d/%d/%d", &v1, &uv1, &n1);
			sscanf_s(s2.c_str(), "%d/%d/%d", &v2, &uv2, &n2);
			sscanf_s(s3.c_str(), "%d/%d/%d", &v3, &uv3, &n3);
			Vertex ve1 = { vertices[v1 - 1].x, vertices[v1 - 1].y, vertices[v1 - 1].z, uvs[uv1 - 1].x, uvs[uv1 - 1].y };
			Vertex ve2 = { vertices[v2 - 1].x, vertices[v2 - 1].y, vertices[v2 - 1].z, uvs[uv2 - 1].x, uvs[uv2 - 1].y };
			Vertex ve3 = { vertices[v3 - 1].x, vertices[v3 - 1].y, vertices[v3 - 1].z, uvs[uv3 - 1].x, uvs[uv3 - 1].y };
			out.push_back(ve1);
			out.push_back(ve2);
			out.push_back(ve3);
		}
	}
}

const char* VertexShaderSource = R"(
#version 330 core
in vec3 coord;
in vec2 uv;
out vec2 texcoord;
flat out int index;
uniform mat4 affine;
uniform mat4 proj;
uniform vec4 offsets[9];

mat4 rotateX( in float angle ) {
	return mat4(	1.0,		0,			0,			0,
			 		0, 	cos(angle),	-sin(angle),		0,
					0, 	sin(angle),	 cos(angle),		0,
					0, 			0,			  0, 		1);
}

mat4 rotateY( in float angle ) {
	return mat4(	cos(angle),		0,		sin(angle),	0,
			 				0,		1.0,			 0,	0,
					-sin(angle),	0,		cos(angle),	0,
							0, 		0,				0,	1);
}

mat4 rotateZ( in float angle ) {
	return mat4(	cos(angle),		-sin(angle),	0,	0,
			 		sin(angle),		cos(angle),		0,	0,
							0,				0,		1,	0,
							0,				0,		0,	1);
}

void main() {
	float offset = offsets[gl_InstanceID].x;
    float scale = offsets[gl_InstanceID].y;
	float rot_axis = offsets[gl_InstanceID].z;
	float rot_sun = offsets[gl_InstanceID].w;
	vec4 pos = rotateY(rot_axis) * vec4(coord * scale * 2, 1.0);
    pos = rotateY(rot_sun) * (pos + vec4(offset, 0.0, 0.0, 0.0));
    gl_Position = proj * affine * pos;
	texcoord = uv;
	index = gl_InstanceID;
})";

const char* FragShaderSource = R"(
#version 330 core
in vec2 texcoord;
flat in int index;
uniform sampler2D sun;
uniform sampler2D mercury;
uniform sampler2D venus;
uniform sampler2D earth;
uniform sampler2D mars;
uniform sampler2D jupiter;
uniform sampler2D saturn;
uniform sampler2D uranus;
uniform sampler2D neptune;

void main() {
	switch (index)
	{
		case 0:
			gl_FragColor = texture(sun, texcoord);
			break;
		case 1:
			gl_FragColor = texture(mercury, texcoord);
			break;
		case 2:
			gl_FragColor = texture(venus, texcoord);
			break;
		case 3:
			gl_FragColor = texture(earth, texcoord);
			break;
		case 4:
			gl_FragColor = texture(mars, texcoord);
			break;
		case 5:
			gl_FragColor = texture(jupiter, texcoord);
			break;
		case 6:
			gl_FragColor = texture(saturn, texcoord);
			break;
		case 7:
			gl_FragColor = texture(uranus, texcoord);
			break;
		case 8:
			gl_FragColor = texture(neptune, texcoord);
			break;
	}
})";