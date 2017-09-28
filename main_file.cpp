
#define GLM_FORCE_RADIANS
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <lodepng.h>
#include <cmath>
#include "constants.h"
#include "skeleton.h"
#include "skeletonModel.h"
#include "colors.h"
#include "floor.h"
#include "myCube.h"

using namespace glm;

float aspect=1.0f; //Aktualny stosunek szerokości do wysokości okna
float speed_x=0; //Szybkość kątowa obrotu obiektu w radianach na sekundę wokół osi x
float speed_y=PI/4; //Szybkość kątowa obrotu obiektu w radianach na sekundę wokół osi y
float firstDanceSpeed=0; //Szybkość kątowa obrotu obiektu w radianach na sekundę wokół osi y
float secondDanceSpeed=0; //Szybkość kątowa obrotu obiektu w radianach na sekundę wokół osi y
float torsoProgress=0; //Szybkość kątowa obrotu obiektu w radianach na sekundę wokół osi y
float headProgress=0; //Szybkość kątowa obrotu obiektu w radianach na sekundę wokół osi y
float arm_r_uProgress=0; //Szybkość kątowa obrotu obiektu w radianach na sekundę wokół osi y
float arm_r_bProgress=0; //Szybkość kątowa obrotu obiektu w radianach na sekundę wokół osi y
float hand_rProgress=0; //Szybkość kątowa obrotu obiektu w radianach na sekundę wokół osi y
float leg_r_uProgress=0; //Szybkość kątowa obrotu obiektu w radianach na sekundę wokół osi y
float leg_r_bProgress=0; //Szybkość kątowa obrotu obiektu w radianach na sekundę wokół osi y
float foot_rProgress=0; //Szybkość kątowa obrotu obiektu w radianach na sekundę wokół osi y
bool tempbool=false;

vec3 torsoVecDirection = vec3(0.0f,0.0f,0.0f);
vec3 headVecDirection = vec3(0.0f,0.0f,0.0f);
vec3 arm_r_uVecDirection = vec3(0.0f,0.0f,0.0f);
vec3 arm_r_bVecDirection = vec3(0.0f,0.0f,0.0f);
vec3 hand_rVecDirection = vec3(0.0f,0.0f,0.0f);
vec3 leg_r_uVecDirection = vec3(0.0f,0.0f,0.0f);
vec3 leg_r_bVecDirection = vec3(0.0f,0.0f,0.0f);
vec3 foot_rVecDirection = vec3(0.0f,0.0f,0.0f);

float arm_l_uProgress=0;
float arm_l_bProgress=0;
float hand_lProgress=0;
float leg_l_uProgress=0;
float leg_l_bProgress=0;
float foot_lProgress=0;

vec3 arm_l_uVecDirection = vec3(0.0f,0.0f,0.0f);
vec3 arm_l_bVecDirection = vec3(0.0f,0.0f,0.0f);
vec3 hand_lVecDirection = vec3(0.0f,0.0f,0.0f);
vec3 leg_l_uVecDirection = vec3(0.0f,0.0f,0.0f);
vec3 leg_l_bVecDirection = vec3(0.0f,0.0f,0.0f);
vec3 foot_lVecDirection = vec3(0.0f,0.0f,0.0f);

// GLuint tex; //uchwyt do obrazka tekstury
// GLuint tex2; //uchwyt do obrazka tekstury
GLuint *tex = new GLuint[2];

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


//Procedura obługi zmiany rozmiaru bufora ramki
void windowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); //Obraz ma być generowany w oknie o tej rozdzielczości
	aspect=(float)width/(float)height; //Stosunek szerokości do wysokości okna
}

