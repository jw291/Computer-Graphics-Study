//4각형 그리기 예제
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>

void display (void) //화면에 Display할 내용을 담고 있는 메서드
{
    glClear(GL_COLOR_BUFFER_BIT); //화면 Clear
    glBegin(GL_POLYGON); //그래픽 Object를 선언
    glVertex3f(-0.5, -0.5, -0.5);//화면에 점을 찍음
    glVertex3f(0.5, -0.5, 0.5);//화면에 점을 찍음
    glVertex3f(0.5, 0.5, 0.0);//화면에 점을 찍음
    glVertex3f(-0.5, 0.5, 0.0); //화면에 점을 찍음
    glEnd();//그래픽 Object 내용 마무리
    glFlush();//버퍼에 있는 내용을 그리도록 함
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutCreateWindow("Test");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}


