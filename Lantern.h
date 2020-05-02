#ifndef LANTERN_H
#define LANTERN_H

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


class Lantern
{
private:
	int verts = 965;

	glm::vec3 coordinates = glm::vec3(-4.0f, 0.0f, -4.0f);
	glm::vec3 lantern_size = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 Position = glm::vec4(glm::vec3(0.0f), 1.0f);
	glm::mat4 M_lantern = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową

	bool destroyed = false;

	std::vector< glm::vec4 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec4 > normals; // Won't be used at the moment.
	std::vector< glm::vec4 > colors;

public:
	void setObject(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals, std::vector < glm::vec4 > out_colors);
	void destroy();
	bool is_destroyed();
	glm::vec3 getPosition();
	glm::vec3 getSize();
	void draw(ShaderProgram* sp, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
};

#endif