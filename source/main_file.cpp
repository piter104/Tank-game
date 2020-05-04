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

//float v[8000];
//float colors_particles[8000];
//int count = 0;

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
std::vector< glm::vec4 > normals; // Won't be used at the moment.
std::vector< glm::vec4 > colors;

std::vector< glm::vec4 > vertices2;
std::vector< glm::vec2 > uvs2;
std::vector< glm::vec4 > normals2; // Won't be used at the moment.
std::vector< glm::vec4 > colors2;

std::vector< glm::vec4 > vertices3;
std::vector< glm::vec2 > uvs3;
std::vector< glm::vec4 > normals3; // Won't be used at the moment.
std::vector< glm::vec4 > colors3;

std::vector< glm::vec4 > vertices4;
std::vector< glm::vec2 > uvs4;
std::vector< glm::vec4 > normals4; // Won't be used at the moment.
std::vector< glm::vec4 > colors4;

std::vector< glm::vec4 > vertices5;
std::vector< glm::vec2 > uvs5;
std::vector< glm::vec4 > normals5; // Won't be used at the moment.
std::vector< glm::vec4 > colors5;


std::vector< glm::vec4 > vertices6;
std::vector< glm::vec2 > uvs6;
std::vector< glm::vec4 > normals6; // Won't be used at the moment.
std::vector< glm::vec4 > colors6;

std::vector< glm::vec4 > vertices7;
std::vector< glm::vec2 > uvs7;
std::vector< glm::vec4 > normals7; // Won't be used at the moment.
std::vector< glm::vec4 > colors7;

std::vector< glm::vec4 > vertices8;
std::vector< glm::vec2 > uvs8;
std::vector< glm::vec4 > normals8; // Won't be used at the moment.
std::vector< glm::vec4 > colors8;

std::vector< glm::vec4 > vertices9;
std::vector< glm::vec2 > uvs9;
std::vector< glm::vec4 > normals9; // Won't be used at the moment.
std::vector< glm::vec4 > colors9;

ShaderProgram* sp;
ShaderProgram* spf;
ShaderProgram* spl;
ShaderProgram* spt;
ShaderProgram* spp;


//struct Particle {
//	glm::vec3 position; //Położenie cząstki
//	glm::vec3 speed; //Prędkość cząstki
//	glm::vec4 color;
//	float ttl; //Czas życia
//};
//const int n = 2000; //Liczba cząstek
//
//Particle system2[n]; //Tablica cząstek
//
//glm::vec3 gravity = glm::vec3(0, 0, 0); //Wektor grawitacji
//
//float rand_gen() {
//	// return a uniformly distributed random value
//	return ((float)(rand()) + 1.) / ((float)(RAND_MAX)+1.);
//}
//float normalRandom() {
//	// return a normally distributed random value
//	float v1 = rand_gen();
//	float v2 = rand_gen();
//	return cos(2 * 3.14 * v2) * sqrt(-2. * log(v1));
//}
//
//void createParticle(Particle& p) { //Zainicjowanie cząstki
//	p.position = glm::vec3(0, 10.0f, 0);
//	p.speed = glm::vec3(normalRandom() * 0.5 + 0, normalRandom() * 0.5 + 0, normalRandom() * 0.5 + 0);
//	p.color = glm::vec4(glm::clamp((normalRandom() * 0.5f + 1), 0.2f, 1.0f), glm::clamp((normalRandom() * 0.4f + 0.3f), 0.0f, 0.5f), 0.0f, 1.0f);
//	p.ttl = glm::clamp(normalRandom() * 3 + 10, 0.0f, 10.0f);
//}
//void initializeSystem(Particle* system2, int n) {//Zainicjowanie każdej cząstki
//	for (int i = 0; i < n; i++)
//		createParticle(system2[i]);
//	count = n;
//}
//
//void processSystem(Particle* system2, glm::vec3 gravity, int n, float timestep) {
//	if (count > 0) {
//		count = 0;
//		for (int i = 0; i < n; i++)
//		{
//			system2[i].position += system2[i].speed * timestep; //przesunięcie
//			system2[i].color.r = glm::clamp(system2[i].color.r, 0.5f - system2[i].ttl / 30.0f, 0.5f + system2[i].ttl / 30.0f);
//			system2[i].color.g = glm::clamp(system2[i].color.g, 0.5f - system2[i].ttl / 30.0f, 0.5f + system2[i].ttl / 30.0f);
//			system2[i].color.b = glm::clamp(system2[i].color.b, 0.5f - system2[i].ttl / 30.0f, 0.5f + system2[i].ttl / 30.0f);
//			system2[i].speed += gravity * timestep; //uwzględnienie grawitacji
//			system2[i].ttl -= timestep; //Skrócenie czasu życia cząstki
//			if (system2[i].ttl > 0) {
//				v[i * 4] = system2[i].position.x;
//				v[i * 4 + 1] = system2[i].position.y;
//				v[i * 4 + 2] = system2[i].position.z;
//				v[i * 4 + 3] = 1;
//
//				colors_particles[i * 4] = system2[i].color.r;
//				colors_particles[i * 4 + 1] = system2[i].color.g;
//				colors_particles[i * 4 + 2] = system2[i].color.b;
//				colors_particles[i * 4 + 3] = 1.0f;
//				count += 1;
//			}
//		}
//	}
//
//}


bool loadOBJ(const char* path, std::vector < glm::vec4 >& out_vertices, std::vector < glm::vec2 >& out_uvs, std::vector < glm::vec4 >& out_normals, std::vector < glm::vec4 >& out_colors)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec4 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec4 > temp_normals;

