#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include "Vector3D.h"
#include "QuadMesh.h"
#include "Matrix3D.h"
#include "CubeMesh.h"
#include "RGBpixmap.h"

const int meshSize = 32;    // Default Mesh Size
const int vWidth = 650;     // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels

#define PI 3.14159265

int camr = 20;

double val = PI / 180;
double thetaC = (GLdouble)PI/2;
double phiC = (GLdouble)PI/4;

GLdouble camx = 0;
GLdouble camy = 8.4852;
GLdouble camz = 8.4852;

int sYaw;
int sPitch;
int ePitch;

int sRoll;//bonus

static int currentButton;
static unsigned char currentKey;

GLdouble width = 300;
GLdouble height = 300;
GLdouble zoom = 60;

// Lighting/shading and material properties for submarine - upcoming lecture - just copy for now

// Light properties
static GLfloat light_position0[] = { -6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_position1[] = { 6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };

// Material properties
static GLfloat submarine_mat_ambient[] = { 0.4F, 0.2F, 0.0F, 1.0F };
static GLfloat submarine_mat_specular[] = { 0.1F, 0.1F, 0.0F, 1.0F };
static GLfloat submarine_mat_diffuse[] = { 0.9F, 0.5F, 0.0F, 1.0F };
static GLfloat submarine_mat_shininess[] = { 0.0F };

// A quad mesh representing the ground / sea floor 
static QuadMesh groundMesh;

static CubeMesh cube;

// Structure defining a bounding box, currently unused
//struct BoundingBox {
//    Vector3D min;
//    Vector3D max;
//} BBox;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
Vector3D ScreenToWorld(int x, int y);




RGBpixmap pix[2]; // make six empty pixmaps, one for each side of cube



int main(int argc, char **argv)
{

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(vWidth, vHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Assignment 2");

    // Initialize GL
    initOpenGL(vWidth, vHeight);

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotionHandler);
	//glutPassiveMotionFunc(mouseMotionHandler);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(functionKeys);

    // Start event loop, never returns
    glutMainLoop();

    return 0;
}


void createHoles() {
	//x z, b a 
	float holes_hills[12][4] = { { 0,0,-4,0.01},
								{ -8,5,-1, 0.5 },
								{ 10,8,-2, 0.07 },

								//hills
								{ 4,0, 3, 0.5},
								{ 8,0, 3, 0.4 },

								{ -4,0, 3, 0.3 },
								{ -11,0, 7, 0.1 },

								{ 12,-9, 2,0.07 },
								{ -10,7, 3, 0.5 },
							
								{ 13,-15, 4, 0.2 },
								{ 5,-15, 4, 0.2 },
								{ -12,-15, 4, 0.05 }
	};
	
	for (int k = 0; k < sizeof(holes_hills)/sizeof(holes_hills[0]); k++) {
		int currentVertex = 0;
		float a = holes_hills[k][3];
		float b = holes_hills[k][2];

		float vx = holes_hills[k][0];
		float vy = 0;
		float vz = holes_hills[k][1];

		for (int i = 0; i < meshSize + 1; i++) {
			for (int j = 0; j < meshSize + 1; j++) {
				double x = groundMesh.vertices[currentVertex].position.x;
				double z = groundMesh.vertices[currentVertex].position.z;

				double r = sqrt(pow((vx - x), 2) + pow((vz - z), 2));//distance to hole
				groundMesh.vertices[currentVertex].position.y += b*exp(-a*pow(r, 2));
				currentVertex++;
			}
		}
	}
	ComputeNormalsQM(&groundMesh);
}

// Set up OpenGL. For viewport and projection setup see reshape(). */
void initOpenGL(int w, int h)
{

	// Other OpenGL setup
	glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
	//glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
	glClearColor(0.6F, 0.6F, 0.6F, 0.0F);  // Color and depth for glClear
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective

	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	readBMPFile(&pix[0], "..\\grass2.bmp");  // read texture for side 1 from image
	setTexture(&pix[0], 2000);

	readBMPFile(&pix[1], "..\\grass3.bmp");  // read texture for side 1 from image
	setTexture(&pix[1], 2001);

	/*
	//glGenTextures(1, 2000);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   // store pixels by byte	
	glBindTexture(GL_TEXTURE_2D, 2000); // select current texture
	*/
	// Set up texture mapping assuming no lighting/shading 
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // Set up and enable lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);   // This light is currently off

    // Set up ground/sea floor quad mesh
    Vector3D origin = NewVector3D(-16.0f, 0.0f, 16.0f);
    Vector3D dir1v = NewVector3D(1.0f, 0.0f, 0.0f);
    Vector3D dir2v = NewVector3D(0.0f, 0.0f, -1.0f);
    groundMesh = NewQuadMesh(meshSize);
    InitMeshQM(&groundMesh, meshSize, origin, 32.0, 32.0, dir1v, dir2v);

    Vector3D ambient = NewVector3D(0.0f, 0.05f, 0.0f);
    Vector3D diffuse = NewVector3D(0.4f, 0.8f, 0.4f);
    Vector3D specular = NewVector3D(0.04f, 0.04f, 0.04f);
    //SetMaterialQM(&groundMesh, ambient, diffuse, specular, 0.2);

	createHoles();
    // Set up the bounding box of the scene
    // Currently unused. You could set up bounding boxes for your objects eventually.
    //Set(&BBox.min, -8.0f, 0.0, -8.0);
    //Set(&BBox.max, 8.0f, 6.0,  8.0);
}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom, (GLdouble)width / height, 0.2, 40.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Set up the camera at position (0, 6, 12) looking at the origin, up along positive y axis
	gluLookAt(camx, camy, camz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Apply transformations to move submarine
    // ...
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 2001);


	glPushMatrix();
	
	//CubeMesh d = newCube();

	glPopMatrix();

	glPushMatrix();
	GLUquadricObj *quadric;
	quadric = gluNewQuadric();
	glTranslatef(10, 1, 1);
	gluQuadricTexture(quadric, GL_TRUE);
	gluSphere(quadric, .5, 36, 18);

	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 2000);


	glPushMatrix();
    // Draw ground/sea floor
    DrawMeshQM(&groundMesh, meshSize);
	glPopMatrix();

    glutSwapBuffers();   // Double buffering, swap buffers
}

// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
    // Set up viewport, projection, then change to modelview matrix mode - 
    // display function will then set up camera and do modeling transforms.
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	width = w;
	height = h;
}

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'z':
		zoom -= 1;
		break;
	case 'Z':
		zoom += 1;
		break;
	}
	if (zoom < 30) {
		zoom += 1;
	}
	else if (zoom > 70) {
		zoom -= 1;
	}

    glutPostRedisplay();   // Trigger a window redisplay
}


// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
    // Help key
    if (key == GLUT_KEY_F1)
    {
		printf("-----------------------------------------Help Menu--------------------------------------------\n");
		printf("To change the elbow pitch use the 'e' and 'E' keys\n");
		printf("To change the shoulder pitch use the 'r' and 'R' keys\n");
		printf("To change the shoulder yaw use the 't' and 'T' keys\n");
		printf("To change the shoulder roll use the 'q' and 'Q' keys\n");

		printf("To zoom the camera in and out use the 'z' and 'Z' keys, respectively\n");
		
		printf("To change the orientation of the camera, left click down and move mouse left, right, up or down\n");
		printf("--------------------------------------------------s--------------------------------------------\n");
    }

    glutPostRedisplay();   // Trigger a window redisplay
}

int fClick = 0;
// Mouse button callback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
    currentButton = button;

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
			fClick = 1;

        }
		if (state == GLUT_UP)
		{
			
			;
		}
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
        {
            ;
        }
        break;
    default:
        break;
    }

    glutPostRedisplay();   // Trigger a window redisplay
}
int px = 0;
int py = 0;
int signp = 1;
int signt = 1;
// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
    if (currentButton == GLUT_LEFT_BUTTON)
    {
		if (fClick == 0) {
			int dx = xMouse - px;
			int dy = yMouse - py;

			phiC += dx * 0.01;
			thetaC += dy* 0.01;
			
			camx = camr*cos(phiC)*cos(thetaC);
			camy = camr*cos(phiC)*sin(thetaC);
			camz = camr*sin(phiC);
			if (camy < 0) {
				camy = 0;
			}
			
		}
		fClick = 0;
		px = xMouse;
		py = yMouse;
		//printf("%lf %lf %lf %lf %lf \n", camx, camy, camz, thetaC*(180/PI), phiC*(180/PI));
	//gluLookAt(camx, camy, camz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    }
    glutPostRedisplay();   // Trigger a window redisplay
}


Vector3D ScreenToWorld(int x, int y)
{
    // you will need to finish this if you use the mouse
    return NewVector3D(0, 0, 0);
}



