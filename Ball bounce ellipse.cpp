#ifdef _WIN32
#include <windows.h>
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"GLUT32.lib")
#pragma warning(disable:4244)
#pragma warning(disable:4305)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <GL/glut.h>
#define  PI acos(-1.0)
#define  COLORED false

using namespace std;

int Wi=640, He=480;


void myInit (void);
void display(void);
void reshape (int w, int h);

void GridDraw(void);
void drawPixel(int x,int y);
void draw_line0(int x,int y,int x0,int y0);
int zoneDetection(int x0,int y0,int x1,int y1);
void drawLine(int x0,int y0,int x1,int y1);

void myInit (void)
{
    glClearColor(0.0, 0.0, 0.0,1.0);
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D (-Wi/2, Wi/2-1, -He/2, He/2-1); // size of projection plane (origin at the center)
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}


void GridDraw(void)
{
    glBegin(GL_LINES);

    glColor3f(1.0,1.0,1.0);
    glVertex2i(-320,0);
    glVertex2i(319,0);
    glVertex2i(0,239);
    glVertex2i(0,-240);


    glEnd();

}
void drawPixel(int x, int y)
{
    glVertex2i(x, y);
}

void drawPixel(int x, int y, int cx, int cy)
{
    glVertex2i(x+cx, y+cy);
}

void drawPixel4Way(int x, int y, int cx, int cy){
    drawPixel(x,  y, cx, cy);
    drawPixel(x,  -y, cx, cy);
    drawPixel(-x,  y, cx, cy);
    drawPixel(-x,  -y, cx, cy);
}

void drawEllipse(int a, int b, int cx, int cy){
    int d = 4*b*b-4*a*a*(b-1);
    int x = 0, y = b;

    drawPixel4Way(x, y, cx, cy);

    while(4*a*a*(y-0.5) > 4*b*b*(x+1)){
        if(d<0) //dE
        {
            d += 4*b*b*(2*x + 3);
            x++;
        }
        else{
            d += 4*b*b*(2*x+3) + 4*a*a*(-2*y+2);
            x++;
            y--;
        }
        drawPixel4Way(x, y, cx, cy);
    }

    while(y>0){
        if(d<0) //sE
        {
            d += 4*a*a*(3-2*y) + 4*b*b*(2*x+2);
            x++;
            y--;
        }
        else{
            d += 4*a*a*(3-2*y);
            y--;
        }
        drawPixel4Way(x, y, cx, cy);
    }
}

void adjust(int cy, int &dx, int b, int minB){

    int sign = (dx<0?-1:1);

    if(sign == -1 && b<50){
        dx = 3*sign;
        return;
    }
    if(sign == 1 && b<50){
        dx = 3*sign;
        return;
    }


    if(sign == 1 && cy>=60) {
        dx = -1;
    }
    else if(cy>=-30){
        dx = 3;
    }
    else dx = 6;

    dx *= sign;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
   
    int yMin = -He/2, yMax = 190;
    int cx = 0, cy = 140, baseRadius = 50;
    int a = baseRadius, b = baseRadius;
    int dx = -1;
    int minB = 10;

     

    bool reducing = false, increasing = false;

    while(true){
    //    printf("%d\n", dx);

       adjust(cy, dx, b, minB);

        cy += dx;


        if(dx<0) // falling
        {
        //    glColor3f(1.0, 1.0, 1.0);
            if(cy - b <= yMin){

                if(b>minB){
                    b += dx;
                    //a -= dx;
                    a = (int)sqrt(2*baseRadius*baseRadius - b*b);
                }
                else dx*=-1;
            }
        }
        else{ // increasing
            //            glColor3f(0.0, 1.0, 0.0);

            if(b<baseRadius){
                 b += dx;
                 a = (int)sqrt(2*baseRadius*baseRadius - b*b);
                if(cy - b < yMin) cy += dx;
            }
            if(cy + b >= yMax) dx *= -1;

        }
       
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POINTS);
            drawEllipse(a, b, cx, cy);
        glEnd();
        glutSwapBuffers();
    }

    glEnd();
    glutSwapBuffers();
}


int main (int argc, char **argv)
{

    glutInit (&argc, argv); // to initialize the toolkit;
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); // sets the display mode
    glutInitWindowSize (Wi, He); // sets the window size
    glutInitWindowPosition (0, 0); // sets the starting position for the window
    glutCreateWindow ("Graphics Lab 2"); // creates the window and sets the title
    myInit(); // additional initializations as necessary
    glutReshapeFunc(reshape);
    glutDisplayFunc (display);
    glutMainLoop(); // go into a loop until event occurs
    return 0;
}
