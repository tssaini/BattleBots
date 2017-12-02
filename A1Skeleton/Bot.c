#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include "CubeMesh.h"
#include "QuadMesh.h"

#include "Matrix3D.h"
#include "Bot.h"

#define PI 3.14159265

void drawWheel();
void draw_circle(const GLfloat radius, const GLuint num_vertex);

Bot newBot() {
	Bot b;
	b.x = 0;
	b.y = 0;
	b.z = 0;
	b.aPitch = 0;
	b.aYaw = 0;
	b.hPitch = 0;
	b.botAngle = 0;
	b.speed = 0;
	b.wheelAngle = 0;

	return b;
}

void drawPlayerBot(Bot *b) {

	//bot 1
	glPushMatrix();
	CubeMesh c = newCube();

	//move bot
	glTranslatef(b->x, b->y, b->z);
	glRotatef(b->botAngle, 0.0, 1.0, 0.0);
	//bind camo texture
	glBindTexture(GL_TEXTURE_2D, 2003);


	glPushMatrix();
	//move the weapon
	glRotatef(b->aYaw, 0, 1, 0);
	glRotatef(b->aPitch, 0, 0, 1);

	//hammer
	glPushMatrix();
	glTranslatef(0, 2.5, 0);
	glRotatef(b->hPitch, 0, 0, 1);
	//glTranslatef(1.25, 0.25, 0);
	glScalef(1.5, 0.7, 0.8);
	glScalef(0.5, 0.5, 0.5);
	drawCube(&c);
	glPopMatrix();

	//shoulder
	glPushMatrix();
	glTranslatef(0, 1.25, 0);
	glRotated(90, 0, 0, 1);
	glScalef(2.5, 0.5, 0.5);
	glScalef(0.5, 0.5, 0.5);
	drawCube(&c);
	glPopMatrix();

	glPopMatrix();


	//base
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glScalef(0.75, 0.25, 0.75);
	drawCube(&c);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 2002);
	//wheel 1
	glPushMatrix();
	glTranslatef(0.5, 0, 0.75);
	glRotatef(b->wheelAngle, 0, 0, 1);
	glScalef(0.4, 0.4, 0.4);
	drawWheel();
	glPopMatrix();

	//wheel 2
	glPushMatrix();
	glTranslatef(-0.5, 0, 0.75);
	glRotatef(b->wheelAngle, 0, 0, 1);
	glScalef(0.4, 0.4, 0.4);
	drawWheel();
	glPopMatrix();

	//wheel 3
	glPushMatrix();
	glTranslatef(-0.5, 0, -0.75 - 0.20);
	glRotatef(b->wheelAngle, 0, 0, 1);
	glScalef(0.4, 0.4, 0.4);
	drawWheel();
	glPopMatrix();

	//wheel 4
	glPushMatrix();
	glTranslatef(0.5, 0, -0.75 - 0.20);
	glRotatef(b->wheelAngle, 0, 0, 1);
	glScalef(0.4, 0.4, 0.4);
	drawWheel();
	glPopMatrix();

	glPopMatrix();

}

void drawAIBot(Bot *b) {
	//bot 1
	glPushMatrix();
	CubeMesh c = newCube();

	//move bot
	glTranslatef(b->x, b->y, b->z);
	glRotatef(b->botAngle, 0.0, 1.0, 0.0);
	//bind gold texture
	glBindTexture(GL_TEXTURE_2D, 2004);


	glPushMatrix();
	//move the weapon
	glRotatef(b->aYaw, 0, 1, 0);
	glRotatef(b->aPitch, 0, 0, 1);

	//hammer
	glPushMatrix();
	glTranslatef(0, 2.5, 0);
	glRotatef(b->hPitch, 0, 0, 1);
	//glTranslatef(1.25, 0.25, 0);
	glScalef(1.5, 0.7, 0.8);
	glScalef(0.5, 0.5, 0.5);
	drawCube(&c);
	glPopMatrix();

	//shoulder
	glPushMatrix();
	glTranslatef(0, 1.25, 0);
	glRotated(90, 0, 0, 1);
	glScalef(2.5, 0.5, 0.5);
	glScalef(0.5, 0.5, 0.5);
	drawCube(&c);
	glPopMatrix();

	glPopMatrix();


	//base
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glScalef(0.75, 0.25, 0.75);
	drawCube(&c);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 2002);
	//wheel 1
	glPushMatrix();
	glTranslatef(0.5, 0, 0.75);
	glRotatef(b->wheelAngle, 0, 0, 1);
	glScalef(0.4, 0.4, 0.4);
	drawWheel();
	glPopMatrix();

	//wheel 2
	glPushMatrix();
	glTranslatef(-0.5, 0, 0.75);
	glRotatef(b->wheelAngle, 0, 0, 1);
	glScalef(0.4, 0.4, 0.4);
	drawWheel();
	glPopMatrix();

	//wheel 3
	glPushMatrix();
	glTranslatef(-0.5, 0, -0.75 - 0.20);
	glRotatef(b->wheelAngle, 0, 0, 1);
	glScalef(0.4, 0.4, 0.4);
	drawWheel();
	glPopMatrix();

	//wheel 4
	glPushMatrix();
	glTranslatef(0.5, 0, -0.75 - 0.20);
	glRotatef(b->wheelAngle, 0, 0, 1);
	glScalef(0.4, 0.4, 0.4);
	drawWheel();
	glPopMatrix();

	glPopMatrix();
}

