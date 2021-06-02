#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>

GLfloat Delta = 0.0;

void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.5, 0.8);
    glVertex3f(-1.0 + Delta, -0.5, 0.0);
    glVertex3f(0.0 + Delta, -0.5, 0.0);
    glVertex3f(0.0 + Delta, 0.5, 0.0);
    glVertex3f(-1.0 + Delta, 0.5, 0.0);
    glEnd();
    
    glutSwapBuffers(); //이중 버퍼의 데이터를 스왑해주는 버퍼
}

void MyTimer(int value){
    Delta = Delta + 0.005; //약간씩 증가시켜서 움직이게 함
    glutPostRedisplay();//새로그림
    glutTimerFunc(10, MyTimer, value); //20ms 후에 다시 타이머를 호출함.
}

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //이중 버퍼링을 위해 이중버퍼 사용
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Animation & Double Buffering");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0);
    
    //콜백함수
    glutDisplayFunc(MyDisplay);
    glutTimerFunc(10, MyTimer, 1); // 20ms마다 MyTimer를 호출하라 1은 식별자(여러 타이머에 대한)
    
    glutMainLoop();
}
