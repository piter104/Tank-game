#ifndef BULLET_H
#define BULLET_H

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

class Bullet
{
private:
	float radius = 0.1f;
	int counter = 0;
	bool first_frame_shot = true;
	bool collision;
	glm::vec3 shoot = glm::vec3(1.0f, 0.5f, 0.0f); //spawn kuli
	glm::vec3  bullet_vector = glm::vec3(0.3f, -0.01, 0.0f);
	glm::vec4 Position = glm::vec4(glm::vec3(0.0f), 1.0f);
	glm::mat4 Mp1;
	glm::mat4 M_copy;

	void collision_detector();

public:
	void generate(glm::mat4 M_wieza, glm::vec3 lufa_cords);
	bool shooting(bool shoot_ball);
	bool hasCollision();
};

#endif