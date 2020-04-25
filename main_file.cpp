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

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"

float speed = 0; //Prędkość kątowa obrotu obiektu
float shoot = 1.0f; //skret kół
float angle = 90.0f;
float bullet_speed = 0.3f;
bool shoot_ball = false;

glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 7.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

	const float cameraSpeed = 0.05f; // adjust accordingly
	const float rotateSpeed = PI/6;
	const float driveSpeed = 0.1f;
	//glm::vec3 cameraForward= glm::vec3(0.0f, 0.0f, driveSpeed);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		speed += driveSpeed;
		cameraPos += driveSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		speed -= driveSpeed;
		cameraPos -= driveSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		angle -= rotateSpeed;
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * driveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		angle += rotateSpeed;
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * driveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		shoot_ball = true;
	}
}

int shooting(int counter)
{
	if (shoot_ball == true && counter < 40)
	{
		shoot += bullet_speed;
		counter += 1;
	}
	else
	{
		shoot = 1.0f;
		shoot_ball = false;
		counter = 0;
	}
	return counter;
}

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0.5, 0.12, 0.9, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
	glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości

	glm::mat4 M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	M = glm::rotate(M, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi Y
	M = glm::translate(M, glm::vec3(speed, 0.0f, 0.0f));
	glm::mat4 V = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania

	spLambert->use(); //Aktywuj program cieniujący


	glUniform4f(spLambert->u("color"), 0, 1, 0, 1); //Ustaw kolor rysowania obiektu
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu

	Models::teapot.drawSolid(); //Narysuj obiekt


	if (shoot_ball == true)
	{
		glm::mat4 Mp1 = glm::translate(M, glm::vec3(shoot, 0.25f, 0.0f)); //...i macierz przesunięcia
		Mp1 = glm::scale(Mp1, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mp1));  //Załadowanie macierzy modelu do programu cieniującego
		glUniform4f(spLambert->u("color"), 0, 1, 0, 1); //Planeta jest zielona

		Models::sphere.drawSolid(); //Narysowanie obiektu
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

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

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

	int counter = 0;

	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		counter = shooting(counter);
		drawScene(window); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
