#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	glm::vec3 Pos;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 model;
	float Speed = 0.5f;

	Camera()
	{
		Reset();
	}
	
	void W()
	{
		Pos += Speed * Front;
	}
	
	void S()
	{
		Pos -= Speed * Front;
	}
	
	void A()
	{
		Pos -= glm::normalize(glm::cross(Front, Up)) * Speed;
	}
	
	void D()
	{
		Pos += glm::normalize(glm::cross(Front, Up)) * Speed;
	}

	void YawPlus()
	{
		glm::vec3 newFront;
		newFront.x = cos(glm::radians(1.0f)) * Front.x - sin(glm::radians(1.0f)) * Front.z;
		newFront.y = Front.y;
		newFront.z = sin(glm::radians(1.0f)) * Front.x + cos(glm::radians(1.0f)) * Front.z;
		Front = glm::normalize(newFront);
	}
	
	void YawMinus()
	{
		glm::vec3 newFront;
		newFront.x = cos(glm::radians(-1.0f)) * Front.x - sin(glm::radians(-1.0f)) * Front.z;
		newFront.y = Front.y;
		newFront.z = sin(glm::radians(-1.0f)) * Front.x + cos(glm::radians(-1.0f)) * Front.z;
		Front = glm::normalize(newFront);
	}

	void PitchPlus()
	{
		glm::vec3 newFront;
		newFront.x = Front.x;
		newFront.y = cos(glm::radians(1.0f)) * Front.y - sin(glm::radians(1.0f)) * Front.z;
		newFront.z = sin(glm::radians(1.0f)) * Front.y + cos(glm::radians(1.0f)) * Front.z;
		Front = glm::normalize(newFront);
	}
	
	void PitchMinus()
	{
		glm::vec3 newFront;
		newFront.x = Front.x;
		newFront.y = cos(glm::radians(-1.0f)) * Front.y - sin(glm::radians(-1.0f)) * Front.z;
		newFront.z = sin(glm::radians(-1.0f)) * Front.y + cos(glm::radians(-1.0f)) * Front.z;
		Front = glm::normalize(newFront);
	}

	void Perspective()
	{
		proj = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);	
	}

	void Ortho()
	{
		proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	}

	void Reset()
	{	
		Pos = glm::vec3(0.0f, 0.0f, 3.0f);
		Front = glm::vec3(0.0f, 0.0f, -1.0f);
		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(Pos, Pos + Front, Up);
		proj = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
		model = glm::mat4(1.0f);
	}
	
	glm::mat4 MVP()
	{
		view = glm::lookAt(Pos, Pos + Front, Up);
		return proj * view * model;
	}
};