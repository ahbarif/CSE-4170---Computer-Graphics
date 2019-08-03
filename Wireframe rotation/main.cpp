#include <bits/stdc++.h>
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
//   gluOrtho2D (-Wi/4, Wi/4-1, -He/4, He/4-1);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}

// Line drawing Library
void drawPixel(int x, int y);
void drawPixel(int x, int y, int zone);
void drawLine_0(int x0, int y0, int x1, int y1, int zone);
int getZone(int x0, int y0, int x1, int y1);
void drawLine(int x0, int y0, int x1, int y1);

typedef double i32;

struct point{
    i32 x, y, z;
    point(){}
    point(i32 x, i32 y, i32 z):x(x), y(y), z(z){}
};

struct Line{
    point st, en;
    
    Line(){}
    Line(point st, point en): st(st), en(en){}

};

vector<Line> cubeLines;
vector<Line> projectedLines;

i32 zp, qdx, qdy, qdz;
point cop;

void input(){
    
    freopen("in.txt", "r", stdin);
    cin>>cop.x>>cop.y>>cop.z>>zp;
    
    int n;
    scanf("%d", &n);
    for(int i = 0; i<n; i++){
        point st, en;
        i32 x, y, z;
        cin>>x>>y>>z;

        st = point(x, y, z);
        
        cin>>x>>y>>z;
        en = point(x, y, z);

        cubeLines.push_back(Line(st, en));
    }


     qdx = cop.x;
     qdy = cop.y;
     qdz = cop.z - zp;


}
void project();

void display(){
  
    glClear(GL_COLOR_BUFFER_BIT);
    
    project();
}


point projectPoint(point p){

    i32 x = p.x;
    i32 y = p.y;
    i32 z = p.z;

    i32 xnew = (x - z*(qdx/qdz) + zp * (qdx/qdz)) / ((-z/qdz) + 1 + (zp/qdz));
    i32 ynew = (y - z*(qdy/qdz) + zp * (qdy/qdz)) / ((-z/qdz) + 1 + (zp/qdz));
    i32 znew = zp;

    return point(xnew, ynew, znew);
}


void solve(){

    input();
}

void general_rotation(double x, double y, double z, double &x1, double &y1, double &z1, double angle) {
	double PI = acos(-1.0);
    double value = (angle * PI) / 180.0;
	x1 = x * cos(value) - y * sin(value);
	y1 = x * sin(value) + y * cos(value);
	z1 = z;
	return;
}

void rotate(double angle, int choice){

    double x1, y1, z1, x2, y2, z2;
    
    for(int i = 0; i<cubeLines.size(); i++){

         Line L = cubeLines[i];

         if(choice == 0) // x axis so x, y, z
         {
            general_rotation(L.st.x, L.st.y, L.st.z, x1, y1, z1, angle);
            general_rotation(L.en.x, L.en.y, L.en.z, x2, y2, z2, angle);

         }
         else if(choice == 1){ // y axis so y, z, x
    
            general_rotation(L.st.y, L.st.z, L.st.x, y1, z1, x1, angle);
            general_rotation(L.en.y, L.en.z, L.en.x, y2, z2, x2, angle);
         }
         else if(choice == 2){ // z axis so z, x, y
            
            general_rotation(L.st.z, L.st.x, L.st.y, z1, x1, y1, angle);
            general_rotation(L.en.z, L.en.x, L.en.y, z2, x2, y2, angle);
         } 
           cubeLines[i].st.x = x1;
            cubeLines[i].st.y = y1;
            cubeLines[i].st.z = z1;

            cubeLines[i].en.x = x2;
            cubeLines[i].en.y = y2;
            cubeLines[i].en.z = z2; 
     }
}

void drawCube(){


     glClear(GL_COLOR_BUFFER_BIT);
      glBegin(GL_POINTS);
      glColor3f(0.0, 1.0, 0.0);
          for(int i = 0; i<projectedLines.size(); i++){
        point st = projectedLines[i].st;
        point en = projectedLines[i].en;
        drawLine((int)st.x, (int)st.y, (int)en.x, (int)en.y);
    }
      glEnd();
      glutSwapBuffers();
}

void project(){


     projectedLines.clear();

     for(int i = 0; i<cubeLines.size(); i++){
         Line line = cubeLines[i];

         point newSt = projectPoint(line.st);
         point newEn = projectPoint(line.en);        
         projectedLines.push_back(Line(newSt, newEn));
     }

     drawCube();

}



void catchKey(int key, int x, int y) {

    // int choice = 0, angle = 0;

	if (key == GLUT_KEY_LEFT) {
		printf("Left key is pressed\n");
		rotate(1.0, 0);
		//drawCube();
	}
	else if (key == GLUT_KEY_RIGHT) {
		printf("Right key is pressed\n");
		rotate(-1.0, 0);
	//	drawCube();
	}
	else if (key == GLUT_KEY_DOWN) {
		printf("Down key is pressed\n");
        rotate(1.0, 2);
	}
	else if (key == GLUT_KEY_UP) {
		printf("Up key is pressed\n");
        rotate(-1.0, 2);
	}

    project();  

}

int main (int argc, char **argv){

  solve();

  glutInit (&argc, argv); // to initialize the toolkit;
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // sets the display mode
  glutInitWindowSize (Wi, He); // sets the window size
  glutInitWindowPosition (0, 0); // sets the starting position for the window
  glutCreateWindow ("Line Drawing"); // creates the window and sets the title
  myInit(); // additional initializations as necessary
  glutReshapeFunc(reshape);

  glutDisplayFunc (display);
  glutSpecialFunc(catchKey);


  glutMainLoop(); // go into a loop until event occurs
   return 0;
}
