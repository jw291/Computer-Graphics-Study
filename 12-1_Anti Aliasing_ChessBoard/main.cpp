#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <iostream>

#define _WINDOW_WIDTH 500
#define _WINDOW_HEIGHT 500

GLfloat camPos_x = 0, camPos_y = 5, camPos_z = -4; //위치
GLfloat camAt_x = 0, camAt_y = 0, camAt_z = 10; //바라보는 곳
GLfloat camUp_x = 0, camUp_y = 1, camUp_z = 0; //방향

void MyReshape(int width, int height){
    GLfloat left = 0, bottom = 0;
    glViewport(left, bottom, width-left, height-bottom);
    glMatrixMode(GL_PROJECTION);//투영변환 모드 초기화
    glLoadIdentity();
    GLfloat ratio = (float)(width-left)/(height-bottom); //비율 값
    gluPerspective(40, ratio, 0.1, 1000); //원근 투영
}

void drawRect(GLfloat x,GLfloat y,GLfloat z,GLfloat width,GLfloat r,GLfloat g,GLfloat b){
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
        glVertex3f(x,y,z);
        glVertex3f(x+width,y,z);
        glVertex3f(x+width,y,z+width);
        glVertex3f(x,y,z+width);
    glEnd();
}

void drawBlackWhiteRect(GLint nx, GLint nz, GLfloat sx, GLfloat sy, GLfloat sz,GLfloat w, int bBlack){
    GLfloat x, y, z;
    x = sx + nx * w;
    y = sy;
    z = sz + nz * w;
    
    if(bBlack == 1)
        drawRect(x, y, z, w, 0, 0, 0);
    else
        drawRect(x, y, z, w, 1, 1, 1);
}

void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(camPos_x, camPos_y, camPos_z, camAt_x, camAt_y, camAt_z, camUp_x,camUp_y, camUp_z);
    
    glEnable(GL_BLEND);
    
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    for(int i =0; i < 1000; i++){
        for(int j =0; j < 1000; j++){
            int bBlack = (i+j)%2;
            drawBlackWhiteRect(i, j, -10, 0, 0, 0.2, bBlack);
        }
    }
    
    glDisable(GL_BLEND);
    
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(50, 50);
    
    glutCreateWindow("anti aliasing");
    
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    
    glutMainLoop();
    return 0;
}

