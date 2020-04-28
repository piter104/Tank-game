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


#include "Bullet.h"
#include "Tank.h"
#include "Box.h"

Bullet bullet = Bullet();
Tank tank = Tank();
Box box = Box();

float speed = 0; //predkosc czołgu

float angle = 90.0f;
bool shoot_ball = false;
bool fisrt_frame_shot = true;
float lastX = 400;
float lastY = 300;
float yaw = 0.0f;
float pitch = 0.0f;
float yaw_limit_down = 0.0f;
float yaw_limit_up = 15.0f;

const float movingSpeed = 0.1f;
const float rotateSpeed = PI / 2;

bool w_press = false;
bool s_press = false;
bool a_press = false;
bool d_press = false;

glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 7.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec4 Position = glm::vec4(glm::vec3(0.0f), 1.0f);
glm::vec4 tank_position = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

glm::vec3 speed_vector = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 camera_transform = glm::vec3(0.0f, 2.0f, 7.0f);

glm::mat4 M_copy;

GLuint tex; //Uchwyt – deklaracja globalna

void freeOpenGLProgram(GLFWwindow* window);


GLuint readTexture(char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

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
	direction.y = 2.0f;
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
	initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0.3, 0.8, 1, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback); //Zarejestruj procedurę obsługi myszki

	glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury

	tex = readTexture((char*)"ground.png");
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	glDeleteTextures(1, &tex);
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************


	if (w_press) {
		speed -= movingSpeed;
		speed_vector.z -= movingSpeed * sin(angle * PI / 180);
		speed_vector.x += movingSpeed * cos(angle * PI / 180);
	}
	if (s_press) {
		speed += movingSpeed;
		speed_vector.z += movingSpeed * sin(angle * PI / 180);
		speed_vector.x -= movingSpeed * cos(angle * PI / 180);
	}
	if (a_press) {
		angle += rotateSpeed;
	}
	if (d_press) {
		angle -= rotateSpeed;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości

	tank_position = tank.getPosition();
	cameraFront = glm::vec3(tank_position[0], tank_position[1], tank_position[2]);
	cameraPos = camera_transform + cameraFront;
	glm::mat4 V = glm::lookAt(cameraPos, cameraFront, cameraUp); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania

	tank.move(speed_vector, angle, pitch, yaw);



	//wspolrzedne
	//printf("%f, %f, %f, %f\n", Transformed[0], Transformed[1], Transformed[2], Transformed[3]);

	//Opis modelu
	//Tablica współrzędnych wierzchołków
	float verts[] = {
	  1.0f,-1.0f,0.0f,1.0f, //A
	 -1.0f, 1.0f,0.0f,1.0f, //B
	 -1.0f,-1.0f,0.0f,1.0f, //C

	  1.0f,-1.0f,0.0f,1.0f, //A
	  1.0f, 1.0f,0.0f,1.0f, //D
	 -1.0f, 1.0f,0.0f,1.0f, //B
	};

	//Tablica współrzędnych teksturowania
	float texCoords[] = {
	  100.0f, 0.0f,   //A
	  0.0f, 100.0f,    //B
	  0.0f, 0.0f,    //C

	  10.0f, 0.0f,    //A
	  100.0f, 100.0f,    //D
	  0.0f, 100.0f,    //B
	};

	//Liczba wierzchołków w tablicy
	int vertexCount = 6;

	//Kod rysujący
	spTextured->use();
	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V));

	glm::mat4 M_floor = glm::mat4(1.0f);

	M_floor = glm::rotate(M_floor, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi Y
	M_floor = glm::rotate(M_floor, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
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

	spLambert->use(); //Aktywuj program cieniujący

	glUniform4f(spLambert->u("color"), 0, 1, 0, 1); //Ustaw kolor rysowania obiektu
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(tank.getM())); //Załaduj do programu cieniującego macierz modelu


	if (!bullet.hasCollision(box.getPosition(), box.getSize()))
	{
		box.draw();
	}
	glUniform4f(spLambert->u("color"), 0, 1, 0, 1);


	if (shoot_ball == true)
	{
		bullet.generate(tank.getM_wieza(), tank.getLufa_cords());
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


	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		shoot_ball = bullet.shooting(shoot_ball);
		drawScene(window); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
