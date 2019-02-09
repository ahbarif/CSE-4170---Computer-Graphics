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
#include <unistd.h>
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

int xc = 0, yc = 0;

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

    int xd, yd;

    if(zone == 0) {
        glColor3f(1.0,1.0,1.0);     // white
        xd = x; yd = y;
    }
    else if(zone == 1){
       glColor3f(0.0,1.0,0.0);     // Light green
        xd = y; yd = x;

    }
    else if(zone == 2){
        glColor3f(1.0, 1.0, 0.0);   // Yellow
        xd = -y; yd = x;

    }
    else if(zone == 3){
        glColor3f(1.0, 0.0, 0.0);   // Red
        xd = -x; yd = y;

    }
    else if(zone == 4){
        glColor3f(1.0, 0.0, 1.0);   // Purple
        xd = -x; yd = -y;

    }
    else if(zone == 5){
        glColor3f(0.0, 0.0, 1.0);   // Blue
        xd = -y; yd = -x;

    }
    else if(zone == 6){
        glColor3f(1.0,0.5,0.0);     // Orange
        xd = y; yd = -x;

    }
    else if(zone == 7){
        glColor3f(0.1, 0.5, 0.5);   // Cyan
        xd = x; yd = -y;

    }

    drawPixel(xd+xc, yd+yc);
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
    if(zone == 0) drawLine_0(x0, y0, x1, y1, zone);
    else if(zone == 1) drawLine_0(y0, x0, y1, x1, zone);
    else if(zone == 2) drawLine_0(y0, -x0, y1, -x1, zone);
    else if(zone == 3) drawLine_0(-x0, y0, -x1, y1, zone);
    else if(zone == 4) drawLine_0(-x0, -y0, -x1, -y1, zone);
    else if(zone == 5) drawLine_0(-y0, -x0, -y1, -x1, zone);
    else if(zone == 6) drawLine_0(-y0, x0, -y1, x1, zone);
    else if(zone == 7) drawLine_0(x0, -y0, x1, -y1, zone);
}

void draw8way(int x, int y){
    for(int i = 0; i<8; i++) drawPixel(x, y, i);
}



void drawCircleR0(int r){
    int d = 5 - 4*r;
    int x = r, y = 0;
    draw8way(x, y);

    while(x>y){

      if(d<0){
        d += (8*y + 12);
        y++;
      }
      else{
        d += (-8*x + 8*y + 20);
        x--;
        y++;
      }
      draw8way(x, y);
    }
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();

}

void drawGrid(){
    int ox = 0, oy = 0;
    glColor3f(0.7,0.7,0.7); // grey

    for(int i = -320; i<320; i++) glVertex2i(i, 0);
    for(int i = -240; i<240; i++) glVertex2i(0, i);

}

void drawCircle0r(int r){
  int d = 5 - 4*r;
  int x = 0, y = r;

  draw8way(x, y);

  while(x<y){
    if(d<0) {
      d += (8*x + 12);
      x++;
    }
    else{
      d += (8*x - 8*y + 20);
      x++;
      y--;
    }

    draw8way(x, y);
  }
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glBegin(GL_POINTS);

  //  drawGrid();

    xc = 0;
    yc = 0;

    int dx = 2, dy = 2, radius = 40;

    while(true){
      xc += dx;
      yc += dy;

      if(dx>0){
        if(xc+radius>=Wi/2) dx*= -1;
      }
      else {
        if(xc-radius <= -Wi/2) dx *= -1;
      }

      if(dy>0){
        if(yc+radius>=He/2) dy*= -1;
      }
      else {
        if(yc-radius <= -He/2) dy *= -1;
      }

      if(dx>0 && dy>0) glColor3f(1.0,1.0,1.0);
      else if(dx>0 && dy<0) glColor3f(1.0, 0.0,0.0);
      else if(dx<0 && dy<0) glColor3f(0.0,0.0,1.0);
      else glColor3f(0.0,1.0,0.0);


      glClear(GL_COLOR_BUFFER_BIT);
      glBegin(GL_POINTS);
          drawCircleR0(40);
      glEnd();
      glutSwapBuffers();


    }

    glEnd();
  //  glFlush();
  glutSwapBuffers();
}


int main (int argc, char **argv){

  srand(time(NULL));

  glutInit (&argc, argv); // to initialize the toolkit;
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); // sets the display mode
  glutInitWindowSize (Wi, He); // sets the window size
  glutInitWindowPosition (0, 0); // sets the starting position for the window
  glutCreateWindow ("Line Drawing"); // creates the window and sets the title
  myInit(); // additional initializations as necessary
  glutReshapeFunc(reshape);

  glutDisplayFunc (display);

  glutMainLoop(); // go into a loop until event occurs
   return 0;
}
