#pragma once

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

namespace GraphicsEngine {
	
	//abstract camera class, nothing special, camera can't move
	//just seperating this out
	//maybe I want split screen or something in the future
	class Camera {
	public:
		//attributes
		glm::vec3 position;
		glm::vec3 front;

		//projection matrix wont ever change so can just create it here in the camera
		glm::mat4 projection;  

	public:
		Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), float windowSize = 800.f) :
			front(glm::vec3(0.f, 0.f, -1.f)), position(position)
		{
			projection = glm::ortho(0.0f, 8.f, 0.f, 8.f, 0.001f, 2000.f);
			//projection = glm::perspective(glm::radians(90.f), (float)windowSize / windowSize, 0.01f, 2000.f);
		};

		glm::mat4 GetViewMatrix() {
			return glm::lookAt(position, position + front, glm::vec3(0.f, 1.f, 0.f));
		}
	};
}