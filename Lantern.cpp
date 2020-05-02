#include "Lantern.h"

void Lantern::setObject(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals, std::vector < glm::vec4 > out_colors)
{
	vertices = out_vertices;
	uvs = out_uvs;
	normals = out_normals; // Won't be used at the moment.
	colors = out_colors;
}

void Lantern::destroy()
{
	destroyed = true;
}

bool Lantern::is_destroyed()
{
	return destroyed;
}

glm::vec3 Lantern::getPosition()
{
	return coordinates;
}

glm::vec3 Lantern::getSize()
{
	return lantern_size;
}

void Lantern::draw(ShaderProgram* sp, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
{
	glm::mat4 M_lantern = glm::mat4(1.0f);
	glm::mat4 V = glm::lookAt(cameraPos, cameraFront, cameraUp); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania
	float color[] = { 1,1,0,1 };
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, color);
	M_lantern = glm::translate(M_lantern, coordinates);
	M_lantern = glm::scale(M_lantern, glm::vec3(0.3f, 0.3f, 0.3f));

	sp->use();//Aktywacja programu cieniuj¹cego

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_lantern));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glEnableVertexAttribArray(sp->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
	glEnableVertexAttribArray(sp->a("color"));
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]); //Wska¿ tablicê z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, &colors[0]);
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);
	printf("%u\n", vertices.size());
	printf("%u\n", colors.size());
	printf("%u\n", normals.size());


	glDrawArrays(GL_TRIANGLES, 0, 1476); //Narysuj obiekt
	//glDrawElements(GL_TRIANGLES, 705, GL_UNSIGNED_INT, indexes);

	glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));
	glDisableVertexAttribArray(sp->a("normal"));
}