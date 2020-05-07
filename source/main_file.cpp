/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.
Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.
Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <algorithm>
#include <fstream>
#include <istream>
#include <string>
#include <iostream>

#include "include/Tree.h"
#include "include/Bullet.h"
#include "include/Tank.h"
#include "include/Box.h"
#include "include/Floor.h"
#include "include/Texture.h"
#include "include/Lantern.h"
#include "include/shaderprogram.h"
#include "include/ParticleSystem.h"


Bullet bullet = Bullet();
Tank tank = Tank();
Box box = Box();
Tree tree = Tree();
Tree tree2 = Tree();
Lantern lantern = Lantern();
Lantern lantern2 = Lantern();
Texture floor_texture = Texture();
Texture lamp_bottom_texture = Texture();
Texture lamp_white_texture = Texture();
Texture box_texture = Texture();
Texture tree_texture = Texture();
Texture bullet_texture = Texture();
Texture wheel_texture = Texture();
Texture tank_texture = Texture();
Floor ground = Floor();
ParticleSystem particleSystem = ParticleSystem();


float angle = 90.0f;
bool shoot_ball = false;
bool fisrt_frame_shot = true;
float lastX = 400;
float lastY = 300;
float yaw = 0.0f;
float pitch = 0.0f;
float yaw_limit_down = 0.0f;
float yaw_limit_up = 10.0f;
float wheel_speed_right = 0.0f;
float wheel_speed_left = 0.0f;

const float movingSpeed = 0.1f;
const float rotateSpeed = PI / 2;

bool w_press = false;
bool s_press = false;
bool a_press = false;
bool d_press = false;

glm::vec3 cameraPos = glm::vec3(0.0f, 4.0f, 12.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec4 tank_position = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
glm::vec3 speed_vector = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_transform = glm::vec3(0.0f, 4.0f, 12.0f);

std::vector< glm::vec4 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec4 > normals;

ShaderProgram* spf;
ShaderProgram* spt;
ShaderProgram* spp;


bool loadOBJ(const char* path, std::vector < glm::vec4 >& out_vertices, std::vector < glm::vec2 >& out_uvs, std::vector < glm::vec4 >& out_normals)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec4 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec4 > temp_normals;

	out_vertices.clear();
	out_uvs.clear();
	out_normals.clear();

	//#pragma warning(suppress : 4996)
	#pragma warning (disable : 4996)
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec4 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertex.a = 1.0f;
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec4 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normal.a = 0.0f;
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec4 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);

		unsigned int uvsIndex = uvIndices[i];
		glm::vec2 uvs = temp_uvs[uvsIndex - 1];
		out_uvs.push_back(uvs);

		unsigned int normalIndex = normalIndices[i];
		glm::vec4 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
	return true;
}

void freeOpenGLProgram(GLFWwindow* window);


//Procedura obsługi myszki
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = lastX - xpos;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += yoffset;
	pitch += xoffset;

	if (yaw > yaw_limit_up)
		yaw = yaw_limit_up;
	if (yaw < yaw_limit_down)
		yaw = yaw_limit_down;

	glm::vec3 direction;
	direction.x = camera_transform[0] * cos(glm::radians(xoffset)) + camera_transform[2] * sin(glm::radians(xoffset));
	direction.z = -camera_transform[0] * sin(glm::radians(xoffset)) + camera_transform[2] * cos(glm::radians(xoffset));
	direction.y = 4.0f;
	camera_transform = direction;
}

