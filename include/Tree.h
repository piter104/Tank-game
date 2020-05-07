#ifndef TREE_H
#define TREE_H

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

class Tree
{
private:
	glm::vec3 coordinates = glm::vec3(4.0f, 0.0f, -4.0f);

	std::vector< glm::vec4 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec4 > normals; // Won't be used at the moment.

public:
	void setObject(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals);
	void setCords(glm::vec3 coords);
	void draw(ShaderProgram* sp, GLuint tex);
};

#endif