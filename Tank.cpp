#include "Tank.h"

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

void Tank::move(glm::vec3 speed_vector, float angle, float pitch, float yaw, glm::vec3 &camera_transform, glm::vec3 &cameraFront, glm::vec3 &cameraPos, glm::vec3 cameraUp)
{
	glm::mat4 M = glm::mat4(1.0f);
	
	M = glm::translate(M, speed_vector);
	M = glm::rotate(M, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); //Pomnó¿ macierz modelu razy macierz obrotu o k¹t angle wokó³ osi Y
	M = glm::scale(M, glm::vec3(0.8f, 0.3f, 0.7f));
	tank_position = M * Position;
	cameraFront = glm::vec3(tank_position[0], tank_position[1], tank_position[2]);
	cameraPos = camera_transform + cameraFront;
	glm::mat4 V = glm::lookAt(cameraPos, cameraFront, cameraUp); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M)); //Za³aduj do programu cieniuj¹cego macierz modelu

	Models::cube.drawSolid(); //Narysuj obiekt

	M_wieza = glm::translate(M, glm::vec3(0.0f, 1.5f, 0.0f)); //...i macierz przesuniêcia
	M_wieza = glm::scale(M_wieza, glm::vec3(1/0.8f, 1/0.3f, 1/0.7f));
	M_wieza = glm::rotate(M_wieza, glm::radians(90.0f - angle), glm::vec3(0.0f, 1.0f, 0.0f)); //Pomnó¿ macierz modelu razy macierz obrotu o k¹t angle wokó³ osi Y
	M_wieza = glm::rotate(M_wieza, glm::radians(pitch), glm::vec3(0.0f, 1.0f, 0.0f));
	M_wieza = glm::rotate(M_wieza, glm::radians(yaw), glm::vec3(0.0f, 0.0f, 1.0f));
	M_wieza = glm::scale(M_wieza, glm::vec3(0.8f, 0.3f, 0.7f));

	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M_wieza)); //Za³aduj do programu cieniuj¹cego macierz modelu

	Models::teapot.drawSolid(); //Narysuj obiekt

	glm::mat4 M_lufa = glm::translate(M_wieza, lufa_cords); //...i macierz przesuniêcia
	M_lufa = glm::scale(M_lufa, glm::vec3(0.4f, 0.1f, 0.1f));

	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M_lufa)); //Za³aduj do programu cieniuj¹cego macierz modelu

	Models::cube.drawSolid(); //Narysuj obiekt
}



