#include "include/ParticleSystem.h"

float ParticleSystem::rand_gen() {
	return ((float)(rand()) + 1.) / ((float)(RAND_MAX)+1.);
}

float ParticleSystem::normalRandom() {
	float v1 = rand_gen();
	float v2 = rand_gen();
	return cos(2 * 3.14 * v2) * sqrt(-2. * log(v1));
}

void ParticleSystem::createParticle(Particle& p) 
{
	p.position = glm::vec3(0, 10.0f, 0);
	p.speed = glm::vec3(normalRandom() * 1.5f + 0, normalRandom() * 1.5f + 0, normalRandom() * 1.5f + 0);
	p.color = glm::vec4(glm::clamp((normalRandom() * 0.5f + 1), 0.2f, 1.0f), glm::clamp((normalRandom() * 0.4f + 0.3f), 0.0f, 0.5f), 0.0f, 1.0f);
	p.ttl = glm::clamp(normalRandom() * 4 + 9, 0.0f, 10.0f);
}

void ParticleSystem::initializeSystem( int n) 
{
	for (int i = 0; i < n; i++)
		createParticle(system[i]);
	count = n;
	first_frame = true;
}

void ParticleSystem::processSystem(int n, float timestep) 
{
	if (count > 0) {
		count = 0;
		for (int i = 0; i < n; i++)
		{
			system[i].position += system[i].speed * timestep;
			system[i].color.r = glm::clamp(system[i].color.r, 0.5f - system[i].ttl / 30.0f, 0.5f + system[i].ttl / 30.0f);
			system[i].color.g = glm::clamp(system[i].color.g, 0.5f - system[i].ttl / 30.0f, 0.5f + system[i].ttl / 30.0f);
			system[i].color.b = glm::clamp(system[i].color.b, 0.5f - system[i].ttl / 30.0f, 0.5f + system[i].ttl / 30.0f);
			system[i].speed *= gravity;

			system[i].ttl -= timestep;
			if (system[i].ttl > 0) {
				v[i * 4] = system[i].position.x;
				v[i * 4 + 1] = system[i].position.y;
				v[i * 4 + 2] = system[i].position.z;
				v[i * 4 + 3] = 1;

				colors_particles[i * 4] = system[i].color.r;
				colors_particles[i * 4 + 1] = system[i].color.g;
				colors_particles[i * 4 + 2] = system[i].color.b;
				colors_particles[i * 4 + 3] = 1.0f;
				count += 1;
			}
		}
	}

}

void ParticleSystem::drawParticles(glm::mat4 P, glm::mat4 V, ShaderProgram* sp, glm::mat4 M_lufa)
{
	processSystem(2000, 0.2f);
	sp->use();
	if (first_frame) {
		M_lufa_copy = M_lufa;
		first_frame = false;
	}
	
	glm::mat4 M_Particle = M_lufa_copy;
	M_Particle = glm::translate(M_Particle, glm::vec3(0.0f, -0.51f, 2.5f));
	M_Particle = glm::scale(M_Particle, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_Particle));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, v);

	glEnableVertexAttribArray(sp->a("color"));
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors_particles);

	glDrawArrays(GL_POINTS, 0, count);
}
