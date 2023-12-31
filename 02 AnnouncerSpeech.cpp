#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <gl/glu.h>
#include <GL/gl.h> 
#include <cmath>
#include "stb_image.h"

#define PI 3.141592653
#define SCREEN_WIDTH	1600
#define SCREEN_HEIGHT	900
#define FPS	30


float cameraX = 1.9;
float cameraY = 0.8;
float cameraZ = 2.1;
float targetX = 0.0;
float targetY = 0.8;
float targetZ = 0.0;
float cameraSpeed = 0.02;
float scale = 2;
const float MICROPHONE_HEIGHT = 0.2f;
const float MICROPHONE_RADIUS = 0.4f;

float rightHandAngleZ = 0.0;
float micHeight = 10.0;
float micPlacement = 1.5;
float rightHandAngleY = 0.0;

int timer1 = 0;
int timer2 = 0;

bool waiting1 = false;
bool rightHandUp = false;

GLuint texture1; // Texture ID

void display();
void initGL();
void idle();
void timer(int value);
void loadTexture(const char* fileName);

void floor();
void cube();
void drawLine3D(float startX, float startY, float startZ, float endX, float endY, float endZ);
void drawCylinder(float radius, float height);
void groupCylinder();
void groupLines();
void camera();
void drawSphere(float radius, int slices, int stacks);
void texture();
void reshape(int width, int height);
void stage();
void announcer();
void mic();
void humanModel();
void drawMicrophone();

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Cube Example");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glEnable(GL_DEPTH_TEST);
	loadTexture("C:/Users/HP/Desktop/cg images/tt.jpg");
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture1);

	camera();
	texture();

	floor();
	stage();

	announcer();
	mic();


	glFlush();
	glutSwapBuffers();
}

void initGL()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPointSize(10.0);
	glLineWidth(5.0);

	// lights
	glEnable(GL_COLOR_MATERIAL); // cheat!!!!
	glEnable(GL_LIGHTING);
	GLfloat dir[] = { 0.2, 0.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, dir);
	glEnable(GL_LIGHT0);

	// depth buffer
	glEnable(GL_DEPTH_TEST);
}

void idle()
{
	DWORD start = GetTickCount64();
	while (GetTickCount64() - start < 1000 / FPS);

	if (timer1 >= 0)
	{
		timer1 += 5;
	}
	if (timer1 >= 100 && timer1 < 250)
	{
		rightHandAngleZ -= 5.0;
	}
	else if (timer1 >= 250 && timer1 < 370)
	{
		micHeight -= 0.2f;
	}
	else if (timer1 >= 370 && timer1 < 400)
	{
		micHeight -= 0.15f;
		rightHandAngleZ += 5.0;
	}
	else if (timer1 >= 400 && timer1 < 420)
	{
		micPlacement += 0.17;
		rightHandAngleY += 5.0;
	}
	else
	{
		timer1 = timer1;
	}


	glutPostRedisplay();
}

void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);
}

