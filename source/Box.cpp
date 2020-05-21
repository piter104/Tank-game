#include "include/Box.h"

void Box::setObject(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals)
{
	vertices = out_vertices;
	uvs = out_uvs;
	normals = out_normals;
}

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
	return coordinates + glm::vec3(-1.0f, 0.0f, -1.2f);
}

glm::vec3 Box::getSize()
{
	return box_size;
}

void Box::setCords(glm::vec3 coords)
{
	coordinates = coords;
}

void Box::draw(ShaderProgram *sp, GLuint tex)
{
		glm::mat4 M_skrzynia = glm::mat4(1.0f);
		float color[] = { 1,1,0,1 };
		glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, color);
		

		M_skrzynia = glm::translate(M_skrzynia, coordinates);
		M_skrzynia = glm::scale(M_skrzynia, glm::vec3(0.5f, 0.5f, 0.5f));

		sp->use();
		glUniform4f(sp->u("lp"), -4,3, -4, 1);
		glUniform4f(sp->u("lp2"), -12, 3, -12, 1);
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_skrzynia));

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
}