#include "include/Tank.h"
#include "include/constants.h"
#include "include/allmodels.h"
#include "include/lodepng.h"
#include "include/shaderprogram.h"

void Tank::setObjectBottom(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals)
{
	vertices = out_vertices;
	uvs = out_uvs;
	normals = out_normals;
}

void Tank::setObjectTurret(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals)
{
	vertices2 = out_vertices;
	uvs2 = out_uvs;
	normals2 = out_normals;
}

void Tank::setObjectBarrel(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals)
{
	vertices3 = out_vertices;
	uvs3 = out_uvs;
	normals3 = out_normals;
}

void Tank::setObjectWheel(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals)
{
	vertices4 = out_vertices;
	uvs4 = out_uvs;
	normals4 = out_normals;
}

glm::vec3 Tank::getLufa_cords()
{
	return lufa_cords;
}

//namierzanie obiektu
glm::vec4 Tank::getPosition()
{
	return tank_position;
}

glm::mat4 Tank::getM_wieza()
{
	return M_wieza;
}

glm::mat4 Tank::getM_lufa()
{
	return M_lufa;
}
glm::mat4 Tank::getM()
{
	return M;
}



bool Tank::collision_detector(glm::vec3 object_position, glm::vec3 object_size)
{
	// kolizja w osi x
	bool collisionX = object_position.x + object_size.x >= tank_position.x &&
		tank_position.x + tank_size.x >= object_position.x;

	// kolizja w osi z
	bool collisionY = object_position.z + object_size.z >= tank_position.z &&
		tank_position.z + tank_size.z >= object_position.z;

	return collisionX && collisionY;
}

void Tank::move(glm::vec3 speed_vector, float wheel_speed_left, float wheel_speed_right, float angle, float pitch, float yaw, glm::vec3 &camera_transform, glm::vec3 &cameraFront, glm::vec3 &cameraPos, glm::vec3 cameraUp, ShaderProgram *sp, GLuint tex, GLuint tex2)
{
	glm::mat4 M = glm::mat4(1.0f);

	M = glm::translate(M, speed_vector);
	M = glm::translate(M, glm::vec3(0.0f,0.4f,0.0f));
	M = glm::rotate(M, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	tank_position = M * Position;
	cameraFront = glm::vec3(tank_position[0], tank_position[1], tank_position[2]);
	cameraPos = camera_transform + cameraFront;
	glm::mat4 V = glm::lookAt(cameraPos, cameraFront, cameraUp);
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	sp->use();

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("normal"));

	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);
	
	glEnableVertexAttribArray(sp->a("aTexCoord"));
	glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, &uvs[0]);

	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(sp->u("ourTexture1"), 0);
	

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	M_wieza = glm::translate(M, glm::vec3(0.0f, 0.0f, 0.0f));

	M_wieza = glm::translate(M_wieza, glm::vec3(0.0f, 1.0f, 0.0f));
	M_wieza = glm::rotate(M_wieza, glm::radians(90.0f - angle), glm::vec3(0.0f, 1.0f, 0.0f));
	M_wieza = glm::rotate(M_wieza, glm::radians(pitch), glm::vec3(0.0f, 1.0f, 0.0f));


	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_wieza));

	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices2[0]);
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals2[0]);


	glDrawArrays(GL_TRIANGLES, 0, vertices2.size());

	glm::mat4 M_temp = glm::rotate(M_wieza, glm::radians(yaw), glm::vec3(0.0f, 0.0f, 1.0f));
	
	M_lufa = glm::translate(M_temp, lufa_cords);
	M_lufa = glm::rotate(M_lufa, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	M_lufa = glm::rotate(M_lufa, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_lufa));

	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices3[0]);
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals3[0]);

	glDrawArrays(GL_TRIANGLES, 0, vertices3.size());

	// petla do rysowania kó³ wzd³u¿ boku czo³gu
	for (float i = -2; i < 3; i++)
	{
		//pêtla do rysowania przeciwleg³ych kó³
		for (float j = 0; j < 2; j++)
		{
			M_wheel = glm::translate(M, glm::vec3(10.5 * i / 10, 0.05f, -1.5f + 3.0f*j));
			if (j == 0)
				M_wheel = glm::rotate(M_wheel, glm::radians(40 * -wheel_speed_left), glm::vec3(0.0f, 0.0f, 1.0f));
			else
				M_wheel = glm::rotate(M_wheel, glm::radians(40 * -wheel_speed_right), glm::vec3(0.0f, 0.0f, 1.0f));


			M_wheel = glm::scale(M_wheel, glm::vec3(0.9f, 0.9f, 0.9f));

			glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_wheel));
			
			glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices4[0]);
			glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals4[0]);

			glEnableVertexAttribArray(sp->a("aTexCoord"));
			glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, &uvs4[0]);

			glBindTexture(GL_TEXTURE_2D, tex2);
			glUniform1i(sp->u("ourTexture1"), 0);

			glDrawArrays(GL_TRIANGLES, 0, vertices4.size());
		}
	}
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));

	}