void loadTexture(const char* fileName)
{
	int width, height, numChannels;
	unsigned char* imageData = stbi_load(fileName, &width, &height, &numChannels, 0);
	if (!imageData)
	{
		printf("Failed to load texture: %s\n", fileName);
		return;
	}

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Specify the texture image data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

void floor()
{
	glBegin(GL_QUADS);
	glColor3ub(64, 64, 64);
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for the first vertex
	glVertex3f(-3.0 * scale, -0.45 * scale, 3.0 * scale);
	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for the second vertex
	glVertex3f(3.0 * scale, -0.45 * scale, 3.0 * scale);
	glTexCoord2f(1.0f, 1.0f); // Texture coordinate for the third vertex
	glVertex3f(3.0 * scale, -0.45 * scale, -3.0 * scale);
	glTexCoord2f(0.0f, 1.0f); // Texture coordinate for the fourth vertex
	glVertex3f(-3.0 * scale, -0.45 * scale, -3.0 * scale);
	glEnd();
}

void cube()
{
	// Draw the cuboid
	glBegin(GL_QUADS);
	// Front face
	glColor3f(1.0, 0.0, 0.0); // Red color
	glVertex3f(-1.0 * scale, -0.2 * scale, 1.0 * scale); // Adjusted y-coordinate
	glVertex3f(1.0 * scale, -0.2 * scale, 1.0 * scale); // Adjusted y-coordinate
	glVertex3f(1.0 * scale, 0.2 * scale, 1.0 * scale); // Adjusted y-coordinate
	glVertex3f(-1.0 * scale, 0.2 * scale, 1.0 * scale); // Adjusted y-coordinate
	glColor3f(1.0, 0.0, 0.0); // Red color
	glVertex3f(-1.0 * scale, -0.2 * scale, -1.0 * scale); // Adjusted y-coordinate
	glVertex3f(1.0 * scale, -0.2 * scale, -1.0 * scale); // Adjusted y-coordinate
	glVertex3f(1.0 * scale, 0.2 * scale, -1.0 * scale); // Adjusted y-coordinate
	glVertex3f(-1.0 * scale, 0.2 * scale, -1.0 * scale); // Adjusted y-coordinate
	// Right face
	glColor3f(0.0, 0.0, 1.0); // Blue color
	glVertex3f(1.0 * scale, -0.2 * scale, -1.0 * scale); // Adjusted y-coordinate
	glVertex3f(1.0 * scale, -0.2 * scale, 1.0 * scale); // Adjusted y-coordinate
	glVertex3f(1.0 * scale, 0.2 * scale, 1.0 * scale); // Adjusted y-coordinate
	glVertex3f(1.0 * scale, 0.2 * scale, -1.0 * scale); // Adjusted y-coordinate
	glColor3f(0.0, 0.0, 1.0); // Blue color
	glVertex3f(-1.0 * scale, -0.2 * scale, -1.0 * scale); // Adjusted y-coordinate
	glVertex3f(-1.0 * scale, -0.2 * scale, 1.0 * scale); // Adjusted y-coordinate
	glVertex3f(-1.0 * scale, 0.2 * scale, 1.0 * scale); // Adjusted y-coordinate
	glVertex3f(-1.0 * scale, 0.2 * scale, -1.0 * scale); // Adjusted y-coordinate
	// Top face
	glColor3ub(192, 192, 192); // Grey color
	glVertex3f(-1.0 * scale, 0.2 * scale, 1.0 * scale); // Adjusted y-coordinate
	glVertex3f(1.0 * scale, 0.2 * scale, 1.0 * scale); // Adjusted y-coordinate
	glVertex3f(1.0 * scale, 0.2 * scale, -1.0 * scale); // Adjusted y-coordinate
	glVertex3f(-1.0 * scale, 0.2 * scale, -1.0 * scale); // Adjusted y-coordinate
	// Bottom face
	glColor3f(1.0, 0.0, 1.0); // Magenta color
	glVertex3f(-1.0 * scale, -0.2 * scale, 1.0 * scale); // Adjusted y-coordinate
	glVertex3f(1.0 * scale, -0.2 * scale, 1.0 * scale); // Adjusted y-coordinate
	glVertex3f(1.0 * scale, -0.2 * scale, -1.0 * scale); // Adjusted y-coordinate
	glVertex3f(-1.0 * scale, -0.2 * scale, -1.0 * scale); // Adjusted y-coordinate
	glEnd();
}

void drawLine3D(float startX, float startY, float startZ, float endX, float endY, float endZ)
{
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(startX, startY, startZ);
	glVertex3f(endX, endY, endZ);
	glEnd();
}

void drawCylinder(float radius, float height)
{
	const int numSegments = 30;
	const float segmentAngle = 2.0 * PI / numSegments;

	glBegin(GL_QUAD_STRIP);
	glColor3f(0.5, 0.5, 0.5);
	for (int i = 0; i <= numSegments; ++i)
	{
		float angle = segmentAngle * i;
		float x = radius * cos(angle);
		float z = radius * sin(angle);

		glVertex3f(x, height / 2.0, z);
		glVertex3f(x, -height / 2.0, z);
	}
	glEnd();
}

void groupCylinder()
{
	glPushMatrix();
	glTranslatef(1.0 * scale, 1.0, 1.0 * scale);
	drawCylinder(0.03, 1.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0 * scale, 1.0, -1.0 * scale);
	drawCylinder(0.03, 1.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0 * scale, 1.0, -1.0 * scale);
	drawCylinder(0.03, 1.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0 * scale, 1.0, 1.0 * scale);
	drawCylinder(0.03, 1.2);
	glPopMatrix();
}

void groupLines()
{
	drawLine3D(1.0 * scale, 1.0, 1.0 * scale, 1.0 * scale, 1.0, -1.0 * scale);
	drawLine3D(1.0 * scale, 1.0, 1.0 * scale, -1.0 * scale, 1.0, 1.0 * scale);
	drawLine3D(-1.0 * scale, 1.0, 1.0 * scale, -1.0 * scale, 1.0, -1.0 * scale);
	drawLine3D(1.0 * scale, 1.0, -1.0 * scale, -1.0 * scale, 1.0, -1.0 * scale);

	drawLine3D(1.0 * scale, 0.8, 1.0 * scale, 1.0 * scale, 0.8, -1.0 * scale);
	drawLine3D(1.0 * scale, 0.8, 1.0 * scale, -1.0 * scale, 0.8, 1.0 * scale);
	drawLine3D(-1.0 * scale, 0.8, 1.0 * scale, -1.0 * scale, 0.8, -1.0 * scale);
	drawLine3D(1.0 * scale, 0.8, -1.0 * scale, -1.0 * scale, 0.8, -1.0 * scale);

	drawLine3D(1.0 * scale, 1.2, 1.0 * scale, 1.0 * scale, 1.2, -1.0 * scale);
	drawLine3D(1.0 * scale, 1.2, 1.0 * scale, -1.0 * scale, 1.2, 1.0 * scale);
	drawLine3D(-1.0 * scale, 1.2, 1.0 * scale, -1.0 * scale, 1.2, -1.0 * scale);
	drawLine3D(1.0 * scale, 1.2, -1.0 * scale, -1.0 * scale, 1.2, -1.0 * scale);
}

void camera()
{
	gluLookAt(cameraX, cameraY, cameraZ,  // camera position
		targetX, targetY, targetZ,        // target position
		0.0, 1.0, 0.0);                   // up vector
}

void drawSphere(float radius, int slices, int stacks)
{
	float theta, phi;
	float thetaStep = 2.0f * PI / slices;
	float phiStep = PI / stacks;

	for (int i = 0; i < stacks; ++i)
	{
		phi = -PI / 2.0f + (float)i * phiStep; // Start from the bottom and go upwards

		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= slices; ++j)
		{
			theta = j * thetaStep;

			// Calculate the vertex positions
			float x = radius * std::cos(phi) * std::cos(theta);
			float y = radius * std::sin(phi);
			float z = radius * std::cos(phi) * std::sin(theta);

			glNormal3f(x, y, z);
			glTexCoord2f(static_cast<float>(j) / slices, static_cast<float>(i) / stacks); // Set texture coordinates
			glVertex3f(x, y, z);

			// Calculate the vertex positions for the next stack
			x = radius * std::cos(phi + phiStep) * std::cos(theta);
			y = radius * std::sin(phi + phiStep);
			z = radius * std::cos(phi + phiStep) * std::sin(theta);

			glNormal3f(x, y, z);
			glTexCoord2f(static_cast<float>(j) / slices, static_cast<float>(i + 1) / stacks); // Set texture coordinates
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}

void texture()
{
	glColor3f(0.5, 0.5, 0.5);
	drawSphere(15, 10, 10);
	glFlush();
	glDisable(GL_TEXTURE_2D);
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)width / (double)height, 1.0, 100.0);
}

