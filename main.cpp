#include<windows.h>
#include <GL/glut.h>
#include <math.h>

#define pi (2*acos(0.0))

struct point{
    double x,y,z;
};

float x = 0;
float t = 0;
float g = 9.8;
float u = 30;
float h = 70;
float y = h;


struct point eye = {100,100,100};

void drawSphere(double radius,int slices,int stacks)
{
	struct point points[stacks+1][slices+1];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=  radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
		    points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);

		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
			    glColor3f(i/stacks, i/slices, 0.9);
			    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);


                glColor3f(i/stacks, i/slices, 0.9);
			    glVertex3f(points[i][j].x,points[i][j].y, -points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y, -points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y, -points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y, -points[i+1][j].z);

			}glEnd();
		}
	}
}
void drawAxes()
{

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);{
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f( 1000,0,0);
        glVertex3f(-1000,0,0);

        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(0,-1000,0);
        glVertex3f(0, 1000,0);

        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(0,0, 1000);
        glVertex3f(0,0,-1000);
    }glEnd();

}


void drawGrid()
{
	int i;

    glColor3f(0.6, 0.6, 0.6);	//grey
    glBegin(GL_LINES);{
        for(i=-80;i<=80;i++){

            if(i==0)
                continue;	//SKIP the MAIN axes

            //lines parallel to Y-axis
            glVertex3f(i*10, -900, 0);
            glVertex3f(i*10,  900, 0);

            //lines parallel to X-axis
            glVertex3f(-900, i*10, 0);
            glVertex3f( 900, i*10, 0);
        }
    }glEnd();

}

void drawBall()
{
    glPushMatrix();
        glTranslated(0,x,y);
        //glTranslated(0,0,x);
        drawSphere(10, 40, 40);
    glPopMatrix();
}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	gluLookAt(eye.x, eye.y, eye.z,	0,0,0,	0,0,1);



	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    drawBall();

    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){

    t += 0.01;

    x = u*t;
    y = h - g*t*t/2;

    if(y<0)
    {
        y=0;
        t=0;
    }
	//codes for any changes in Models, Camera
	glutPostRedisplay(); // marks the current window as needing to be redisplayed
}

void init(){
	//codes for initialization

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	5000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view -
	//in the X direction (horizontally) = width/height
	//near distance
	//far distance
}

void keyboard_action(unsigned char key, int x, int y)
{
    switch (key){
    case '+':
        eye.x++;
        break;
    case '-':
        eye.x--;
        break;
    case 'u':
        eye.z++;
        break;
    case 'd':
        eye.z--;
        break;
    case 'l':
        eye.y--;
        break;
    case 'r':
        eye.y++;
        break;
    }
}

int main(int argc, char **argv){

	glutInit(&argc,argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
	/*
	GLUT_DOUBLE - allows for display on the double buffer window

    GLUT_RGB - shows color (Red, green, blue)

    GLUT_DEPTH - allows for depth buffer
    */

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);
	//When depth testing is enabled, OpenGL tests the depth value
	//of a fragment against the content of the depth buffer

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)
	glutKeyboardFunc(keyboard_action);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}



