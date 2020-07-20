//============================================================================
// Name        : Dibujo de Triangulo por puntos ingresados en Codigo
// Professor   : Herminio Paucar
// Version     :
// Description : Utilizamos los Vertex y Fragment Shaders
//============================================================================

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <glm/vec3.hpp>//Nueva librerria para que funcione

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>

GLuint renderingProgram;

GLfloat* m_Vertices;
GLuint m_VBO;
GLuint m_VAO;

using namespace std;

int numberOfVertices;//Variable global

// displays the contents of OpenGL's log when GLSL compilation failed
void printShaderLog(GLuint shader) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        cout << "Shader Info Log: " << log << endl;
        free(log);
    }
}

// displays the contents of OpenGL's log when GLSL linking failed
void printProgramLog(int prog) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        cout << "Program Info Log: " << log << endl;
        free(log);
    }
}

// checks the OpenGL error flag for the occurrrence of an OpenGL error
// detects both GLSL compilation errors and OpenGL runtime errors
bool checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        cout << "glError: " << glErr << endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

string readShaderSource(const char *filePath) {
    string content = "";
    ifstream fileStream(filePath, ios::in);
//    cerr << "Error: " << strerror(errno) << endl;  // No such file or directory
//    cout << fileStream.is_open() << endl;  // 0
    string line = "";
    while (!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

GLuint createShaderProgram() {
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    string vertShaderStr = readShaderSource("src/vertShader.glsl");
    string fragShaderStr = readShaderSource("src/fragShader.glsl");

    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);

    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1) {
        cout << "vertex compilation failed" << endl;
        printShaderLog(vShader);
    }

    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1) {
        cout << "fragment compilation failed" << endl;
        printShaderLog(fShader);
    }

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);

    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        cout << "linking failed" << endl;
        printProgramLog(vfProgram);
    }

    return vfProgram;
}


void init (GLFWwindow* window) {
    renderingProgram = createShaderProgram();

    numberOfVertices = 26; /*Son 13 vertices por las 4 figuras mas los 13 puntos que se van a pintar
     	 	 	 	 	 	 atte. Jesus Leon*/

    //Triangulo rectangulo
    glm::vec3 p1(-0.75f,0.65f,0.0f);
    glm::vec3 p1A = p1 +glm::vec3(-0.0, -0.55, 0.0);
    glm::vec3 p1B = p1 +glm::vec3(0.65, -0.55, 0.0);
    glm::vec3 c1(1.0f,0.0f,0.0f);

    //Triangulo escaleno
    glm::vec3 p0(0.9f,0.65f,0.0f);
    glm::vec3 p0A = p0 +glm::vec3(-0.1, -0.55, 0.0);
    glm::vec3 p0B = p0 +glm::vec3(-0.4, -0.55, 0.0);
    glm::vec3 c0(1.0f,0.3f,1.0f);

    //Isoceles
    glm::vec3 p2(-0.5f,-0.2f,0.0f);
    glm::vec3 p2A = p2 +glm::vec3(-0.4, -0.6, 0.0);
    glm::vec3 p2B = p2 +glm::vec3(0.4, -0.6, 0.0);
    glm::vec3 c2(0.0f,1.0f,0.0f);

    //Rectangulo
    glm::vec3 p3(0.2f,-0.2f,0.0f);
    glm::vec3 p3A = p3 +glm::vec3(0.7, 0.0, 0.0);
    glm::vec3 p3B = p3 +glm::vec3(0.0, -0.6, 0.0);
    glm::vec3 p3C = p3 +glm::vec3(0.7, -0.6, 0.0);
    glm::vec3 c3(0.6f,0.6f,0.0f);


    glm::vec3 m_Vertices[3*numberOfVertices]{
    	p0, c0, p0A, c0, p0B, c0,
    	p1, c1, p1A, c1, p1B, c1,
    	p2, c2, p2A, c2, p2B, c2,
    	p3, c3, p3A, c3, p3B, c3,p3C, c3
    };//Aca proyectamos las figuras con sus colores


	// Cria um ID na GPU para nosso buffer
	glGenBuffers(1, &m_VBO);

	// Cria um ID na GPU para um array de  buffers
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// Reserva memoria na GPU para um TARGET receber dados
	// Copia esses dados pra essa 疵ea de memoria
	glBufferData(
			GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
			250*sizeof(GLfloat),	// tamanho do buffer
			m_Vertices,			// Dados a serem copiados pra GPU
			GL_STATIC_DRAW		// Pol咜ica de acesso aos dados, para otimiza鈬o
		);

	// Nesse ponto, ja copiamos nossos dados pra GPU.
	// Mas, OpenGL nao faz ideia do que eles representam
	// Sao 6 v駻tices ? 3 v駻ices e 3 cores ?
	// Os vertices vem antes ou depois das cores ?
	// Ou est縊 intercalados v駻tices depois cores, depois vertices, depois cores novamente....
	// Precisamos dizer com nossos dados precisam ser interpretados:
	glVertexAttribPointer(
			0,			// Lembra do (layout = 0 ) no vertex shader ? Esse valor indica qual atributo estamos indicando
			3,			// cada vertice � composto de 3 valores
			GL_FLOAT,	// cada valor do v駻tice � do tipo GLfloat
			GL_FALSE,	// Quer normalizar os dados e converter tudo pra NDC ? ( no nosso caso, j� esta tudo correto, ent縊 deixamos como FALSE)
			6 * sizeof(GLfloat),// De quantos em quantos bytes, este atributo � encontrado no buffer ? No nosso caso 3 floats pros vertices + 3 floats pra cor = 6 floats
			(GLvoid*) 0	// Onde est� o primeiro valor deste atributo no buffer. Nesse caso, est� no in兤io do buffer
		);

	glEnableVertexAttribArray(0);	// Habilita este atributo

	// Faremos a mesma coisa pra cor de cada v駻tice
	glVertexAttribPointer(
			1,			// Lembra do (layout = 1 ) no vertex shader ? Esse valor indica qual atributo estamos indicando
			3,			// cada vertice � composto de 3 valores
			GL_FLOAT,	// cada valor do v駻tice � do tipo GLfloat
			GL_FALSE,	// Quer normalizar os dados e converter tudo pra NDC ? ( no nosso caso, j� esta tudo correto, ent縊 deixamos como FALSE)
			6 * sizeof(GLfloat),// De quantos em quantos bytes, este atributo � encontrado no buffer ? No nosso caso 3 floats pros vertices + 3 floats pra cor = 6 floats
			(GLvoid*) (3 * sizeof(GLfloat))	// Onde est� o primeiro valor deste atributo no buffer. Nesse caso, 3 floats ap o in兤io do buffer
		);

	glEnableVertexAttribArray(1);		// Habilita este atributo


	glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {
    glUseProgram(renderingProgram);

	// Use este VAO e suas configura鋏es
	glBindVertexArray(m_VAO);
	glPointSize(10.0f);
    glDrawArrays(GL_TRIANGLES, 0, 9);
    glDrawArrays(GL_TRIANGLE_STRIP, 9, 4);

	glBindVertexArray(0);

}

int main(void) {
    if (!glfwInit()) {
    	exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(500, 500, "Leon Chavez, Jesus Alberto", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
    	exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
