#pragma once
#include <include/model.h>
#include <include\shaderprogram.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


class ParticleSystem
{

private:
	float v[8000];
	float colors_particles[8000];
	
	glm::mat4 M_lufa_copy;

	bool first_frame = true;
	struct Particle {
		glm::vec3 position; //Po³o¿enie cz¹stki
		glm::vec3 speed; //Prêdkoœæ cz¹stki
		glm::vec4 color;
		float ttl; //Czas ¿ycia
	};
	const int n = 2000; //Liczba cz¹stek

	Particle system[2000]; //Tablica cz¹stek

	glm::vec3 gravity = glm::vec3(0.95f, 0.95f, 0.95f); //Wektor grawitacji

	float rand_gen();
	float normalRandom();

	void createParticle(Particle& p);

public:
	int count = 0;
	void initializeSystem(int n);
	void processSystem(int n, float timestep);
	void drawParticles(glm::mat4 P, glm::mat4 V, ShaderProgram* sp, glm::mat4 M_lufa);
	
};