#pragma warning(suppress : 4996)
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {
		char lineHeader[128];
		// read the first word of the line
#pragma warning(suppress : 4996)
		int res = fscanf(file, "%s", lineHeader);
		//printf("%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec4 vertex;
#pragma warning(suppress : 4996)
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertex.a = 1.0f;
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
#pragma warning(suppress : 4996)
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec4 normal;
#pragma warning(suppress : 4996)
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normal.a = 0.0f;
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

#pragma warning(suppress : 4996)
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

		// to jest tutaj tylko po to, ¿eby jebn¹æ kolor bo nie umiem w tesktury :/
		glm::vec4 color;
		color.x = 0.08; color.y = 0.6; color.z = 0.25; color.w = 1;
		out_colors.push_back(color);
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

	bool res = loadOBJ("objects/redkin_shell.obj", vertices, uvs, normals, colors);
	printf("%d", res);
	bullet.setObject(vertices, uvs, normals, colors);

	res = loadOBJ("objects/box.obj", vertices2, uvs2, normals2, colors2);
	printf("%d", res);
	box.setObject(vertices2, uvs2, normals2, colors2);

	res = loadOBJ("objects/lamp_bottom.obj", vertices3, uvs3, normals3, colors3);
	printf("%d", res);
	lantern.setBottomObject(vertices3, uvs3, normals3, colors3);
	lantern2.setBottomObject(vertices3, uvs3, normals3, colors3);

	res = loadOBJ("objects/lamp.obj", vertices9, uvs9, normals9, colors9);
	printf("%d", res);

	lantern.setLampObject(vertices9, uvs9, normals9, colors9);
	lantern2.setLampObject(vertices9, uvs9, normals9, colors9);

	res = loadOBJ("objects/bottom.obj", vertices4, uvs4, normals4, colors4);
	printf("%d", res);
	tank.setObjectBottom(vertices4, uvs4, normals4, colors4);

	res = loadOBJ("objects/turret.obj", vertices5, uvs5, normals5, colors5);
	printf("%d", res);
	tank.setObjectTurret(vertices5, uvs5, normals5, colors5);

	res = loadOBJ("objects/lufa.obj", vertices6, uvs6, normals6, colors6);
	printf("%d", res);
	tank.setObjectBarrel(vertices6, uvs6, normals6, colors6);

	res = loadOBJ("objects/wheel3.obj", vertices7, uvs7, normals7, colors7);
	printf("%d", res);
	tank.setObjectWheel(vertices7, uvs7, normals7, colors7);

	res = loadOBJ("objects/tree2.obj", vertices8, uvs8, normals8, colors8);
	printf("%d", res);



	tree.setObject(vertices8, uvs8, normals8, colors8);
	tree2.setObject(vertices8, uvs8, normals8, colors8);

	tree.setCords(glm::vec3(2.0f, 0.0f, -20.0f));
	tree2.setCords(glm::vec3(-17.0f, 0.0f, 10.0f));

	box.setCords(glm::vec3(4.0f, 0.0f, -4.0f));

	lantern.setCords(glm::vec3(-4.0f, 0.0f, -4.0f));
	lantern2.setCords(glm::vec3(-12.0f, 0.0f, -12.0f));

	sp = new ShaderProgram("shaders/v_simplest.glsl", NULL, "shaders/f_simplest.glsl");
	spf = new ShaderProgram("shaders/v_floor.glsl", NULL, "shaders/f_floor.glsl");
	spl = new ShaderProgram("shaders/v_lantern.glsl", NULL, "shaders/f_lantern.glsl");
	spt = new ShaderProgram("shaders/v_text.glsl", NULL, "shaders/f_text.glsl");
	spp = new ShaderProgram("shaders/v_particle.glsl", NULL, "shaders/f_particle.glsl");

	floor_texture.readTexture((char*)"textures/ground.png");
	lamp_bottom_texture.readTexture((char*)"textures/lantern_botom_tex.png");
	lamp_white_texture.readTexture((char*)"textures/lamp_tex.png");
	box_texture.readTexture((char*)"textures/light_wood.png");
	tree_texture.readTexture((char*)"textures/tree2.png");
	bullet_texture.readTexture((char*)"textures/redkin.png");
	wheel_texture.readTexture((char*)"textures/wheel.png");
	tank_texture.readTexture((char*)"textures/tank3.png");

	particleSystem.initializeSystem(2000);
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//freeShaders();
	glDeleteTextures(1, &floor_texture.tex);
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
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

	//printf("%f, %f, %f, %f, %f, %f\n", cameraFront.x, cameraFront.y, cameraFront.z, cameraPos.x, cameraPos.y, cameraPos.z);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości

	spt->use(); //Aktywuj program cieniujący

	glUniform4f(spt->u("lp"), -4, 3.5, -4, 1);
	glUniform4f(spt->u("lp2"), -12, 3, -12, 1);

	tank.move(speed_vector, wheel_speed_left, wheel_speed_right, angle, pitch, yaw, camera_transform, cameraFront, cameraPos, cameraUp, spt, tank_texture.tex, wheel_texture.tex);

	tree.draw(spt, tree_texture.tex);
	tree2.draw(spt, tree_texture.tex);

	glm::mat4 V = glm::lookAt(cameraPos, cameraFront, cameraUp); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania


	if (particleSystem.count == 0 && !shoot_ball) {
		particleSystem.initializeSystem(2000);
	}

	if (shoot_ball == true)
	{
		particleSystem.drawParticles(P, V, spp, tank.getM_lufa());


		bullet.generate(tank.getM_lufa(),  spt, bullet_texture.tex);
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

	glUniform4f(sp->u("color"), 0, 1, 0, 1);

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