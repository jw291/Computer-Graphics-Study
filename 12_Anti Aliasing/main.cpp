#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <iostream>

void MyReshape(int width, int height){
    GLfloat left = 0, bottom = 0;
    glViewport(left, bottom, width-left, height-bottom);
    glMatrixMode(GL_PROJECTION);//투영변환 모드 초기화
    glLoadIdentity();
    glOrtho(-2, 2, -2, 2, -2, 2);
}

void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //glutWireTeapot(0.5);
    
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(100);
    glLineWidth(10);
    glBegin(GL_POINTS);
        glVertex3f(0, 0, 0);
    glEnd();
    
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 2, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(2, 2, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(2, 4, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(4, 2, 0);
    glEnd();
    
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
