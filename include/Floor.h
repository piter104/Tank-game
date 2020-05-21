#pragma once

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

class Floor
{

public:

	float verts[24] = {
	  1.0f,-1.0f,0.0f,1.0f, //A
	 -1.0f, 1.0f,0.0f,1.0f, //B
	 -1.0f,-1.0f,0.0f,1.0f, //C

	  1.0f,-1.0f,0.0f,1.0f, //A
	  1.0f, 1.0f,0.0f,1.0f, //D
	 -1.0f, 1.0f,0.0f,1.0f, //B
	};

	float colors[24] = {
 0.73f,0.5f,0.3f,1.0f, //A
 0.73f,0.5f,0.3f,1.0f, //A
 0.73f,0.5f,0.3f,1.0f, //A
  0.73f,0.5f,0.3f,1.0f, //A
   0.73f,0.5f,0.3f,1.0f, //A
	0.73f,0.5f,0.3f,1.0f, //A
	};

	float normals[24] = {
				0.0f, 0.0f,-1.0f,0.0f,
				0.0f, 0.0f,-1.0f,0.0f,
				0.0f, 0.0f,-1.0f,0.0f,

				0.0f, 0.0f,-1.0f,0.0f,
				0.0f, 0.0f,-1.0f,0.0f,
				0.0f, 0.0f,-1.0f,0.0f,
	};

	//Tablica wspólrzednych teksturowania
	float texCoords[12] = {
	  200.0f, 0.0f,   //A
	  0.0f, 200.0f,    //B
	  0.0f, 0.0f,    //C

	  200.0f, 0.0f,    //A
	  200.0f, 200.0f,    //D
	  0.0f, 200.0f,    //B
	};

	//Liczba wierzcholków w tablicy
	int vertexCount = 6;

	Floor();

	void draw_floor(glm::mat4 P, glm::mat4 V, GLuint tex, ShaderProgram* sp);

};