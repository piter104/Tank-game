#include "Tank.h"


void Tank::setObjectBottom(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals, std::vector < glm::vec4 > out_colors)
{
	vertices = out_vertices;
	uvs = out_uvs;
	normals = out_normals; // Won't be used at the moment.
	colors = out_colors;
}

void Tank::setObjectTurret(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals, std::vector < glm::vec4 > out_colors)
{
	vertices2 = out_vertices;
	uvs2 = out_uvs;
	normals2 = out_normals; // Won't be used at the moment.
	colors2 = out_colors;
}

void Tank::setObjectBarrel(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals, std::vector < glm::vec4 > out_colors)
{
	vertices3 = out_vertices;
	uvs3 = out_uvs;
	normals3 = out_normals; // Won't be used at the moment.
	colors3 = out_colors;
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

glm::mat4 Tank::getM()
{
	return M;
}



bool Tank::collision_detector(glm::vec3 object_position, glm::vec3 object_size)
{
	// Collision x
	bool collisionX = object_position.x + object_size.x >= tank_position.x &&
		tank_position.x + tank_size.x >= object_position.x;

	// Collision z
	bool collisionY = object_position.z + object_size.z >= tank_position.z &&
		tank_position.z + tank_size.z >= object_position.z;

	// Collision only if on both axes
	return collisionX && collisionY;
}

void Tank::move(glm::vec3 speed_vector, float angle, float pitch, float yaw, glm::vec3 &camera_transform, glm::vec3 &cameraFront, glm::vec3 &cameraPos, glm::vec3 cameraUp, ShaderProgram *sp)
{
	glm::mat4 M = glm::mat4(1.0f);


	M = glm::translate(M, speed_vector);
	M = glm::rotate(M, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); //Pomn� macierz modelu razy macierz obrotu o k�t angle wok� osi Y
	tank_position = M * Position;
	cameraFront = glm::vec3(tank_position[0], tank_position[1], tank_position[2]);
	cameraPos = camera_transform + cameraFront;
	glm::mat4 V = glm::lookAt(cameraPos, cameraFront, cameraUp); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	sp->use();//Aktywacja programu cieniuj�cego

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M)); //Za�aduj do programu cieniuj�cego macierz modelu

	glEnableVertexAttribArray(sp->a("vertex"));  //W��cz przesy�anie danych do atrybutu vertex
	glEnableVertexAttribArray(sp->a("color"));
	glEnableVertexAttribArray(sp->a("normal"));

	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]); //Wska� tablic� z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, &colors[0]);
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);


	glDrawArrays(GL_TRIANGLES, 0, vertices.size()); //Narysuj obiekt


	M_wieza = glm::translate(M, glm::vec3(0.0f, 0.0f, 0.0f)); //...i macierz przesuni�cia

	M_wieza = glm::translate(M_wieza, glm::vec3(0.0f, 1.0f, 0.0f));
	M_wieza = glm::rotate(M_wieza, glm::radians(90.0f - angle), glm::vec3(0.0f, 1.0f, 0.0f)); //Pomn� macierz modelu razy macierz obrotu o k�t angle wok� osi Y
	M_wieza = glm::rotate(M_wieza, glm::radians(pitch), glm::vec3(0.0f, 1.0f, 0.0f));
	M_wieza = glm::rotate(M_wieza, glm::radians(yaw), glm::vec3(0.0f, 0.0f, 1.0f));
	//M_wieza = glm::scale(M_wieza, glm::vec3(0.8f, 0.3f, 0.7f));



	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_wieza)); //Za�aduj do programu cieniuj�cego macierz modelu

	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices2[0]); //Wska� tablic� z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, &colors2[0]);
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals2[0]);


	glDrawArrays(GL_TRIANGLES, 0, vertices2.size()); //Narysuj obiekt

	
	glm::mat4 M_lufa = glm::translate(M_wieza, lufa_cords); //...i macierz przesuni�cia
	M_lufa = glm::rotate(M_lufa, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	M_lufa = glm::rotate(M_lufa, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_lufa)); //Za�aduj do programu cieniuj�cego macierz modelu

	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices3[0]); //Wska� tablic� z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, &colors3[0]);
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals3[0]);


	glDrawArrays(GL_TRIANGLES, 0, vertices3.size()); //Narysuj obiekt

	glDisableVertexAttribArray(sp->a("vertex"));  //Wy��cz przesy�anie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));
	glDisableVertexAttribArray(sp->a("normal"));

	}

void Tank::moveTurret(glm::vec3 speed_vector, float angle, float pitch, float yaw, ShaderProgram* sp)
{


}

void Tank::moveBarrel(ShaderProgram* sp)
{



}