void moveAI(Bot *aiBot, Bot *pBot, int thread) {
	//TODO: set ai speed and angle here
	time_t t;
	srand((unsigned)time(&t));
	int randInt = rand() % 30; //

	double xDis = pBot->x - aiBot->x;
	double zDis = pBot->z - aiBot->z;


	double angle = atan(xDis/zDis)*180/PI;
	if (zDis > 0) {
		angle += 270;
	}
	else {
		angle += 90;
	}

	double distance = sqrt(pow((aiBot->x - pBot->x), 2) + pow((aiBot->z - pBot->z), 2));

	if (distance > 15) {
		aiBot->speed = 0.15;
		aiBot->botAngle = angle;
	}
	//bring down the hammer
	if (distance <= 3) {
		if (zDis > 0 && aiBot->aPitch > -80) {
			aiBot->aPitch -= 20;
		}
		else if(zDis < 0 && aiBot->aPitch < 80) {
			aiBot->aPitch += 20;
		}
	}
	else if(aiBot->aPitch < 0){
		aiBot->aPitch += 20;
	}
	else if (aiBot->aPitch > 0) {
		aiBot->aPitch -= 20;
	}

	if (randInt == 25 || thread ==0) {//turn at random time
		//speed is max of 0.2 inc 0.02		
		aiBot->speed = (rand() % 20)/100.0 - (rand() % 20) / 100.0;
		aiBot->botAngle = rand() % 360;
	}
		
	if(thread == 0){
		moveBotOnMesh(aiBot);
	}
}

void moveBotOnMesh(Bot *b) {

	if (b->speed >= 0.01) {
		//printf("forwards\n");
		b->wheelAngle -= b->speed * 30 * 7.0;
		if (b->wheelAngle > 360.0)
			b->wheelAngle -= 360.0;
	}
	else if (b->speed <= 0.01) {
		//printf("backwards\n");
		b->wheelAngle -= b->speed * 30 * 7.0;
		if (b->wheelAngle < 0)
			b->wheelAngle += 360.0;
	}


	b->x += b->speed*cos(PI / 180 * -b->botAngle);
	b->z += b->speed*sin(PI / 180 * -b->botAngle);

	glutTimerFunc(50, moveBotOnMesh, b);

	glutPostRedisplay();

}

double getBotY(Bot *b, QuadMesh *groundMesh, int meshSize) {
	int currentVertex = 0;
	double min = 999;
	double minY = 0;

	for (int i = 0; i < meshSize + 1; i++) {
		for (int j = 0; j < meshSize + 1; j++) {
			double x = groundMesh->vertices[currentVertex].position.x;
			double y = groundMesh->vertices[currentVertex].position.y;
			double z = groundMesh->vertices[currentVertex].position.z;

			double r = sqrt(pow((b->x - x), 2) + pow((b->z - z), 2));//distance to hole
			if (r < min) {
				minY = y;
				min = r;
			}

			currentVertex++;
		}
	}
	return minY;
}


void draw_circle(const GLfloat radius, const GLuint num_vertex)
{
	const GLfloat delta_angle = 2.0*PI / num_vertex;

	glBegin(GL_TRIANGLE_FAN);

	glTexCoord2f(0.5, 0.5);

	glVertex4f(0, 0, 0, 1);

	for (int i = 0; i < num_vertex; i++)
	{
		glTexCoord2f((cos(delta_angle*i) + 1.0)*0.5, (sin(delta_angle*i) + 1.0)*0.5);

		glVertex4f(cos(delta_angle*i) * radius, sin(delta_angle*i) * radius, 0, 1);
	}
	glTexCoord2f((1.0 + 1.0)*0.5, (0.0 + 1.0)*0.5);

	glVertex4f(1.0 * radius, 0, 0, 1);

	glEnd();
}


void drawWheel() {

	glPushMatrix();
	GLUquadricObj *quadric;
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 1, 1, 0.5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	draw_circle(1, 20);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(0, 0, 0.5);
	draw_circle(1, 20);
	glPopMatrix();

}