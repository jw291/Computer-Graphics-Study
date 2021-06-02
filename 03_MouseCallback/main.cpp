#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>

#define _WINDOW_WIDTH 300
#define _WINDOW_HEIGHT 300

GLfloat r = 1.0 ,g = 1.0 ,b = 1.0;
GLint bef_x = -1 , bef_y = - 1; //마우스 커서 초기값(이전값)
int mode = 0;

void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT);
    if(mode == 0){
        glutWireSphere(0.3, 15, 15);
    }else if(mode == 1){
        glutWireTeapot(0.3);
    }else if(mode == 2){
        
    }
    
    glFlush();
}
void MyReshape(int width, int height){
    glViewport(0, 0, width, height);
    GLfloat f_w = (GLfloat)width / (GLfloat)_WINDOW_WIDTH;
    GLfloat f_h = (GLfloat)height / (GLfloat)_WINDOW_HEIGHT;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0*f_w, 1.0*f_w, -1.0*f_h, 1.0*f_h, -1, 1);
}

void mouseBtn(int btn, int state, int x, int y){
    if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP){//왼쪽 마우스를 땔 때
        mode = (mode+1)%2;
        glutPostRedisplay();
    }
}

void mouseDrag(int cur_x, int cur_y){
    GLint dx, dy; //초기값과 바뀐값 차이
    if(bef_x >= 0 || bef_y >= 0){
        dx = abs(cur_x - bef_x);
        dy = abs(cur_y - bef_y);
        
        r = (r - 0.1)<0?0:r-0.1;
        g = (g - 0.1)<0?0:g-0.1;
        b = (b - 0.1)<0?0:b-0.1;
        
        glColor3f(r, g, b);
        
        glutPostRedisplay();
    }
    
    bef_x = cur_x;
    bef_y = cur_y;
}

void mouseWindowEntry(int state){
    if(state == GLUT_LEFT){
        mode = 2;
    }else if(state == GLUT_ENTERED){
        mode = 1;
    }
    glutPostRedisplay();
}

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("title");
    glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
    
    //콜백
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    
    //마우스 콜백
    glutMouseFunc(mouseBtn);
    glutMotionFunc(mouseDrag);
    glutEntryFunc(mouseWindowEntry);
    
    glutMainLoop();
}
