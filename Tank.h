#ifndef TANK_H
#define TANK_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"

class Tank
{
private:
	glm::mat4 M = glm::mat4(1.0f);
	glm::mat4 M_wieza = glm::mat4(1.0f);
	glm::vec3 lufa_cords = glm::vec3(1.2f, 0.2f, 0.0f);
	glm::vec3 tank_size = glm::vec3(1.2f, 0.2f, 0.0f);
	glm::vec4 Position = glm::vec4(glm::vec3(0.0f), 1.0f);
	glm::vec4 tank_position = glm::vec4(glm::vec3(0.0f), 1.0f);

public:
	glm::vec3 getLufa_cords();
	glm::vec4 getPosition();
	glm::mat4 getM_wieza();
	glm::mat4 getM();
	bool collision_detector(glm::vec3 object_position, glm::vec3 object_size);
	void move(glm::vec3 speed_vector, float angle, float pitch, float yaw, glm::vec3& camera_transform, glm::vec3& cameraFront, glm::vec3& cameraPos, glm::vec3 cameraUp);
};

#endif

