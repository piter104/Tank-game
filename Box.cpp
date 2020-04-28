#include "Box.h"

glm::vec3 Box::getPosition()
{
	return Position;
}

void Box::draw()
{
	glUniform4f(spLambert->u("color"), 1, 1, 0, 1);
	M_skrzynia = glm::translate(M_skrzynia, glm::vec3(4.0f, 0.0f, -4.0f));
	M_skrzynia = glm::scale(M_skrzynia, glm::vec3(0.5f, 1.0f, 0.5f));

	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M_skrzynia)); //Za³aduj do programu cieniuj¹cego macierz modelu

	Models::cube.drawSolid(); //Narysuj obiekt
}