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

void drawPixel(int x, int y){
    glVertex2i(x, y);
}
void drawPixel(int x, int y, int zone){
    if(zone == 0) {
        glColor3f(1.0,1.0,1.0);     // white
        drawPixel(x, y);

    }
    else if(zone == 1){
        glColor3f(0.0,1.0,0.0);     // Light green
        drawPixel(y, x);
    }
    else if(zone == 2){
        glColor3f(1.0, 1.0, 0.0);   // Yellow
        drawPixel(-y, x);
    }
    else if(zone == 3){
        glColor3f(1.0, 0.0, 0.0);   // Red
        drawPixel(-x, y);
    }
    else if(zone == 4){
        glColor3f(1.0, 0.0, 1.0);   // Purple
        drawPixel(-x, -y);
    }
    else if(zone == 5){
        glColor3f(0.0, 0.0, 1.0);   // Blue
        drawPixel(-y, -x);
    }
    else if(zone == 6){
        glColor3f(1.0,0.5,0.0);     // Orange
        drawPixel(y, -x);
    }
    else if(zone == 7){
        glColor3f(0.1, 0.5, 0.5);   // Cyan
        drawPixel(x, -y);
    }
    
}

void drawLine_0(int x0, int y0, int x1, int y1, int zone){
    int dx = x1 - x0, dy = y1 - y0;
    int d = 2*dy - dx, dE = 2 * dy, dNE = 2*(dy-dx);
    int x = x0, y = y0;
    
    drawPixel(x, y, zone);
    
    while(x<x1){
        if(d>0) {
            x++;
            y++;
            d += dNE;
        }
        else {
            x++;
            d += dE;
        }
        drawPixel(x, y, zone);
    }
}

int getZone(int x0, int y0, int x1, int y1){
        int dx = x1 - x0, dy = y1 - y0;
        
        if(dx>=0 && dy>=0){
            if(dx>=dy) return 0;
            else return 1;
        }
        else if(dx<0 && dy>=0){
            if(abs(dx)>=dy) return 3;
            else return 2;
        }
        else if(dx<0 && dy<0){
            if(abs(dx)>=abs(dy)) return 4;
            else return 5;
        }
        else{
            if(dx>=abs(dy)) return 7;
            else return 6;
        }
}

void drawLine(int x0, int y0, int x1, int y1){
    int zone = getZone(x0, y0, x1, y1);
   // cout<<"zone = " <<zone<<endl;
    if(zone == 0) drawLine_0(x0, y0, x1, y1, zone);
    else if(zone == 1) drawLine_0(y0, x0, y1, x1, zone);
    else if(zone == 2) drawLine_0(y0, -x0, y1, -x1, zone);
    else if(zone == 3) drawLine_0(-x0, y0, -x1, y1, zone);
    else if(zone == 4) drawLine_0(-x0, -y0, -x1, -y1, zone);
    else if(zone == 5) drawLine_0(-y0, -x0, -y1, -x1, zone);
    else if(zone == 6) drawLine_0(-y0, x0, -y1, x1, zone);
    else if(zone == 7) drawLine_0(x0, -y0, x1, -y1, zone);
}

void drawCircle(int x0, int y0, int r){

    for(int t = 0; t<360; t += 10){
        int x1 = x0 + r*cos(t);
        int y1 = y0 + r*sin(t);
        drawLine(x0, y0, x1, y1);
    }

}

void drawRandomCircles(int limit){
  //  640 * 480

    for(int i = 0; i<limit; i++){
        int x = rand()%640 - 320;
        int y = rand()%480 - 240;
        int r = rand()%65 + 35;
        
        drawCircle(x, y, r);
    }

}

void drawRandomLines(int limit){
     for(int i = 0; i<limit; i++){
        int x0 = rand()%640 - 320;
        int y0 = rand()%480 - 240;
        int x1 = rand()%640 - 320;
        int y1 = rand()%480 - 240;
    
        drawLine(x0, y0, x1, y1);
    }
}


void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
   
 
    // drawLine(20, 5, 200, 100);      // 0
    // drawLine(5, 20, 100, 200);      // 1
    // drawLine(-5, 20, -100, 200);    // 2
    // drawLine(-20, 5, -200, 100);    // 3
    // drawLine(-20, -5, -200, -100);  // 4
    // drawLine(-5, -20, -100, -200);  // 5
    // drawLine(5, -20, 100, -200);    // 6
    // drawLine(20, -5, 200, -100);    // 7

    drawRandomCircles(20);
   // drawRandomLines(50);


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
