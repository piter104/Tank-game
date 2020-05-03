#include "include/Floor.h"

Floor::Floor()
{
	//mSP = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
}

void Floor::draw_floor(glm::mat4 P, glm::mat4 V, GLuint tex, ShaderProgram* sp)
{
	//Kod rysuj¹cy
	//spTextured->use();
	sp->use();
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	glm::mat4 M_floor = glm::mat4(1.0f);
	M_floor = glm::rotate(M_floor, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Pomnó¿ macierz modelu razy macierz obrotu o k¹t angle wokó³ osi X
	M_floor = glm::scale(M_floor, glm::vec3(1000.0f, 1000.0f, 1000.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_floor));

	glUniform4f(sp->u("lp"), -4, 3, -4, 1);
	glUniform4f(sp->u("lp2"), -12, 3, -12, 1);

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts);
	glEnableVertexAttribArray(sp->a("color"));
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors);
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals);

	glEnableVertexAttribArray(sp->a("aTexCoord"));
	glVertexAttribPointer(sp->a("aTexCoord"), 2, GL_FLOAT, false, 0, texCoords);

	glActiveTexture(GL_TEXTURE0);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(sp->u("ourTexture1"), 0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("color"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("aTexCoord"));
	//glDisableVertexAttribArray(spTextured->a("texCoord"));
}