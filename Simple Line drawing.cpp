#include <GL/glut.h>
#include<iostream>

void drawPixel(int x, int y){
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}

void drawPixel(float x, float y){
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}

void drawLine(int x0, int y0, int x1, int y1){
	int dy = y1 - y0;
	int dx = x1 - x0;

	float y = (float)y0;

	float m = (float)dy/dx;

	for(int x = x0; x<=x1; x++){
		drawPixel((float)x, y);
		y += m;
	}
}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);

	glutInitWindowPosition(180, 180);
	glutInitWindowSize(640, 480);

	glutCreateWindow("A Simple OpenGL Program");
	gluOrtho2D(-320, 319, -340,239);

//glClear(GL_COLOR_BUFFER_BIT);
//glMatrixMode( GL_PROJECTION );
//glLoadIdentity();

	drawLine(1, 1, 200, 80);



	//for(int i = 1; i<=200; i++) drawPixel(i, 0);
	//glutDisplayFunc(displayMe);
		glutMainLoop();
	return 0;
}
