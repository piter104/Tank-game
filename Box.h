#ifndef BOX_H
#define BOX_H

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


class Box
{
private:
	glm::vec3 coordinates = glm::vec3(4.0f, 0.0f, -4.0f);
	glm::vec3 box_size = glm::vec3(1.0f, 2.0f, 1.0f);
	glm::vec4 Position = glm::vec4(glm::vec3(0.0f), 1.0f);
	glm::mat4 M_skrzynia = glm::mat4(1.0f); //Zainicjuj macierz modelu macierz¹ jednostkow¹

public:
	glm::vec3 getPosition();
	glm::vec3 getSize();
	void draw();
};

#endif