//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	const float cameraSpeed = 0.001f;


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		w_press = true;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
		w_press = false;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		s_press = true;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
		s_press = false;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		a_press = true;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
		a_press = false;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		d_press = true;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
		d_press = false;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		shoot_ball = true;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		freeOpenGLProgram(window);

		glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
		glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
		exit(EXIT_SUCCESS);
	}
}


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0.3, 0.8, 1, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback); //Zarejestruj procedurę obsługi myszki

	glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury

	bool res = loadOBJ("objects/bullet.obj", vertices, uvs, normals);
	printf("Loaded bullet.obj %d\n", res);
	bullet.setObject(vertices, uvs, normals);

	res = loadOBJ("objects/box.obj", vertices, uvs, normals);
	printf("Loaded box.obj %d\n", res);
	box.setObject(vertices, uvs, normals);

	res = loadOBJ("objects/lampa_bottom.obj", vertices, uvs, normals);
	printf("Loaded lampa_bottom.obj %d\n", res);
	lantern.setBottomObject(vertices, uvs, normals);
	lantern2.setBottomObject(vertices, uvs, normals);

	res = loadOBJ("objects/lamp.obj", vertices, uvs, normals);
	printf("Loaded lamp.obj %d\n", res);

	lantern.setLampObject(vertices, uvs, normals);
	lantern2.setLampObject(vertices, uvs, normals);

	res = loadOBJ("objects/bottom.obj", vertices, uvs, normals);
	printf("Loaded bottom.obj %d\n", res);
	tank.setObjectBottom(vertices, uvs, normals);

	res = loadOBJ("objects/turret.obj", vertices, uvs, normals);
	printf("Loaded turret.obj %d\n", res);
	tank.setObjectTurret(vertices, uvs, normals);

	res = loadOBJ("objects/lufa.obj", vertices, uvs, normals);
	printf("Loaded lufa.obj %d\n", res);
	tank.setObjectBarrel(vertices, uvs, normals);

	res = loadOBJ("objects/wheel.obj", vertices, uvs, normals);
	printf("Loaded wheel.obj %d\n", res);
	tank.setObjectWheel(vertices, uvs, normals);

	res = loadOBJ("objects/tree.obj", vertices, uvs, normals);
	printf("Loaded tree.obj %d\n", res);
	tree.setObject(vertices, uvs, normals);
	tree2.setObject(vertices, uvs, normals);

	tree.setCords(glm::vec3(2.0f, 0.0f, -20.0f));
	tree2.setCords(glm::vec3(-17.0f, 0.0f, 10.0f));

	box.setCords(glm::vec3(4.0f, 0.0f, -4.0f));

	lantern.setCords(glm::vec3(-4.0f, 0.0f, -4.0f));
	lantern2.setCords(glm::vec3(-12.0f, 0.0f, -12.0f));

	spf = new ShaderProgram("shaders/v_floor.glsl", NULL, "shaders/f_floor.glsl");
	spt = new ShaderProgram("shaders/v_text.glsl", NULL, "shaders/f_text.glsl");
	spp = new ShaderProgram("shaders/v_particle.glsl", NULL, "shaders/f_particle.glsl");

	floor_texture.readTexture((char*)"textures/ground.png");
	printf("Loaded ground.png\n");
	lamp_bottom_texture.readTexture((char*)"textures/lantern_botom.png");
	printf("Loaded lantern_botom.png\n");
	lamp_white_texture.readTexture((char*)"textures/lamp.png");
	printf("Loaded lamp.png\n");
	box_texture.readTexture((char*)"textures/light_wood.png");
	printf("Loaded light_wood.png\n");
	tree_texture.readTexture((char*)"textures/tree.png");
	printf("Loaded tree.png\n");
	bullet_texture.readTexture((char*)"textures/bullet.png");
	printf("Loaded bullet.png\n");
	wheel_texture.readTexture((char*)"textures/wheel.png");
	printf("Loaded wheel.png\n");
	tank_texture.readTexture((char*)"textures/tank.png");
	printf("Loaded tank.png\n");
	particleSystem.initializeSystem(2000);
	printf("Loaded particleSystem\n");
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//freeShaders();
	glDeleteTextures(1, &floor_texture.tex);
	glDeleteTextures(1, &lamp_bottom_texture.tex);
	glDeleteTextures(1, &lamp_white_texture.tex);
	glDeleteTextures(1, &box_texture.tex);
	glDeleteTextures(1, &tree_texture.tex);
	glDeleteTextures(1, &bullet_texture.tex);
	glDeleteTextures(1, &wheel_texture.tex);
	glDeleteTextures(1, &tank_texture.tex);
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************
	if (w_press) {
		speed_vector.z -= movingSpeed * sin(angle * PI / 180);
		speed_vector.x += movingSpeed * cos(angle * PI / 180);
		wheel_speed_left += movingSpeed;
		wheel_speed_right += movingSpeed;

	}
	if (s_press) {
		speed_vector.z += movingSpeed * sin(angle * PI / 180);
		speed_vector.x -= movingSpeed * cos(angle * PI / 180);
		wheel_speed_left -= movingSpeed;
		wheel_speed_right -= movingSpeed;
	}
	if (a_press) {
		angle += rotateSpeed;
		wheel_speed_right += movingSpeed * 0.5f;
		wheel_speed_left -= movingSpeed * 0.5f;
	}
	if (d_press) {
		angle -= rotateSpeed;
		wheel_speed_right -= movingSpeed * 0.5f;
		wheel_speed_left += movingSpeed * 0.5f;
	}
	tank_position = tank.getPosition();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości

	spt->use(); //Aktywuj program cieniujący

	glUniform4f(spt->u("lp"), -4, 3.5, -4, 1);
	glUniform4f(spt->u("lp2"), -12, 3, -12, 1);

	tank.move(speed_vector, wheel_speed_left, wheel_speed_right, angle, pitch, yaw, camera_transform, cameraFront, cameraPos, cameraUp, spt, tank_texture.tex, wheel_texture.tex);

	tree.draw(spt, tree_texture.tex);
	tree2.draw(spt, tree_texture.tex);

	glm::mat4 V = glm::lookAt(cameraPos, cameraFront, cameraUp); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania

	//if (particleSystem.count == 0 && !shoot_ball) {
	//	particleSystem.initializeSystem(2000);
	//}

	if (shoot_ball == true)
	{
		particleSystem.drawParticles(P, V, spp, tank.getM_lufa());
		bullet.generate(tank.getM_lufa(),  spt, bullet_texture.tex, particleSystem);
	}

	shoot_ball = bullet.shooting(shoot_ball);

	lantern.draw(spf, lamp_bottom_texture.tex,lamp_white_texture.tex, cameraPos, cameraFront, cameraUp);
	lantern2.draw(spf, lamp_bottom_texture.tex, lamp_white_texture.tex, cameraPos, cameraFront, cameraUp);

	ground.draw_floor(P, V, floor_texture.tex, spt);

	if (!bullet.hasCollision(box.getPosition(), box.getSize(), box.is_destroyed()))
	{
		box.draw(spt, box_texture.tex);
	}
	else if (box.is_destroyed() == false)
	{
		box.destroy();
	}

	glfwSwapBuffers(window); //Skopiuj bufor tylny do bufora przedniego
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1000, 800, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące


	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		drawScene(window); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}