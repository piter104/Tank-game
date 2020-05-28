#include "include/Tree.h"

void Tree::setObject(std::vector < glm::vec4 > out_vertices, std::vector < glm::vec2 > out_uvs, std::vector < glm::vec4 > out_normals)
{
	vertices = out_vertices;
	uvs = out_uvs;
	normals = out_normals;
}

void Tree::setCords(glm::vec3 coords)
{
	coordinates = coords;
}

void Tree::draw(ShaderProgram* sp,  GLuint tex)
{
	glm::mat4 M_tree = glm::mat4(1.0f);
	float color[] = { 1,1,0,1 };
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, color);


	M_tree = glm::translate(M_tree, glm::vec3(coordinates));
	M_tree= glm::scale(M_tree, glm::vec3(4.0f, 4.0f, 4.0f));

	sp->use();
	glUniform4f(sp->u("lp"), -4, 3, -4, 1);
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_tree));

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
	glDisableVertexAttribArray(sp->a("aTexCoord"));
}