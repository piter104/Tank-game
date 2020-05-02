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
	glm::vec3 tank_size = glm::vec3(1.6f, 0.6f, 1.4f);
	glm::vec4 Position = glm::vec4(glm::vec3(0.0f), 1.0f);
	glm::vec4 tank_position = glm::vec4(glm::vec3(0.0f), 1.0f);

	std::vector< glm::vec4 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec4 > normals; // Won't be used at the moment.
	std::vector< glm::vec4 > colors;

	std::vector< glm::vec4 > vertices2;
	std::vector< glm::vec2 > uvs2;
	std::vector< glm::vec4 > normals2; // Won't be used at the moment.
	std::vector< glm::vec4 > colors2;

	std::vector< glm::vec4 > vertices3;
	std::vector< glm::vec2 > uvs3;
	std::vector< glm::vec4 > normals3; // Won't be used at the moment.
	std::vector< glm::vec4 > colors3;

public:
	void setObjectBottom(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals, std::vector < glm::vec4 > out_colors);
	void setObjectTurret(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals, std::vector < glm::vec4 > out_colors);
	void setObjectBarrel(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals, std::vector < glm::vec4 > out_colors);
	glm::vec3 getLufa_cords();
	glm::vec4 getPosition();
	glm::mat4 getM_wieza();
	glm::mat4 getM();
	bool collision_detector(glm::vec3 object_position, glm::vec3 object_size);
	void move(glm::vec3 speed_vector, float angle, float pitch, float yaw, glm::vec3& camera_transform, glm::vec3& cameraFront, glm::vec3& cameraPos, glm::vec3 cameraUp, ShaderProgram* sp);
	void moveTurret(glm::vec3 speed_vector, float angle, float pitch, float yaw, ShaderProgram* sp);
	void moveBarrel(ShaderProgram* sp);
};

#endif

