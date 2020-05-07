#include "include/Lantern.h"

void Lantern::setBottomObject(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals)
{
	vertices = out_vertices;
	uvs = out_uvs;
	normals = out_normals; // Won't be used at the moment.
}

void Lantern::setLampObject(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals)
{
	vertices2 = out_vertices;
	uvs2 = out_uvs;
	normals2 = out_normals; // Won't be used at the moment.
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

void Lantern::setCords(glm::vec3 coords)
{
	coordinates = coords;
}


void Lantern::draw(ShaderProgram* sp, ShaderProgram* spl, GLuint tex, GLuint tex2, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
{
	glm::mat4 M_lantern = glm::mat4(1.0f);
	glm::mat4 V = glm::lookAt(cameraPos, cameraFront, cameraUp); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania

	M_lantern = glm::translate(M_lantern, coordinates);
	M_lantern = glm::scale(M_lantern, glm::vec3(0.3f, 0.3f, 0.3f));

	sp->use();//Aktywacja programu cieniuj¹cego
	glUniform4f(sp->u("lp"),-4, 3.5, -4, 1);
	glUniform4f(sp->u("lp2"), -12, 3.5, -12, 1);

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_lantern));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glEnableVertexAttribArray(sp->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
	glEnableVertexAttribArray(sp->a("normal"));

	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]); //Wska¿ tablicê z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);

	glEnableVertexAttribArray(sp->a("aTexCoord"));
	glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, &uvs[0]);

	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(sp->u("ourTexture1"), 0);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size()); //Narysuj obiekt
	glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("normal"));

	spl->use();

	glm::mat4 M_lamp = M_lantern;
	M_lamp = glm::translate(M_lamp, glm::vec3(0.0f,13.2f,0.0f));
	//M_lamp = glm::scale(M_lamp, glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(spl->u("M"), 1, false, glm::value_ptr(M_lamp));
	glUniformMatrix4fv(spl->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spl->u("P"), 1, false, glm::value_ptr(P));
	glEnableVertexAttribArray(spl->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
	glVertexAttribPointer(spl->a("vertex"), 4, GL_FLOAT, false, 0, &vertices2[0]); //Wska¿ tablicê z danymi dla atrybutu vertex


	glEnableVertexAttribArray(spl->a("aTexCoord"));
	glVertexAttribPointer(spl->a("aTexCoord"), 2, GL_FLOAT, false, 0, &uvs2[0]);

	glBindTexture(GL_TEXTURE_2D, tex2);
	glUniform1i(spl->u("ourTexture1"), 0);

	glDrawArrays(GL_TRIANGLES, 0, vertices2.size()); //Narysuj obiekt

	glDisableVertexAttribArray(spl->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex

}