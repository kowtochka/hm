#include <stdlib.h>
#include "graphics.h"

#define COLOR(r,g,b) (r|(g<<8)|(b<<16)|15<<24)
int main()
{
    initgraph(720,720,"ya hochy spat"); 
    
    setcolor(DARKGRAY); 
      moveto(80,360);    
      lineto(640,360);    
      line(640,360,620,340);  
      line(640,360,620,380); 
    
    setcolor(WHITE);
    outtextxy(660,370,"X");
    
    setcolor(DARKGRAY);
      moveto(360,80);      
      lineto(360,640);     
      line(360,80,380,100);   
      line(360,80,340,100); 
    
    setcolor(WHITE);
      outtextxy(380,50,"Y");  
    
    setcolor(RED);
      moveto(360,360);     
    for (double x=0; x<6.5; x +=0.01){
        lineto(360+x*3, 360-(x * x * x));  
    }
    moveto(360,360); 
    for (double x=0; x>-6.5; x -=0.01){
        lineto(360+x*3, 360-(-x * x * x));  
    }

    getch();      
    closegraph();  
    return 0;
}
// gcc f1.c graphics.c $(pkg-config --cflags --libs cairo x11) -lm
