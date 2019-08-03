#include <iostream>
#include <vector>
#include <algorithm>
#include <GL/glut.h>
#include "linedrawing.h"
using namespace std;

int Wi = 640, He = 480;

void myInit(void);
void display(void);
void reshape(int w, int h);

void myInit(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-Wi / 2, Wi / 2 - 1, -He / 2, He / 2 - 1); // size of projection plane (origin at the center)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

struct point
{
    int x, y;
    point() {}
    point(int x, int y) : x(x), y(y) {}
    bool operator<(const point &p) const
    {
        return y < p.y;
    }
};

struct Edge
{
    point st, en;
    Edge() {}
    Edge(point st, point en) : st(st), en(en) {}
    bool operator<(const Edge &E) const
    {
        return st.y < E.st.y;
    }
};

struct EdgeBucketEntry
{
    int yVal;
    float xVal, slopeInverse;
    int lambda;
    EdgeBucketEntry() {}
    bool operator<(const EdgeBucketEntry &E) const
    {
        return xVal < E.xVal;
    }
};

const int M = 1005;

struct Polygon
{

    vector<point> vertices;
    vector<Edge> edges;
    vector<EdgeBucketEntry> edgeTable[M], activeEdgeTable[M];
    int yMin, yMax, n = -1;

    Polygon() {}

    Polygon(vector<point> corners)
    {
        vertices = corners;
        n = vertices.size();
    }

    void addToEdgeTable(int idx)
    {
        Edge E = edges[idx];

        point st = E.st, en = E.en;

        int pos = st.y, yVal = en.y, xVal = st.x;
        int dx = en.x - st.x, dy = en.y - st.y;

        float slope = (float)dx / dy;

        int flag = 0;

        if (idx + 1 < vertices.size() && edges[idx + 1].st.y == E.st.y)
            flag = 1;

        EdgeBucketEntry tuple;

        tuple.xVal = xVal;
        tuple.yVal = yVal;
        tuple.lambda = flag;
        tuple.slopeInverse = slope;

        edgeTable[pos].push_back(tuple);
        return;
    }

    void propagate(const EdgeBucketEntry tuplee, int index)
    {
        int startIndex = index;
        int endIndex = tuplee.yVal - 1;
        EdgeBucketEntry tuple = tuplee;

        for (int i = startIndex; i <= endIndex; i++)
        {
            activeEdgeTable[i].push_back(tuple);
            tuple.xVal += tuple.slopeInverse;
        }
    }

    void convertToActiveEdgeTable()
    {
        for (int i = yMin; i <= yMax; i++)
        {
            for (int j = 0; j < edgeTable[i].size(); j++)
            {
                EdgeBucketEntry tuple = edgeTable[i][j];
                propagate(tuple, i);
            }
        }
    }

    void generateEdgeTable()
    {
        for (int i = 0; i < M; i++)
            edgeTable[i].clear();

        for (int i = 0; i < n; i++)
        {
            point st = vertices[i];
            point en = vertices[(i + 1) % n];

            if (st.y > en.y)
                swap(st, en);

            yMax = max(yMax, en.y);
            yMin = min(yMin, st.y);

            if (st.y != en.y)
                edges.push_back(Edge(st, en));
        }

        sort(edges.begin(), edges.end());

        for (int i = 0; i < edges.size(); i++)
            addToEdgeTable(i);

        return;
    }

    void runScanLine(int Y)
    {

        if (activeEdgeTable[Y].size() == 0)
            return;
        sort(activeEdgeTable[Y].begin(), activeEdgeTable[Y].end());

        int parity = 0;

        for (int i = 0; i < activeEdgeTable[Y].size(); i += 2)
        {
            EdgeBucketEntry st = activeEdgeTable[Y][i];
            EdgeBucketEntry en = activeEdgeTable[Y][i + 1];

            int stX = (int)st.xVal;
            int enX = (int)en.xVal;

            if (en.lambda == 0)
                enX--; // removing xMax

            for (int j = stX; j <= enX; j++)
                drawPixel(j, Y);
        }
    }
    void fillPolygon()
    {
        if(n == -1) return;
        for (int i = yMin; i <= yMax; i += 1)
            runScanLine(i);
    }

} P[10];

int polyCnt = 0; // number of polygons

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POINTS);


    glColor3f(0.0, 1.0, 0.0);
    P[0].fillPolygon();

    glColor3f(1.0, 1.0, 0.0);
    P[1].fillPolygon();

    glColor3f(0.2, 0.2, 1.0);
    P[2].fillPolygon();

    glEnd();
    glFlush();
}

void input()
{

    freopen("polygon_filling.txt", "r", stdin);

    int n;
    vector<point> vec;
    while (cin >> n)
    {
        vec.clear();
        for (int i = 0; i < n; i++)
        {
            int x, y;
            cin >> x >> y;
            vec.push_back(point(x, y));
        }
        P[polyCnt++] = Polygon(vec);
    }
}

void solve()
{

    input();

    for (int i = 0; i < polyCnt; i++)
    {
        P[i].generateEdgeTable();
        P[i].convertToActiveEdgeTable();
    }
}

int main(int argc, char **argv)
{

    solve();

    glutInit(&argc, argv);                       // to initialize the toolkit;
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // sets the display mode
    glutInitWindowSize(Wi, He);                  // sets the window size
    glutInitWindowPosition(0, 0);                // sets the starting position for the window
    glutCreateWindow("Line Drawing");            // creates the window and sets the title
    myInit();                                    // additional initializations as necessary
    glutReshapeFunc(reshape);

    glutDisplayFunc(display);

    glutMainLoop(); // go into a loop until event occurs
    return 0;
}
