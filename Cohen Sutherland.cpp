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
   //     glColor3f(1.0,1.0,1.0);     // white
        drawPixel(x, y);
    }
    else if(zone == 1){
     //   glColor3f(0.0,1.0,0.0);     // Light green
        drawPixel(y, x);
    }
    else if(zone == 2){
       // glColor3f(1.0, 1.0, 0.0);   // Yellow
        drawPixel(-y, x);
    }
    else if(zone == 3){
      //  glColor3f(1.0, 0.0, 0.0);   // Red
        drawPixel(-x, y);
    }
    else if(zone == 4){
      //  glColor3f(1.0, 0.0, 1.0);   // Purple
        drawPixel(-x, -y);
    }
    else if(zone == 5){
      //  glColor3f(0.0, 0.0, 1.0);   // Blue
        drawPixel(-y, -x);
    }
    else if(zone == 6){
       // glColor3f(1.0,0.5,0.0);     // Orange
        drawPixel(y, -x);
    }
    else if(zone == 7){
      //  glColor3f(0.1, 0.5, 0.5);   // Cyan
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
    if(zone == 0) drawLine_0(x0, y0, x1, y1, zone);
    else if(zone == 1) drawLine_0(y0, x0, y1, x1, zone);
    else if(zone == 2) drawLine_0(y0, -x0, y1, -x1, zone);
    else if(zone == 3) drawLine_0(-x0, y0, -x1, y1, zone);
    else if(zone == 4) drawLine_0(-x0, -y0, -x1, -y1, zone);
    else if(zone == 5) drawLine_0(-y0, -x0, -y1, -x1, zone);
    else if(zone == 6) drawLine_0(-y0, x0, -y1, x1, zone);
    else if(zone == 7) drawLine_0(x0, -y0, x1, -y1, zone);
}

int yMax = 130, yMin = -130, xMax = 180, xMin = -180;
int TOP = 8, BOTTOM = 4, RIGHT = 2, LEFT = 1;

int makeCode(int x, int y){
    int code = 0;
    if(y>yMax) code += TOP;
    else if(y<yMin) code += BOTTOM;
    if(x > xMax) code += RIGHT;
    else if(x < xMin) code += LEFT;

    return code;
}


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

        if(x>=xMin  && x<=xMax && y>=yMin && y<=yMax){

               
                
                glEnd();
                glPointSize(4.0);
                glBegin(GL_POINTS);
                    drawPixel(x, y);
                glEnd();
                glPointSize(1.0);
                glBegin(GL_POINTS);
        }

}

void Cohen_SutherLand(int x0, int y0, int x1, int y1){
    
    int code0 = makeCode(x0, y0);
    int code1 = makeCode(x1, y1);

    int x, y;

    int partial = 0;

    while(true){
            if((code0 | code1) == 0){

                  


                if(partial == 0) glColor3f(0.0, 1.0, 0.0);
                else glColor3f(1.0, 1.0, 0.0);

                drawLargePixel(x0, y0);
                drawLargePixel(x1, y1);

                drawLine(x0, y0, x1, y1);
                return;
            }

            else if( code0 & code1){
                if(partial == 0) glColor3f(1.0, 0.0, 0.0);
                else glColor3f(0.5, 0.5, 0.5);
                drawLine(x0, y0, x1, y1);

                drawLargePixel(x0, y0);
                drawLargePixel(x1, y1);

                return;
            }

            else{
                partial = 1;
                int code;
                if(code0) code = code0;
                else code = code1;

                if(code & TOP) // above yMax
                {
                    y = yMax;
                    x = x0 + (y-y0)*(x1-x0)/(y1-y0);
                }
                else if(code & BOTTOM){
                    y = yMin;
                    x = x0 + (y-y0)*(x1-x0)/(y1-y0);
                }
                else if(code & RIGHT){
                    x = xMax;
                    y = y0 + (x-x0)*(y1-y0)/(x1-x0);

                }
                else{
                    x = xMin;
                    y = y0 + (x-x0)*(y1-y0)/(x1-x0);
                }

                

                if(code == code0){

                    glColor3f(0.5, 0.5, 0.5);                   
                    drawLine(x0, y0, x, y);

                    // glPointSize(2);
                    // drawPixel(x, y);
                    // glPointSize(1);
                    x0 = x;
                    y0 = y;
                    code0 = makeCode(x0, y0);
                }
                else
                {
                    glColor3f(0.5, 0.5, 0.5);                    
                    drawLine(x1, y1, x, y);

                    
                    x1 = x;
                    y1 = y;

                    code1 = makeCode(x1, y1);
                }
                

            
            }
    }
    
}
void drawRandomLines(int limit){
     for(int i = 0; i<limit; i++){
        int x0 = rand()%640 - 320;
        int y0 = rand()%480 - 240;
        int x1 = rand()%640 - 320;
        int y1 = rand()%480 - 240;

        Cohen_SutherLand(x0, y0, x1, y1);

    }
}

void drawGrid(){

    drawLine(-320, yMax, 320, yMax);
    drawLine(-320, yMin, 320, yMin);
    drawLine(xMax, 240, xMax, -240);
    drawLine(xMin, 240, xMin, -240);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
   
   



    glBegin(GL_POINTS);


    glColor3f(1.0, 1.0, 1.0);

    drawGrid();


     drawRandomLines(20);


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