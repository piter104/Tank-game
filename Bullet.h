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

	int shoot_length = 100;
	int verts = 8000;

	glm::vec3 bullet_size = glm::vec3(0.8f, 0.5f, 0.7f);
	glm::vec3 shoot = glm::vec3(1.0f, 0.5f, 0.0f); //spawn kuli
	glm::vec3  bullet_vector = glm::vec3(0.3f, -0.01, 0.0f);
	glm::vec4 Position = glm::vec4(glm::vec3(0.0f), 1.0f);
	glm::mat4 Mp1;
	glm::mat4 M_copy;

	std::vector< glm::vec4 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec4 > normals; // Won't be used at the moment.
	std::vector< glm::vec4 > colors;

	void collision_detector(glm::vec3 object_position, glm::vec3 object_size, bool destroyed);

public:
	void setObject(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals, std::vector < glm::vec4 > out_colors);
	void generate(glm::mat4 M_wieza, glm::vec3 lufa_cords, ShaderProgram *sp);
	bool shooting(bool shoot_ball);
	bool hasCollision(glm::vec3 object_position, glm::vec3 object_size, bool destroyed);
};

#endif