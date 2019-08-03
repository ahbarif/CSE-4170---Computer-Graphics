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
#include <iostream>
#include <GL/glut.h>
#include "linedrawing.h"

using namespace std;

int Wi=640, He=480;

void myInit (void);
void display(void);
void reshape (int w, int h);

void myInit (void){
    glClearColor(0.0, 0.0, 0.0,1.0);
}

void reshape (int w, int h){
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluOrtho2D (-Wi/2, Wi/2-1, -He/2, He/2-1); // size of projection plane (origin at the center)
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}

int yMax = 130, yMin = -130, xMax = 180, xMin = -180;

struct point{
    int x, y;
    point(){}
    point(int x, int y):x(x), y(y){}
};

point find_by_t(float t, point st, point en){
    int x = st.x + t * (en.x - st.x);
    int y = st.y + t * (en.y - st.y);

    return point(x, y);
}

void drawLargePixel(int x, int y){

      //  if(x>=xMin  && x<=xMax && y>=yMin && y<=yMax){
                glEnd();
                glPointSize(8.0);
                glBegin(GL_POINTS);
                    drawPixel(x, y);
                glEnd();
                glPointSize(1.0);
                glBegin(GL_POINTS);
     //   }

}

void Cyrus_Beck_Line_clipping(int x0, int y0, int x1, int y1){
    float t, teMax= 0.0, tlMin = 1.0;

    printf("(%d, %d) to (%d, %d)\n", x0, y0, x1, y1);

    glColor3f(0.4, 0.4, 0.4);
    drawLine(x0, y0, x1, y1);
    glColor3f(1.0, 1.0, 0.0);


    t = (float)(yMax - y0)/(y1-y0);     // top
    if(y1 >= y0) tlMin = min(tlMin, t);
    else teMax = max(teMax, t);

    t = (float)(yMin - y0)/(y1-y0);     // bottom
    if(y1 < y0) tlMin = min(tlMin, t);
    else teMax = max(teMax, t);

    t = (float)(xMax - x0)/(x1-x0);     // right
    if(x1 >= x0) tlMin = min(tlMin, t);
    else teMax = max(teMax, t);

    t = (float)(xMin - x0)/(x1-x0);    // left
    if(x1 < x0) tlMin = min(tlMin, t);
    else teMax = max(teMax, t);

    if(tlMin<=1.0 && teMax>=0.0){
        if(tlMin >= teMax){

            printf("Accepted:  tLmin: %.5lf    teMax: %.5lf\n", tlMin, teMax);


            point p0 = find_by_t(teMax, point(x0, y0), point(x1, y1));
            point p1 = find_by_t(tlMin, point(x0, y0), point(x1, y1));


            drawLine(p0.x, p0.y, p1.x, p1.y);
        }
        else{
                    printf("Rejected:  tLmin: %.5lf    teMax: %.5lf\n", tlMin, teMax);

        }
    }
    else  printf("Rejected:  tLmin: %.5lf    teMax: %.5lf\n", tlMin, teMax);

 
    t = (float)(yMax - y0)/(y1-y0);         // top
    if(y1 >= y0) glColor3f(0.0, 1.0, 0.0);  // leaving = green
    else glColor3f(0.0, 0.0, 1.0);          // entering = blue
    point pt = find_by_t(t, point(x0, y0), point(x1, y1));
    if(t>=0.0 && t <= 1.0) drawLargePixel(pt.x, pt.y);

    t = (float)(yMin - y0)/(y1-y0);         // bottom
    if(y1 < y0) glColor3f(0.0, 1.0, 0.0);   // leaving = green
    else glColor3f(0.0, 0.0, 1.0);          // entering = blue
    pt = find_by_t(t, point(x0, y0), point(x1, y1));
    if(t>=0.0 && t <= 1.0)     drawLargePixel(pt.x, pt.y);


    t = (float)(xMax - x0)/(x1-x0);         // right
    if(x1 >= x0) glColor3f(0.0, 1.0, 0.0);  // leaving = green
    else glColor3f(0.0, 0.0, 1.0);         // entering = blue
     pt = find_by_t(t, point(x0, y0), point(x1, y1));
    if(t>=0.0 && t <= 1.0)     drawLargePixel(pt.x, pt.y);

      t = (float)(xMin - x0)/(x1-x0);       // left
    if(x1 < x0) glColor3f(0.0, 1.0, 0.0);   // leaving = green
    else glColor3f(0.0, 0.0, 1.0);         // entering = blue
     pt = find_by_t(t, point(x0, y0), point(x1, y1));
    if(t>=0.0 && t <= 1.0)     drawLargePixel(pt.x, pt.y);
}


void drawGrid(){

    drawLine(-320, yMax, 320, yMax);
    drawLine(-320, yMin, 320, yMin);
    drawLine(xMax, 240, xMax, -240);
    drawLine(xMin, 240, xMin, -240);
}

bool callFlag = false;

void display(){

    // if(callFlag == true) return;
    callFlag = true;
    printf("\n\n");
   
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    glColor3f(1.0, 1.0, 1.0);
    drawGrid();

    //  Cyrus_Beck_Line_clipping(-260, -210, 230, 150);
     Cyrus_Beck_Line_clipping(-280, -150, 290, 160);    // partial

    Cyrus_Beck_Line_clipping(-230, -220, 300, -120);  // rejected

    Cyrus_Beck_Line_clipping(-100, 0, 0, 100);  // rejected
//    drawRandomLines(5);

    glEnd();
    glFlush();
}


int main (int argc, char **argv){

  srand(time(NULL));

  glutInit (&argc, argv); // to initialize the toolkit;
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // sets the display mode
  glutInitWindowSize (Wi, He); // sets the window size
  glutInitWindowPosition (0, 0); // sets the starting position for the window
  glutCreateWindow ("Line Drawing"); // creates the window and sets the title
  myInit(); // additional initializations as necessary
  glutReshapeFunc(reshape);

  glutDisplayFunc (display);

  glutMainLoop(); // go into a loop until event occurs
   return 0;
}