void stage()
{
	glPushMatrix();
	glTranslatef(0.0, -0.5, 0.0);
	cube();
	groupCylinder();
	groupLines();
	glPopMatrix();
}

void announcer()
{
	glPushMatrix();
	glScaled(0.2, 0.2, 0.2);
	glTranslated(0.0, 3.0, 0.0);
	glRotated(40.0, 0.0, 1.0, 0.0);
	glRotated(10.0, 1.0, 0.0, 0.0);
	humanModel();
	glPopMatrix();
}

void mic()
{
	glPushMatrix();
	glColor3ub(0, 0, 0);
	glScaled(0.2, 0.2, 0.2);
	glTranslated(micPlacement, micHeight, 3.0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	drawMicrophone();
	glPopMatrix();
}

void humanModel()
{
	//head
	glPushMatrix();
	glColor3f(0.95f, 0.75f, 0.6f);
	glScaled(0.5, 0.5, 0.5);		//glScaled(0.5, 0.5, 0.5);
	glTranslated(0.0, 3.7, 0.0);		//glTranslated(0.0, 9.0, 0.0);
	glutSolidCube(3.0);			//glutSolidSphere(5.0, 25.0, 25.0);
	glPopMatrix();
	//body
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glScaled(0.8, 1.0, 0.5);
	glTranslated(0.0, -0.5, 0.0);
	glutSolidCube(3.0);
	glPopMatrix();
	//left hand
	glPushMatrix();
	glColor3ub(32, 32, 32);
	glScaled(0.5, 1.0, 0.3); // scaling size
	glTranslated(3.0, -0.5, 0.0); //positioning
	glutSolidCube(2.0); //size
	glPopMatrix();
	//left fist
	glPushMatrix();
	glColor3f(0.95f, 0.75f, 0.6f);
	glScaled(0.1, 0.1, 0.1);
	glTranslated(16.5, -18, 0.0);
	glutSolidSphere(3.0, 25.0, 25.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.0, 0.3, 0.0);
	glRotatef(rightHandAngleY, 0.0, 1.0, 0.0);
	glTranslatef(-3.0, -0.3, 0.0);
	glPushMatrix();
	glTranslatef(3.0, 0.3, 0.0);
	glRotatef(rightHandAngleZ, 1.0, 0.0, 0.0);
	glTranslatef(-3.0, -0.3, 0.0);

	//right hand
	glPushMatrix();
	glColor3ub(32, 32, 32);
	glScaled(0.4, 1.0, 0.3);
	glTranslated(-3.9, -0.5, 0.0);
	glutSolidCube(2.0);
	glPopMatrix();

	//right fist
	glPushMatrix();
	glColor3f(0.95f, 0.75f, 0.6f);
	glScaled(0.1, 0.1, 0.1);
	glTranslated(-16.0, -18, 0.0);
	glutSolidSphere(3.0, 25.0, 25.0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//left leg
	glPushMatrix();
	glColor3ub(32, 32, 32);
	glScaled(0.4, 1.2, 0.3);
	glTranslated(-1.2, -2.5, 0.0);
	glutSolidCube(2.0);
	glPopMatrix();
	//right leg
	glPushMatrix();
	glColor3ub(32, 32, 32);
	glScaled(0.4, 1.2, 0.3);
	glTranslated(1.2, -2.5, 0.0);
	glutSolidCube(2.0);
	glPopMatrix();
}

void drawMicrophone() {
	drawLine3D(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -20.0f);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, MICROPHONE_HEIGHT * -4.0);
	glColor3ub(65, 65, 65);
	GLUquadricObj* base3 = gluNewQuadric();
	gluCylinder(base3, MICROPHONE_RADIUS * 0.15, MICROPHONE_RADIUS * 0.15, MICROPHONE_HEIGHT, 16, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, MICROPHONE_HEIGHT * -3.0);
	glColor3ub(105, 105, 105);
	GLUquadricObj* base2 = gluNewQuadric();
	gluCylinder(base2, MICROPHONE_RADIUS * 0.35, MICROPHONE_RADIUS * 0.35, MICROPHONE_HEIGHT, 16, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, MICROPHONE_HEIGHT * -2.5);
	glColor3ub(51, 51, 51);
	GLUquadricObj* base1 = gluNewQuadric();
	gluCylinder(base1, MICROPHONE_RADIUS * 0.5, MICROPHONE_RADIUS * 0.5, MICROPHONE_HEIGHT, 16, 1);
	glPopMatrix();

	// Draw the microphone base (a cylinder)
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, MICROPHONE_HEIGHT * -2.0);
	glColor3ub(105, 105, 105);
	GLUquadricObj* base = gluNewQuadric();
	gluCylinder(base, MICROPHONE_RADIUS * 0.8, MICROPHONE_RADIUS * 0.8, MICROPHONE_HEIGHT * 2.0, 16, 1);
	glPopMatrix();

	// Draw the microphone body (a cone)
	glTranslatef(0.0f, 0.0f, MICROPHONE_HEIGHT * 0.2);
	glColor3f(0.8f, 0.8f, 0.8f);
	GLUquadricObj* body = gluNewQuadric();
	gluCylinder(body, MICROPHONE_RADIUS * 0.7, 0.0, MICROPHONE_HEIGHT * 0.8, 16, 1);

	// Draw the microphone top (a sphere)
	glTranslatef(0.0f, 0.0f, MICROPHONE_HEIGHT * 0.8);
	glColor3f(0.7f, 0.7f, 0.7f);
	glutSolidSphere(MICROPHONE_RADIUS * 0.9, 16, 16);
}
