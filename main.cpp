#include "main.h"

void Init()
{
	proj = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	affine = glm::mat4(1.0f);
	speeds_sun = {
		0.00, 0.002, 0.003, 0.004, 0.005, 0.006, 0.007, 0.008, 0.009,
	};
	speeds_axis = {
		0.01, 0.04, 0.06, 0.08, 0.1, 0.12, 0.14, 0.16, 0.18,
	};
	offsets = {
		{0, 1.0, 0, 0}, // sun
		{3, 0.003504, 0, 0}, // mercury
		{4, 0.008691, 0, 0}, // venus
		{5, 0.009149, 0, 0}, // earth 
		{6, 0.004868, 0, 0}, // mars
		{7, 0.100398, 0, 0}, // jupyter
		{8, 0.083626, 0, 0}, // saturn
		{9, 0.036422, 0, 0}, // uranus
		{10, 0.035359, 0, 0} // neptune
	};
	//Включаем проверку глубины
	glEnable(GL_DEPTH_TEST);
	// Инициализируем шейдеры
	InitShader();
	// Инициализируем вершинный буфер
	InitVBO();
	InitTextures();
}

void Rotate()
{
	for (int i = 0; i < offsets.size(); i++)
	{
		offsets[i].z = fmod(offsets[i].z + speeds_axis[i], 2 * M_PI);
		offsets[i].w = fmod(offsets[i].w + speeds_sun[i], 2 * M_PI);
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	sf::Window window(sf::VideoMode(1000, 1000), "Shapes", sf::Style::Default, sf::ContextSettings(24));
	SetIcon(window);
	window.setVerticalSyncEnabled(true); // Вертикальная синхронизация
	window.setActive(true); // Устанавливаем контекст OpenGL
	glewInit(); // Инициализируем GLEW
	Init(); // Инициализируем ресурсы
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) // Если пользователь закрыл окно
			{
				window.close(); // Закрываем окно
				goto EXIT_IS_RIGHT_HERE; // Выходим из цикла
			}
			else if (event.type == sf::Event::Resized) // Если пользователь изменил размер окна
			{
				glViewport(0, 0, event.size.width, event.size.height); // Устанавливаем область вывода
			}
			else if (event.type == sf::Event::KeyPressed) // Если пользователь нажал клавишу
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очищаем буфер цвета и буфер глубины
		Rotate();
		Draw(window); // Рисуем
		window.display(); // Выводим на экран
	}
EXIT_IS_RIGHT_HERE: // Метка выхода
	Release(); // Очищаем ресурсы
	return 0; // Выходим из программы
}

void InitVBO()
{
	glGenBuffers(1, &VBO); // Генерируем вершинный буфер
	vector<Vertex> data;
	load_obj("sphere.obj", data);
	VERTICES = data.size();
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Привязываем вершинный буфер
	glBufferData(GL_ARRAY_BUFFER, VERTICES * sizeof(Vertex), data.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Отвязываем вершинный буфер
	checkOpenGLerror();
}

void InitTextures()
{
	LoadTexture(GL_TEXTURE0, texture_sun, "textures/sun_map.jpg");
	LoadTexture(GL_TEXTURE1, texture_mercury, "textures/mercury_map.jpg");
	LoadTexture(GL_TEXTURE2, texture_venus, "textures/venus_map.jpg");
	LoadTexture(GL_TEXTURE3, texture_earth, "textures/earth_map.jpg");
	LoadTexture(GL_TEXTURE4, texture_mars, "textures/mars_map.jpg");
	LoadTexture(GL_TEXTURE5, texture_jupiter, "textures/jupiter_map.jpg");
	LoadTexture(GL_TEXTURE6, texture_saturn, "textures/saturn_map.jpg");
	LoadTexture(GL_TEXTURE7, texture_uranus, "textures/uranus_map.jpg");
	LoadTexture(GL_TEXTURE8, texture_neptune, "textures/neptune_map.jpg");
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

void LoadTexture(GLenum tex_enum, GLuint& tex, const char* path)
{
	glGenTextures(1, &tex); // Генерируем текстуру
	glActiveTexture(tex_enum);
	glBindTexture(GL_TEXTURE_2D, tex); // Привязываем текстуру
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Устанавливаем параметры текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, channels; // Загружаем текстуру
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture " << path << std::endl;
	}
	stbi_image_free(data); // Освобождаем память
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
	
	// Создаем шейдерную программу
	Program = glCreateProgram();

	// Прикрепляем шейдеры к программе
	glAttachShader(Program, vShader);
	glAttachShader(Program, fShader);
	
	// Линкуем шейдерную программу
	glLinkProgram(Program);

	int link;
	glGetProgramiv(Program, GL_LINK_STATUS, &link);

	// Проверяем на ошибки
	if (!link)
	{
		std::cout << "error attach shaders \n";
		return;
	}

	LoadAttrib(Program, A_vertex, "coord");
	LoadAttrib(Program, A_uvs, "uv");
	LoadUniform(Program, U_affine, "affine");
	LoadUniform(Program, U_proj, "proj");
	LoadUniform(Program, U_offsets, "offsets");
	checkOpenGLerror();
	glUseProgram(Program);
	glUniform1i(glGetUniformLocation(Program, "sun"), 0);
	glUniform1i(glGetUniformLocation(Program, "mercury"), 1);
	glUniform1i(glGetUniformLocation(Program, "venus"), 2);
	glUniform1i(glGetUniformLocation(Program, "earth"), 3);
	glUniform1i(glGetUniformLocation(Program, "mars"), 4);
	glUniform1i(glGetUniformLocation(Program, "jupiter"), 5);
	glUniform1i(glGetUniformLocation(Program, "saturn"), 6);
	glUniform1i(glGetUniformLocation(Program, "uranus"), 7);
	glUniform1i(glGetUniformLocation(Program, "neptune"), 8);
	glUseProgram(0);
}

void Draw(sf::Window& window)
{
	window.setTitle("Solar System in OpenGL");
	glUseProgram(Program);
	glUniformMatrix4fv(U_affine, 1, GL_FALSE, glm::value_ptr(affine));
	glUniformMatrix4fv(U_proj, 1, GL_FALSE, glm::value_ptr(proj));
	glUniform4fv(glGetUniformLocation(Program, "offsets"), 9,
		glm::value_ptr(offsets[0]));
	glEnableVertexAttribArray(A_vertex);
	glEnableVertexAttribArray(A_uvs);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(A_vertex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A_uvs, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArraysInstanced(GL_TRIANGLES, 0, VERTICES, 9);
	glDisableVertexAttribArray(A_vertex);
	glDisableVertexAttribArray(A_uvs);
	glUseProgram(0); // Отключаем шейдерную программу
	checkOpenGLerror(); // Проверяем на ошибки
}

void Release()
{
	ReleaseShader(); // Очищаем шейдеры
	ReleaseVBO(); // Очищаем буфер
}

void ReleaseVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Отвязываем буфер
	glDeleteBuffers(1, &VBO); // Удаляем буфер
}

void ReleaseShader()
{
	glUseProgram(0); // Отключаем шейдерную программу
	glDeleteProgram(Program); // Удаляем шейдерную программу
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
		exit(1);
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
