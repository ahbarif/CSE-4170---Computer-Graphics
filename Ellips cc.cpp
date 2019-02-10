void drawElipse(int a,int b)
{
    int d = 4*b*b*(-2*a+1) + 4*a*a;
    int x = a,y=0;
    drawPixel(x,y);
    while(b*b*(4*x-2) > 4*a*a*(y+1))
    {
        if(d<0){
            d += 4*a*a*(2*y+3);
            y++;
        }
        else{ //nw
            d += 4*b*b*(-2*x+2) + 4*a*a*(2*y+3);
            y++;
            x--;
        }
        draw4Way(x,y);
        
    }
    while(x>0)
    {
        
        if(d<0){ //nw
            d += 4*b*b*(-2*x+3) + 4*a*a*(2*y+2);
            y++;
            x--;
        }
        else{
            d += 4*b*b*(-2*x+3);
            x--;
        }
        draw4Way(x,y);
        
    }
}
