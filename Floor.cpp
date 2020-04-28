#include "Floor.h"

void Floor::draw_floor(glm::mat4 P, glm::mat4 V, GLuint tex)
{
	//Kod rysuj¹cy
	spTextured->use();
	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V));

	glm::mat4 M_floor = glm::mat4(1.0f);

	M_floor = glm::rotate(M_floor, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Pomnó¿ macierz modelu razy macierz obrotu o k¹t angle wokó³ osi Y
	M_floor = glm::rotate(M_floor, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Pomnó¿ macierz modelu razy macierz obrotu o k¹t angle wokó³ osi X
	M_floor = glm::translate(M_floor, glm::vec3(0.0f, 0.0f, 0.5f));
	M_floor = glm::scale(M_floor, glm::vec3(1000.0f, 1000.0f, 1000.0f));

	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M_floor));

	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, verts);

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords);

	glActiveTexture(GL_TEXTURE0);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spLambertTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("texCoord"));
}
