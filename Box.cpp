#include "Box.h"

void Box::destroy()
{
	destroyed = true;
}

bool Box::is_destroyed()
{
	return destroyed;
}

glm::vec3 Box::getPosition()
{
	return coordinates;
}

glm::vec3 Box::getSize()
{
	return box_size;
}

void Box::draw(ShaderProgram *sp)
{
		glm::mat4 M_skrzynia = glm::mat4(1.0f);
		float color[] = { 1,1,0,1 };
		glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, color);
		M_skrzynia = glm::translate(M_skrzynia, coordinates);
		M_skrzynia = glm::scale(M_skrzynia, glm::vec3(0.5f, 1.0f, 0.5f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_skrzynia)); //Za³aduj do programu cieniuj¹cego macierz modelu

		Models::cube.drawSolid(); //Narysuj obiekt
}