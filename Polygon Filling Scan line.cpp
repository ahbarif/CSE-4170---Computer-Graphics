#include <bits/stdc++.h>
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
    if(zone == 0)    drawPixel(x, y);
    else if(zone == 1) drawPixel(y, x);
    else if(zone == 2) drawPixel(-y, x);
    else if(zone == 3) drawPixel(-x, y);
    else if(zone == 4) drawPixel(-x, -y);
    else if(zone == 5) drawPixel(-y, -x);
    else if(zone == 6) drawPixel(y, -x);
    else if(zone == 7) drawPixel(x, -y);
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

struct point{
    int x, y;
    point(){}
    point(int x, int y):x(x), y(y){}
    bool operator<(const point &p) const{
        return y < p.y;
    }
};

struct Edge{
    point st, en;
    Edge(){}
    Edge(point st, point en):st(st), en(en){}
    bool operator<(const Edge &E)const{
        return st.y < E.st.y;
    }
};

struct EdgeBucketEntry{
    int yVal;
    float xVal, slopeInverse;
    bool lambda;
    EdgeBucketEntry(){}
    bool operator<(const EdgeBucketEntry &E)const{
        return xVal < E.xVal;
    }
};

vector<point> polygonVertices;
vector<Edge> polygonEdges;
vector<EdgeBucketEntry> edgeTable[1005], activeEdgeTable[1005];
int n;

void input(){
    
    freopen("in.txt", "r", stdin);
    scanf("%d", &n);
    for(int i = 0; i<n; i++){
        int x, y;
        scanf("%d %d", &x, &y);
        polygonVertices.push_back(point(x, y));
    }
}


void addToEdgeTable(int idx){

    Edge E = polygonEdges[idx];

    point st = E.st, en = E.en;

    int pos = st.y, yVal = en.y, xVal = st.x;
    int dx = en.x - st.x, dy = en.y - st.y;

    float slope = (float)dx/dy;

    bool flag = false;

    if(idx+1 < n && polygonEdges[idx+1].st.y == E.st.y) flag = true;

    EdgeBucketEntry tuple;

    tuple.xVal = xVal;
    tuple.yVal = yVal;
    tuple.lambda = flag;
    tuple.slopeInverse = slope;

    edgeTable[pos].push_back(tuple);
    return;
}

void drawLargePixel(int x, int y, int f){

        if(f == 0) glColor3f(0.4, 0.4, 0.4);
        else if(f == 1) glColor3f(1.0, 0.5, 0.0);
        else glColor3f(1.0, 0.0, 0.0);
        
        glEnd();
        glPointSize(5.0);
        glBegin(GL_POINTS);
            drawPixel(x, y);
        glEnd();
        glPointSize(1.0);
        glBegin(GL_POINTS);
}

void propagate(EdgeBucketEntry tuple, int index){
    int startIndex = index;
    int endIndex = tuple.yVal - 1;

    for(int i = startIndex; i<=endIndex; i++){
        activeEdgeTable[i].push_back(tuple);
        tuple.xVal += tuple.slopeInverse;
    }
}

int yMin = INT_MAX, yMax = INT_MIN;


void convertToActiveEdgeTable(){

        for(int i = yMin; i<=yMax; i++){
            for(int j = 0; j<edgeTable[i].size(); j++){
                EdgeBucketEntry tuple = edgeTable[i][j];
                propagate(tuple, i);
            }
        }

        for(int i = yMax; i>=yMin; i--){
    
        printf("y = %2d: ", i);
        
            for(int j = 0; j<activeEdgeTable[i].size(); j++){
                EdgeBucketEntry tuple = activeEdgeTable[i][j];
                printf(" [%2d, %2d, %.2f, %d] ", tuple.yVal, tuple.xVal, tuple.slopeInverse, tuple.lambda);
            }
            cout<<endl;
    }
}

vector<int> pointFrequency; // for lab task only

void generateEdgeTable(){

    polygonVertices.clear();
    polygonEdges.clear();
        map<point, int> mp;

    for(int i = 0; i<1005; i++) edgeTable[i].clear();

    input();

    for(int i = 0; i<n; i++){
        point st = polygonVertices[i];
        point en = polygonVertices[(i+1)%n];

        if(st.y>en.y) swap(st, en); 
    
        yMax = max(yMax, en.y);
        yMin = min(yMin, st.y);
    
        if(st.y != en.y)   {
             polygonEdges.push_back(Edge(st, en));
             mp[st]++;
        }
    }

    sort(polygonEdges.begin(), polygonEdges.end());

    for(int i = 0; i<polygonEdges.size(); i++) {
        addToEdgeTable(i);
    }

    pointFrequency.clear();
    

    for(int i = yMax; i>=yMin; i--){

        if(edgeTable[i].size() == 0) continue;
    
        printf("y = %2d: ", i);
        
            for(int j = 0; j<edgeTable[i].size(); j++){
                EdgeBucketEntry tuple = edgeTable[i][j];
                printf(" [%2d, %2d, %.2f, %d] ", tuple.yVal, tuple.xVal, tuple.slopeInverse, tuple.lambda);
            }
            cout<<endl;
    }

    for(int i = 0; i<n; i++){
        pointFrequency.push_back(mp[polygonVertices[i]]);
    }

    convertToActiveEdgeTable();
   
    return;
}

void Solve(){
    for(int i = 0; i<n; i++){
        point p = polygonVertices[i];
        point q = polygonVertices[(i+1)%n];
        glColor3f(1.0, 1.0, 1.0);
        drawLine(p.x, p.y, q.x, q.y);

        drawLargePixel(p.x, p.y, pointFrequency[i]);
    }
}

void runScanLine(int Y){

    if(activeEdgeTable[Y].size()==0) return;
    sort(activeEdgeTable[Y].begin(), activeEdgeTable[Y].end());

    int parity = 0;

    for(int i = 0; i<activeEdgeTable[Y].size(); i += 2){
        EdgeBucketEntry st = activeEdgeTable[Y][i];
        EdgeBucketEntry en = activeEdgeTable[Y][i+1];

        int stX = (int)st.xVal;
        int enX = (int)en.xVal;

        for(int j = stX; j<=enX; j++) drawPixel(j, Y);
    }
}
void fillPolygon(){

    glColor3f(0.0, 1.0, 0.0);
    for(int i = yMin; i<=yMax; i+=2) runScanLine(i);
}

void display(){
  
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(1.0);
    glBegin(GL_POINTS);


    glColor3f(1.0, 1.0, 1.0);
    
    Solve();

   fillPolygon();


    glEnd();
    glFlush();
}


int main (int argc, char **argv){

  generateEdgeTable();

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