//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed_y=0;
		if (key == GLFW_KEY_RIGHT) speed_y=PI/4;
		if (key == GLFW_KEY_UP) speed_x=PI/2;
		if (key == GLFW_KEY_DOWN) speed_x=-PI/2;
		if (key == GLFW_KEY_1) firstDanceSpeed=PI/2;
		if (key == GLFW_KEY_2) secondDanceSpeed=PI/2;
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) speed_y=0;
		if (key == GLFW_KEY_RIGHT) speed_y=0;
		if (key == GLFW_KEY_UP) speed_x=0;
		if (key == GLFW_KEY_DOWN) speed_x=0;
	}
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glfwSetFramebufferSizeCallback(window, windowResize); //Zarejestruj procedurę obsługi zmiany rozdzielczości bufora ramki
	glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury

	glClearColor(0,0,0,1); //Ustaw kolor czyszczenia ekranu

	glEnable(GL_LIGHTING); //Włącz tryb cieniowania
	glEnable(GL_LIGHT0); //Włącz zerowe źródło światła
	glEnable(GL_DEPTH_TEST); //Włącz używanie budora głębokości
	glEnable(GL_COLOR_MATERIAL); //Włącz śledzenie kolorów przez materiał

	glShadeModel(GL_SMOOTH);


	//Wczytanie i import obrazka – w initOpenGLProgram
	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image, image2; //Alokuj wektor do wczytania obrazka
	unsigned width, height, width2, height2;  //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	//Import do pamięci karty graficznej
	glGenTextures(2, tex);
	// glGenTextures(1,&tex); //Zainicjuj jeden uchwyt
	// glGenTextures(1,&tex2); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex[0]); //Uaktywnij uchwyt
	unsigned error = lodepng::decode(image, width, height, "metal2.png");
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, tex[1]); //Uaktywnij uchwyt
		unsigned error2 = lodepng::decode(image, width, height, "speaker.png");
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
	}

	//Procedura rysująca zawartość sceny
	void drawScene(GLFWwindow* window) {
		//************Tutaj umieszczaj kod rysujący obraz******************l
		mat4 M,R,T,S,I;

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Wyczyść bufor kolorów (czyli przygotuj "płótno" do rysowania)

		//***Przygotowanie do rysowania****
		mat4 P=perspective(50.0f*PI/180.0f,aspect,5.0f,50.0f); //Wylicz macierz rzutowania P
		mat4 V=lookAt( //Wylicz macierz widoku
			vec3(0.0f,0.0f,25.0f),
			vec3(0.0f,0.0f,0.0f),
			vec3(0.0f,1.0f,0.0f));
			glMatrixMode(GL_PROJECTION); //Włącz tryb modyfikacji macierzy rzutowania
			glLoadMatrixf(value_ptr(P)); //Załaduj macierz rzutowania
			glMatrixMode(GL_MODELVIEW);  //Włącz tryb modyfikacji macierzy model-widok


			//Rysowanie kostki
			//1. Wyliczenie i załadowanie macierzy modelu


			//2. Rysowanie modelu
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_NORMALIZE);
			glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania używaj tablicy wierzchołków
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState( GL_NORMAL_ARRAY );

			float lightPos[]={-5,5,0,1};
			float lightPos2[]={5,5,0,1};
			float direction[]={0,0,-1,0};
			// float colorsA[]={0.2f,0.2f,0.2f, 1};
			float colorsD[]={0.8f,0.8f,1, 1};
			glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
			glLightfv(GL_LIGHT0,GL_POSITION,lightPos2);
			glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,direction);

			glLightfv(GL_LIGHT0,GL_DIFFUSE,colorsD);

			glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,100);
			glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,128);
			glBindTexture(GL_TEXTURE_2D, tex[0]); //Uaktywnij uchwyt

			// Render podłogi
				mat4 floorMatrix=mat4(1.0f);
				floorMatrix=rotate(floorMatrix,(PI/2),vec3(0.0f,-1.0f,0.0f));
				floorMatrix=translate(floorMatrix,vec3(6.0f,-8.0f,0.0f));
				glLoadMatrixf(value_ptr(V*floorMatrix));
			glVertexPointer(3,GL_FLOAT,0, floorPositions);
			glNormalPointer( GL_FLOAT, 0, floorNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, floorPositions);
			glDrawArrays(GL_QUADS,0,floorVertices); //Rysuj model

			glBindTexture(GL_TEXTURE_2D, tex[1]); //Uaktywnij uchwyt

			// Render Głośników
			mat4 speakerMatrix=floorMatrix;
			// floorMatrix=rotate(floorMatrix,(PI/2),vec3(0.0f,-1.0f,0.0f));
			speakerMatrix=translate(speakerMatrix,vec3(-15.0f,5.0f,-10.0f));
			glLoadMatrixf(value_ptr(V*speakerMatrix));
		glVertexPointer(3,GL_FLOAT,0, myCubeVertices);
		glNormalPointer( GL_FLOAT, 0, myCubeVertices);
		glTexCoordPointer( 3, GL_FLOAT, 0, myCubeVertices);
		glDrawArrays(GL_QUADS,0,myCubeVertexCount); //Rysuj model

			mat4 speaker2Matrix=floorMatrix;
			// floorMatrix=rotate(floorMatrix,(PI/2),vec3(0.0f,-1.0f,0.0f));
			speaker2Matrix=translate(speaker2Matrix,vec3(-15.0f,5.0f,10.0f));
			glLoadMatrixf(value_ptr(V*speaker2Matrix));
		glVertexPointer(3,GL_FLOAT,0, myCubeVertices);
		glNormalPointer( GL_FLOAT, 0, myCubeVertices);
		glTexCoordPointer( 3, GL_FLOAT, 0, myCubeVertices);
		glDrawArrays(GL_QUADS,0,myCubeVertexCount); //Rysuj model

		glBindTexture(GL_TEXTURE_2D, tex[0]); //Uaktywnij uchwyt

			// Render torsu
			mat4 torsoMatrix=mat4(1.0f);
			torsoMatrix=translate(torsoMatrix,vec3(0.0f,4.0f,0.0f));
			if (torsoProgress) {
				torsoMatrix=rotate(torsoMatrix,torsoProgress,torsoVecDirection);
			}
			glLoadMatrixf(value_ptr(V*torsoMatrix));
			glVertexPointer(4,GL_FLOAT,0, torsoPositions);
			glNormalPointer( GL_FLOAT, 0, torsoNormals);
			glTexCoordPointer( 3, GL_FLOAT, 0, torsoTexels);
			glDrawArrays(GL_QUADS,0,torsoVertices); //Rysuj model

			// Render głowy
			mat4 headMatrix=torsoMatrix;
			headMatrix=translate(headMatrix,vec3(0.0f,2.0f,0.0f));
				if (headProgress) {
					headMatrix=rotate(headMatrix,headProgress,headVecDirection);
				}
			glLoadMatrixf(value_ptr(V*headMatrix));
			glVertexPointer(4,GL_FLOAT,0,headPositions);
			glNormalPointer( GL_FLOAT, 0, headNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, headTexels);
			glDrawArrays(GL_QUADS,0,headVertices); //Rysuj model

			// Render prawego ramienia
			mat4 arm_r_uMatrix=torsoMatrix;
			arm_r_uMatrix=translate(arm_r_uMatrix,vec3(-1.2f,0.5f,0.0f));
			if (arm_r_uProgress) {
				arm_r_uMatrix=rotate(arm_r_uMatrix, arm_r_uProgress, arm_r_uVecDirection);
			}
			glLoadMatrixf(value_ptr(V*arm_r_uMatrix));
			glVertexPointer(4,GL_FLOAT,0,arm_r_uPositions);
			glNormalPointer( GL_FLOAT, 0, arm_r_uNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, arm_r_uTexels);
			glDrawArrays(GL_QUADS,0,arm_r_uVertices); //Rysuj model

			// Render prawego przedramienia
			mat4 arm_r_bMatrix = arm_r_uMatrix;
			arm_r_bMatrix=translate(arm_r_bMatrix,vec3(-3.0f,0.0f,0.0f));
			if (arm_r_bProgress) {
				arm_r_bMatrix=rotate(arm_r_bMatrix, arm_r_bProgress, arm_r_bVecDirection);
			}
			glLoadMatrixf(value_ptr(V*arm_r_bMatrix));
			glVertexPointer(4,GL_FLOAT,0,arm_r_bPositions);
			glNormalPointer( GL_FLOAT, 0, arm_r_bNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, arm_r_bTexels);
			glDrawArrays(GL_QUADS,0,arm_r_bVertices); //Rysuj model

			// Render prawej dłoni
			mat4 hand_rMatrix = arm_r_bMatrix;
			hand_rMatrix=translate(hand_rMatrix,vec3(-2.3f,0.1f,0.0f));
			if (hand_rProgress) {
				hand_rMatrix=rotate(hand_rMatrix, hand_rProgress, hand_rVecDirection);
			}
			glLoadMatrixf(value_ptr(V*hand_rMatrix));
			glVertexPointer(4,GL_FLOAT,0,hand_rPositions);
			glNormalPointer( GL_FLOAT, 0, arm_r_bNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, arm_r_bTexels);
			glDrawArrays(GL_QUADS,0,hand_rVertices); //Rysuj model

			// Render lewego ramienia
			mat4 arm_l_uMatrix=torsoMatrix;
			arm_l_uMatrix=translate(arm_l_uMatrix,vec3(1.2f,0.5f,0.0f));
			if (arm_l_uProgress) {
				 arm_l_uMatrix=rotate( arm_l_uMatrix,  arm_l_uProgress,  arm_l_uVecDirection);
			}
			glLoadMatrixf(value_ptr(V*arm_l_uMatrix));
			glVertexPointer(4,GL_FLOAT,0,arm_l_uPositions);
			glNormalPointer( GL_FLOAT, 0, arm_l_uNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, arm_l_uTexels);
			glDrawArrays(GL_QUADS,0,arm_l_uVertices); //Rysuj model

			// Render lewego przedramienia
			mat4 arm_l_bMatrix = arm_l_uMatrix;
			arm_l_bMatrix=translate(arm_l_bMatrix,vec3(3.0f,0.2f,0.0f));
			// arm_l_bMatrix=rotate(arm_l_bMatrix,angle_y,vec3(0.0f,0.0f,1.0f));
			if (arm_l_bProgress) {
				 arm_l_bMatrix=rotate( arm_l_bMatrix,  arm_l_bProgress,  arm_l_bVecDirection);
			}
			glLoadMatrixf(value_ptr(V*arm_l_bMatrix));
			glVertexPointer(4,GL_FLOAT,0,arm_l_bPositions);
			glNormalPointer( GL_FLOAT, 0, arm_l_bNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, arm_l_bTexels);
			glDrawArrays(GL_QUADS,0,arm_l_bVertices); //Rysuj model

			// Render lewej dłoni
			mat4 hand_lMatrix = arm_l_bMatrix;
			hand_lMatrix=translate(hand_lMatrix,vec3(2.3f,0.1f,0.0f));
			if (hand_lProgress) {
				 hand_lMatrix=rotate(hand_lMatrix, hand_lProgress, hand_lVecDirection);
			}
			glLoadMatrixf(value_ptr(V*hand_lMatrix));
			glVertexPointer(4,GL_FLOAT,0,hand_lPositions);
			glNormalPointer( GL_FLOAT, 0, hand_lNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, hand_lTexels);
			glDrawArrays(GL_QUADS,0,hand_lVertices); //Rysuj model

			// Render prawego uda
			mat4 leg_r_uMatrix=torsoMatrix;
			leg_r_uMatrix=translate(leg_r_uMatrix,vec3(-0.8f,-3.8f,0.0f));
			if (leg_r_uProgress) {
				 leg_r_uMatrix=rotate( leg_r_uMatrix,  leg_r_uProgress,  leg_r_uVecDirection);
			}
			glLoadMatrixf(value_ptr(V*leg_r_uMatrix));
			glVertexPointer(4,GL_FLOAT,0,leg_r_uPositions);
			glNormalPointer( GL_FLOAT, 0, leg_r_uNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, leg_r_uTexels);
			glDrawArrays(GL_QUADS,0,leg_r_uVertices); //Rysuj model

			// Render prawego piszczela
			mat4 leg_r_bMatrix = leg_r_uMatrix;
			leg_r_bMatrix=translate(leg_r_bMatrix,vec3(0.3f,-4.0f,0.0f));
			if ( leg_r_bProgress) {
				  leg_r_bMatrix=rotate(leg_r_bMatrix, leg_r_bProgress, leg_r_bVecDirection);
			}
			glLoadMatrixf(value_ptr(V*leg_r_bMatrix));
			glVertexPointer(4,GL_FLOAT,0,leg_r_bPositions);
			glNormalPointer( GL_FLOAT, 0, leg_r_bNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, leg_r_bTexels);
			glDrawArrays(GL_QUADS,0,leg_r_bVertices); //Rysuj model

			// Render prawej stopy
			mat4 foot_rMatrix = leg_r_bMatrix;
			foot_rMatrix=translate(foot_rMatrix,vec3(-0.1f,-4.0f,0.7f));
			if ( foot_rProgress) {
					foot_rMatrix=rotate(foot_rMatrix, foot_rProgress, foot_rVecDirection);
			}
			glLoadMatrixf(value_ptr(V*foot_rMatrix));
			glVertexPointer(4,GL_FLOAT,0,foot_rPositions);
			glNormalPointer( GL_FLOAT, 0, foot_rNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, foot_rTexels);
			glDrawArrays(GL_QUADS,0,foot_rVertices); //Rysuj model

			// Render lewego uda
			mat4 leg_l_uMatrix=torsoMatrix;
			leg_l_uMatrix=translate(leg_l_uMatrix,vec3(0.8f,-3.8f,0.0f));
			if ( leg_l_uProgress) {
					leg_l_uMatrix=rotate(leg_l_uMatrix, leg_l_uProgress, leg_l_uVecDirection);
			}
			glLoadMatrixf(value_ptr(V*leg_l_uMatrix));
			glVertexPointer(4,GL_FLOAT,0,leg_l_uPositions);
			glNormalPointer( GL_FLOAT, 0, leg_l_uNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, leg_l_uTexels);
			glDrawArrays(GL_QUADS,0,leg_l_uVertices); //Rysuj model

			// Render lewego piszczela
			mat4 leg_l_bMatrix = leg_l_uMatrix;
			leg_l_bMatrix=translate(leg_l_bMatrix,vec3(0.0f,-4.0f,0.0f));
			if ( leg_l_bProgress) {
					leg_l_bMatrix=rotate(leg_l_bMatrix, leg_l_bProgress, leg_l_bVecDirection);
			}
			glLoadMatrixf(value_ptr(V*leg_l_bMatrix));
			glVertexPointer(4,GL_FLOAT,0,leg_l_bPositions);
			glNormalPointer( GL_FLOAT, 0, leg_l_bNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, leg_l_bTexels);
			glDrawArrays(GL_QUADS,0,leg_l_bVertices); //Rysuj model

			// Render lewej stopy
			mat4 foot_lMatrix = leg_l_bMatrix;
			foot_lMatrix=translate(foot_lMatrix,vec3(-0.1f,-4.0f,0.7f));
			if ( foot_lProgress) {
					foot_lMatrix=rotate(foot_lMatrix, foot_lProgress, foot_lVecDirection);
			}
			glLoadMatrixf(value_ptr(V*foot_lMatrix));

			glVertexPointer(4,GL_FLOAT,0,foot_rPositions);
			glNormalPointer( GL_FLOAT, 0, foot_rNormals);
			glTexCoordPointer( 2, GL_FLOAT, 0, foot_rTexels);
			glDrawArrays(GL_QUADS,0,foot_rVertices); //Rysuj model


			//Posprzątaj po sobie
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState( GL_NORMAL_ARRAY );
			// glDisableClientState(GL_COLOR_ARRAY);

			glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
		}

		reset() {
			torsoProgress = 0;
			headProgress = 0;
			hand_lProgress = 0;
			arm_l_bProgress = 0;
			arm_l_uProgress = 0;

			hand_rProgress = 0;
			arm_r_bProgress = 0;
			arm_r_uProgress = 0;

			leg_l_uProgress = 0;
			leg_r_uProgress = 0;
			leg_l_bProgress = 0;
			leg_r_bProgress = 0;
		}

		simpleDance(GLFWwindow* window) {
			reset();
			float danceProgress=0.0f; //Aktualny kąt obrotu obiektu wokół osi y
			glfwSetTime(0); //Wyzeruj timer
			// // Krok 1
			torsoVecDirection = vec3(0.0f,1.0f,0.0f);
			headVecDirection = vec3(0.0f,0.0f,1.0f);
			hand_lVecDirection = vec3(0.0f,0.0f,1.0f);
			arm_l_bVecDirection = vec3(1.0f,0.0f,1.0f);
			arm_l_uVecDirection = vec3(0.0f,0.0f,1.0f);
			hand_rVecDirection = vec3(0.0f,0.0f,1.0f);
			arm_r_bVecDirection = vec3(1.0f,0.0f,1.0f);
			arm_r_uVecDirection = vec3(0.0f,0.0f,1.0f);
			leg_l_uVecDirection = vec3(0.0f,-1.0f,0.0f);
			leg_r_uVecDirection = vec3(0.0f,1.0f,0.0f);
			leg_l_bVecDirection = vec3(-1.0f,0.0f,0.0f);
			leg_r_bVecDirection = vec3(1.0f,0.0f,0.0f);

			// start fali
			for(int i = 0; i<150; i++) {
				// angle_y+=speed_y*glfwGetTime();
				danceProgress += firstDanceSpeed*glfwGetTime();
				glfwSetTime(0); //Wyzeruj timer

				torsoProgress = danceProgress*4;
				headProgress = danceProgress;
				hand_lProgress = danceProgress;
				arm_l_bProgress = danceProgress*2;
				arm_l_uProgress = danceProgress;

				hand_rProgress = danceProgress;
				arm_r_bProgress = danceProgress*2;
				arm_r_uProgress = danceProgress;

				leg_l_uProgress = danceProgress;
				leg_r_uProgress = danceProgress;
				leg_l_bProgress = danceProgress*4;
				leg_r_bProgress = danceProgress*1.5;
				drawScene(window); //Wykonaj procedurę rysującą

				if (i % 25 == 0 ) {
					firstDanceSpeed*=-1;
				}
			}
			// Krok 2
			for(int i = 0; i<150; i++) {
				danceProgress += firstDanceSpeed*glfwGetTime();
				glfwSetTime(0); //Wyzeruj timer

				torsoProgress = -danceProgress*4;
				headProgress = -danceProgress;
				hand_lProgress = -danceProgress;
				arm_l_bProgress = -danceProgress*2;
				arm_l_uProgress = -danceProgress;

				hand_rProgress = -danceProgress;
				arm_r_bProgress = -danceProgress*2;
				arm_r_uProgress = -danceProgress;

				leg_l_uProgress = -danceProgress;
				leg_r_uProgress = -danceProgress;
				leg_l_bProgress = danceProgress*4;
				leg_r_bProgress = danceProgress*1.5;
				drawScene(window); //Wykonaj procedurę rysującą

				if (i % 25 == 0 ) {
					firstDanceSpeed*=-1;
				}
			}

			// Krok 3
			headVecDirection = vec3(0.0f,1.0f,0.0f);
			hand_lVecDirection = vec3(0.0f,0.0f,1.0f);
			arm_l_bVecDirection = vec3(0.0f,0.0f,1.0f);
			hand_rVecDirection = vec3(0.0f,0.0f,1.0f);
			arm_r_bVecDirection = vec3(0.0f,0.0f,1.0f);
			leg_l_uVecDirection = vec3(1.0f,0.0f,0.0f);
			leg_r_uVecDirection = vec3(1.0f,0.0f,0.0f);
			leg_l_bVecDirection = vec3(-2.0f,0.0f,0.0f);
			leg_r_bVecDirection = vec3(2.0f,0.0f,0.0f);

			for(int i = 0; i<200; i++) {
				// angle_y+=speed_y*glfwGetTime();
				danceProgress += firstDanceSpeed*glfwGetTime();
				glfwSetTime(0); //Wyzeruj timer

				torsoProgress = danceProgress;
				headProgress = danceProgress;
				hand_lProgress = danceProgress;
				arm_l_bProgress = danceProgress*2;
				arm_l_uProgress = danceProgress;

				hand_rProgress = danceProgress;
				arm_r_bProgress = danceProgress*2;
				arm_r_uProgress = danceProgress;

				leg_l_uProgress = danceProgress;
				leg_r_uProgress = danceProgress;
				leg_l_bProgress = danceProgress;
				leg_r_bProgress = danceProgress*2;
				drawScene(window); //Wykonaj procedurę rysującą

				if (i % 20 == 0 ) {
					firstDanceSpeed*=-1;
				}
			}

			// reset();

			// Krok 4 - reverse 3
			headVecDirection = vec3(0.0f,1.0f,0.0f);
			hand_lVecDirection = vec3(0.0f,0.0f,1.0f);
			arm_l_bVecDirection = vec3(0.0f,0.0f,1.0f);
			hand_rVecDirection = vec3(0.0f,0.0f,1.0f);
			arm_r_bVecDirection = vec3(1.0f,0.0f,1.0f);
			leg_l_uVecDirection = vec3(1.0f,0.0f,0.0f);
			leg_r_uVecDirection = vec3(1.0f,0.0f,0.0f);
			leg_l_bVecDirection = vec3(2.0f,0.0f,0.0f);
			leg_r_bVecDirection = vec3(-2.0f,0.0f,0.0f);

			for(int i = 0; i<200; i++) {
				// angle_y+=speed_y*glfwGetTime();
				danceProgress += firstDanceSpeed*glfwGetTime();
				glfwSetTime(0); //Wyzeruj timer

				torsoProgress = -danceProgress;
				headProgress = -danceProgress;
				hand_lProgress = -danceProgress;
				arm_l_bProgress = -danceProgress*2;
				arm_l_uProgress = -danceProgress;

				hand_rProgress = -danceProgress;
				arm_r_bProgress = -danceProgress*2;
				arm_r_uProgress = -danceProgress;

				leg_l_uProgress = -danceProgress;
				leg_r_uProgress = -danceProgress;
				leg_l_bProgress = danceProgress*2;
				leg_r_bProgress = -danceProgress;
				drawScene(window); //Wykonaj procedurę rysującą

				if (i % 20 == 0 ) {
					firstDanceSpeed*=-1;
				}
			}
			// Krok 5
			headVecDirection = vec3(0.0f,1.0f,0.0f);
			hand_lVecDirection = vec3(0.0f,0.0f,1.0f);
			arm_l_bVecDirection = vec3(0.0f,0.0f,1.0f);
			hand_rVecDirection = vec3(0.0f,0.0f,1.0f);
			arm_r_bVecDirection = vec3(0.0f,0.0f,1.0f);
			leg_l_uVecDirection = vec3(0.0f,0.0f,1.0f);
			leg_r_uVecDirection = vec3(0.0f,0.0f,1.0f);

			for(int i = 0; i<200; i++) {
				// angle_y+=speed_y*glfwGetTime();
				danceProgress += firstDanceSpeed*glfwGetTime();
				glfwSetTime(0); //Wyzeruj timer

				headProgress = danceProgress;
				hand_lProgress = danceProgress;
				arm_l_bProgress = danceProgress*4;

				hand_rProgress = danceProgress;
				arm_r_bProgress = danceProgress*4;

				if (danceProgress > 0) {
					leg_r_uProgress = -danceProgress*2;
				} else {
					leg_l_uProgress = -danceProgress*2;
				}
				drawScene(window); //Wykonaj procedurę rysującą

				if (i % 40 == 0 ) {
					firstDanceSpeed*=-1;
				}
			}
			firstDanceSpeed=0;
		}

		hardcoreDance(GLFWwindow* window) {
			reset();
			float danceProgress=0.0f; //Aktualny kąt obrotu obiektu wokół osi y
			glfwSetTime(0); //Wyzeruj timer
			// Krok 3
			headVecDirection = vec3(0.0f,1.0f,0.0f);
			hand_rVecDirection = vec3(0.0f,0.0f,1.0f);
			arm_r_bVecDirection = vec3(0.0f,0.0f,1.0f);
			//1
			torsoVecDirection = vec3(0.0f,1.0f,0.0f);

			arm_l_uVecDirection = vec3(0.0f,1.0f,0.0f);
			arm_r_uVecDirection = vec3(0.0f,1.0f,0.0f);
			arm_l_bVecDirection = vec3(0.0f,0.0f,1.0f);
			hand_lVecDirection = vec3(0.0f,0.0f,1.0f);
			leg_l_uVecDirection = vec3(1.0f,0.0f,0.0f);
			leg_r_uVecDirection = vec3(1.0f,0.0f,0.0f);
			leg_l_bVecDirection = vec3(-1.0f,0.0f,0.0f);
			leg_r_bVecDirection = vec3(-1.0f,0.0f,0.0f);
			for(int i = 0; i<240; i++) {
				danceProgress += secondDanceSpeed*glfwGetTime();
				glfwSetTime(0); //Wyzeruj timer

				// headProgress = danceProgress;
				// hand_lProgress = danceProgress;
				// arm_l_bProgress = danceProgress*2;
				// arm_l_uProgress = danceProgress;
				//
				// hand_rProgress = danceProgress;
				// arm_r_bProgress = danceProgress*2;
				// arm_r_uProgress = danceProgress;

				if (i % 80 == 0) {
					tempbool = !tempbool;
				}

				if (tempbool) {
					torsoProgress = danceProgress;
					arm_r_uProgress = -danceProgress*2;
					leg_r_uProgress = danceProgress*2;
					leg_r_bProgress = danceProgress*2;
				} else {
					torsoProgress = -danceProgress;
					arm_l_uProgress = danceProgress*2;
					leg_l_uProgress = danceProgress*2;
					leg_l_bProgress = danceProgress*2;
				}

				drawScene(window); //Wykonaj procedurę rysującą

				if (i % 40 == 0 ) {
					secondDanceSpeed*=-1;
				}
			}
			tempbool=true;
			// 2
			arm_l_uVecDirection = vec3(0.0f,1.0f,0.0f);
			arm_r_uVecDirection = vec3(0.0f,1.0f,0.0f);

			for(int i = 0; i<200; i++) {
				danceProgress += secondDanceSpeed*glfwGetTime();
				glfwSetTime(0); //Wyzeruj timer
				if (i == 20) {
					tempbool = !tempbool;
				}
				if (i == 120) {
					tempbool = !tempbool;
				}

				if (tempbool) {
					torsoProgress = danceProgress;
				} else {
					arm_r_uProgress = -danceProgress*2;
					arm_l_uProgress = danceProgress*2;
				}

				drawScene(window); //Wykonaj procedurę rysującą

				if (i % 40 == 0 ) {
					secondDanceSpeed*=-1;
				}
			}

			//3

			arm_l_bVecDirection = vec3(0.0f,0.0f,1.0f);
			arm_r_bVecDirection = vec3(0.0f,0.0f,1.0f);
			headVecDirection = vec3(1.0f,0.0f,0.0f);

			for(int i = 0; i<200; i++) {
				danceProgress += secondDanceSpeed*glfwGetTime();
				glfwSetTime(0); //Wyzeruj timer

				// headProgress = danceProgress;
				// hand_lProgress = danceProgress;
				// arm_l_bProgress = danceProgress*2;
				// arm_l_uProgress = danceProgress;
				//
				// hand_rProgress = danceProgress;
				// arm_r_bProgress = danceProgress*2;
				// arm_r_uProgress = danceProgress;

				if (i % 40 == 0) {
					tempbool = !tempbool;
				}
				torsoProgress = danceProgress;

				if (tempbool) {
					headProgress = -danceProgress;
					arm_l_bProgress = -danceProgress*2;
					arm_r_bProgress = danceProgress*2;
				} else {
					headProgress = -danceProgress;
					arm_l_bProgress = -danceProgress*2;
					arm_r_bProgress = danceProgress*2;
				}

				drawScene(window); //Wykonaj procedurę rysującą

				if (i % 40 == 0 ) {
					secondDanceSpeed*=-1;
				}
			}

			secondDanceSpeed=0;
			tempbool=false;
		}

		int main(void)
		{
			GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

			glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

			if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
				fprintf(stderr, "Nie można zainicjować GLFW.\n");
				exit(EXIT_FAILURE);
			}

			window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

			if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
			{
				glfwTerminate();
				exit(EXIT_FAILURE);
			}

			glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
			glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

			GLenum err;
			if ((err=glewInit()) != GLEW_OK) { //Zainicjuj bibliotekę GLEW
				fprintf(stderr, "Nie można zainicjować GLEW: %s\n", glewGetErrorString(err));
				exit(EXIT_FAILURE);
			}

			initOpenGLProgram(window); //Operacje inicjujące


			float angle_x=0.0f; //Aktualny kąt obrotu obiektu wokół osi x
			float angle_y=0.0f; //Aktualny kąt obrotu obiektu wokół osi y
			glfwSetTime(0); //Wyzeruj timer


			//Główna pętla
			while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
			{
				angle_x+=speed_x*glfwGetTime(); //Oblicz przyrost kąta obrotu i zwiększ aktualny kąt
				glfwSetTime(0); //Wyzeruj timer
				drawScene(window); // załadowanie sceny początkowej

				// wywołanie tanców
				if (firstDanceSpeed) { // taniec pierwszy po kliknięciu '1'
					simpleDance(window);
				}
				if (secondDanceSpeed) { // taniec drtugi po kliknięciu '2'
					hardcoreDance(window);
				}

				glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
			}
			glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
			glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
			exit(EXIT_SUCCESS);
		}
