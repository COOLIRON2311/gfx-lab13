#include "main.h"

void Init()
{
	proj = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	affine = glm::mat4(1.0f);
	//�������� �������� �������
	glEnable(GL_DEPTH_TEST);
	// �������������� �������
	InitShader();
	// �������������� ��������� �����
	InitVBO();
	// InitTextures();
}

int main()
{
	setlocale(LC_ALL, "Russian");
	sf::Window window(sf::VideoMode(600, 600), "Shapes", sf::Style::Default, sf::ContextSettings(24));
	SetIcon(window);
	window.setVerticalSyncEnabled(true); // ������������ �������������
	window.setActive(true); // ������������� �������� OpenGL
	glewInit(); // �������������� GLEW
	Init(); // �������������� �������
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) // ���� ������������ ������ ����
			{
				window.close(); // ��������� ����
				goto EXIT_IS_RIGHT_HERE; // ������� �� �����
			}
			else if (event.type == sf::Event::Resized) // ���� ������������ ������� ������ ����
			{
				glViewport(0, 0, event.size.width, event.size.height); // ������������� ������� ������
			}
			else if (event.type == sf::Event::KeyPressed) // ���� ������������ ����� �������
			{
				// Rotation
				if (event.key.code == sf::Keyboard::P)
				{
					affine = glm::rotate(affine, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
				}
				else if (event.key.code == sf::Keyboard::R)
				{
					affine = glm::rotate(affine, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else if (event.key.code == sf::Keyboard::Y)
				{
					affine = glm::rotate(affine, 0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
				}

				// Movement
				else if (event.key.code == sf::Keyboard::W)
				{
					affine = glm::translate(affine, glm::vec3(0.0f, 0.0f, 0.1f));
				}
				else if (event.key.code == sf::Keyboard::S)
				{
					affine = glm::translate(affine, glm::vec3(0.0f, 0.0f, -0.1f));
				}
				else if (event.key.code == sf::Keyboard::A)
				{
					affine = glm::translate(affine, glm::vec3(-0.1f, 0.0f, 0.0f));
				}
				else if (event.key.code == sf::Keyboard::D)
				{
					affine = glm::translate(affine, glm::vec3(0.1f, 0.0f, 0.0f));
				}
				else if (event.key.code == sf::Keyboard::Z)
				{
					affine = glm::translate(affine, glm::vec3(0.0f, 0.1f, 0.0f));
				}
				else if (event.key.code == sf::Keyboard::X)
				{
					affine = glm::translate(affine, glm::vec3(0.0f, -0.1f, 0.0f));
				}

				else if (event.key.code == sf::Keyboard::F1)
				{
					proj = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
				}

				else if (event.key.code == sf::Keyboard::F2)
				{
					proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
				}

				else if (event.key.code == sf::Keyboard::Escape)
				{
					affine = glm::mat4(1.0f);
				}
			}
			
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ����� ����� � ����� �������
		Draw(window); // ������
		window.display(); // ������� �� �����
	}
EXIT_IS_RIGHT_HERE: // ����� ������
	Release(); // ������� �������
	return 0; // ������� �� ���������
}

void InitVBO()
{
	glGenBuffers(1, &VBO); // ���������� ��������� �����
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadOBJ("test.obj", vertices, uvs, normals);
	sz = vertices.size();
	vector<Vertex> data;
	for (int i = 0; i < sz; i++)
	{
		Vertex v = { vertices[i][0], vertices[i][1], vertices[i][2], white, uvs[i][0], uvs[i][1]};
		data.push_back(v);
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ����������� ��������� �����
	glBufferData(GL_ARRAY_BUFFER, sz * sizeof(Vertex), data.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // ���������� ��������� �����
	checkOpenGLerror();
}

void InitTextures()
{
	glGenTextures(1, &texture1); // ���������� ��������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1); // ����������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // ������������� ��������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, channels; // ��������� ��������
	unsigned char* data = stbi_load("metallica.jpg", &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data); // ����������� ������

	glGenTextures(1, &texture2); // ���������� ��������
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2); // ����������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // ������������� ��������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("wall.jpg", &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data); // ����������� ������
}

void LoadAttrib(GLuint prog, GLint& attrib, const char* attr_name)
{
	attrib = glGetAttribLocation(prog, attr_name);
	if (attrib == -1)
	{
		std::cout << "could not bind attrib " << attr_name << std::endl;
		return;
	}
}

void LoadUniform(GLuint prog, GLint& attrib, const char* attr_name)
{
	attrib = glGetUniformLocation(prog, attr_name);
	if (attrib == -1)
	{
		std::cout << "could not bind uniform " << attr_name << std::endl;
		return;
	}
}

void InitShader()
{
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &VertexShaderSource, NULL);
	glCompileShader(vShader);
	std::cout << "vertex shader \n";
	ShaderLog(vShader);

	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &FragShaderSource, NULL);
	glCompileShader(fShader);
	std::cout << "fragment shader \n";
	ShaderLog(fShader);
	
	// ������� ��������� ���������
	Program = glCreateProgram();

	// ����������� ������� � ���������
	glAttachShader(Program, vShader);
	glAttachShader(Program, fShader);
	
	// ������� ��������� ���������
	glLinkProgram(Program);

	int link;
	glGetProgramiv(Program, GL_LINK_STATUS, &link);

	// ��������� �� ������
	if (!link)
	{
		std::cout << "error attach shaders \n";
		return;
	}

	LoadAttrib(Program, A1_vertex, "coord");
	LoadAttrib(Program, A1_color, "color");
	LoadUniform(Program, U1_affine, "affine");
	LoadUniform(Program, U1_proj, "proj");
	checkOpenGLerror();
}

void Draw(sf::Window& window)
{
	window.setTitle("Solar System in OpenGL");
	glUseProgram(Program);
	glUniformMatrix4fv(U1_affine, 1, GL_FALSE, glm::value_ptr(affine));
	glUniformMatrix4fv(U1_proj, 1, GL_FALSE, glm::value_ptr(proj));
	glEnableVertexAttribArray(A1_vertex);
	glEnableVertexAttribArray(A1_color);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(A1_vertex, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A1_color, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, sz);
	glDisableVertexAttribArray(A1_vertex);
	glDisableVertexAttribArray(A1_color);
	glUseProgram(0); // ��������� ��������� ���������
	checkOpenGLerror(); // ��������� �� ������
}

void Release()
{
	ReleaseShader(); // ������� �������
	ReleaseVBO(); // ������� �����
}

void ReleaseVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); // ���������� �����
	glDeleteBuffers(1, &VBO); // ������� �����
}

void ReleaseShader()
{
	glUseProgram(0); // ��������� ��������� ���������
	glDeleteProgram(Program); // ������� ��������� ���������
}

void ShaderLog(unsigned int shader)
{
	int infologLen = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
	if (infologLen > 1)
	{
		int charsWritten = 0;
		std::vector<char> infoLog(infologLen);
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
		std::cout << "InfoLog: " << infoLog.data() << std::endl;
	}
}

void checkOpenGLerror()
{
	GLenum errCode;
	const GLubyte* errString;
	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		std::cout << "OpenGL error: " << errString << std::endl;
	}
}

void SetIcon(sf::Window& wnd)
{
	sf::Image image;
	if (!image.loadFromFile("icon.png"))
	{
		std::cout << "error load icon \n";
		return;
	}

	wnd.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
}
