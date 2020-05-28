#include "include/Bullet.h"

void Bullet::setObject(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals)
{
	vertices = out_vertices;
	uvs = out_uvs;
	normals = out_normals;
}


bool Bullet::shooting(bool shoot_ball)
{
	if (shoot_ball == true && counter < shoot_length)
	{
		shoot = shoot + bullet_vector;
		counter += 1;
		return true;
	}
	else
	{
		shoot = glm::vec3(1.0f, 0.0f, 0.0f);
		collision = false;
		counter = 0;
		first_frame_shot = true;
		return false;
	}
}

void Bullet::generate(glm::mat4 M_lufa, ShaderProgram *sp, GLuint tex, ParticleSystem &system)
{
		if (first_frame_shot == true) {
			M_copy = M_lufa;
			system.initializeSystem(2000);
			first_frame_shot = false;
			
		}
		
		Mp1 = glm::translate(M_copy, glm::vec3(shoot[0] - 0.2f, shoot[1], shoot[2]));
		Mp1 = glm::rotate(Mp1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		Mp1 = glm::scale(Mp1, glm::vec3(bullet_size));

		sp->use();

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mp1));

		glEnableVertexAttribArray(sp->a("vertex"));
		glEnableVertexAttribArray(sp->a("normal"));

		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);

		glEnableVertexAttribArray(sp->a("aTexCoord"));
		glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, &uvs[0]);

		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(sp->u("ourTexture1"), 0);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableVertexAttribArray(sp->a("vertex"));
		glDisableVertexAttribArray(sp->a("normal"));
		glDisableVertexAttribArray(sp->a("aTexCoord"));
}

void Bullet::collision_detector(glm::vec3 object_position, glm::vec3 object_size, bool destroyed)
{
	glm::vec4 bullet_position = Mp1 * Position;

	// kolizja w osi x
	bool collisionX = object_position.x + object_size.x >= bullet_position.x &&
		bullet_position.x + bullet_size.x >= object_position.x;

	// kolizja w osi y
	bool collisionY = object_position.y + object_size.y >= bullet_position.y &&
		bullet_position.y + bullet_size.y >= object_position.y;

	// kolizja w osi z
	bool collisionZ = object_position.z + object_size.z >= bullet_position.z &&
		bullet_position.z + bullet_size.z >= object_position.z;

	if (collisionX && collisionY && collisionZ && !destroyed)
	{
		counter = shoot_length;
		collision = true;
		Mp1 = glm::translate(M_copy, glm::vec3(0.0f, 0.0f, 0.0f));

	}
	else if (!destroyed)
	{
		collision = false;
	}
	else
	{
		collision = true;
	}
}

bool Bullet::hasCollision(glm::vec3 object_position, glm::vec3 object_size, bool destroyed)
{
	collision_detector(object_position, object_size, destroyed);
	return collision;
}
