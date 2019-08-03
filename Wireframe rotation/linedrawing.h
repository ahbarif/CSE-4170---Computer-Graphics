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

