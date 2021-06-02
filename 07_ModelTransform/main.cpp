#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>

#define _WINDOW_WIDTH 800
#define _WINDOW_HEIGHT 800

int angle_upper = 0;
int angle_lower = 0;

int dir_upper = 1;
int dir_lower = 1;

void MyReshape(int width, int height){
    glViewport(0, 0, width, height);
    GLfloat f_w = (GLfloat)width / (GLfloat)_WINDOW_WIDTH;
    GLfloat f_h = (GLfloat)height / (GLfloat)_WINDOW_HEIGHT;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0*f_w, 1.0*f_w, -1.0*f_h, 1.0*f_h, -2, 2);
    gluLookAt(0.5, 0.5, 1, 0, 0, 0, 0, 1, 0);
}

void drawXAxis(){
    glBegin(GL_LINES);
        //선
        glVertex3f(0, 0, 0);
        glVertex3f(0.3, 0, 0);
        //선머리
        glVertex3f(0.3, 0, 0);
        glVertex3f(0.21, 0.09, 0);
        //선머리
        glVertex3f(0.3, 0, 0);
        glVertex3f(0.21, -0.09, 0);
    
    glEnd();
}

void drawAxis(){
    glColor3f(1, 1, 1);
   // glMatrixMode(GL_MODELVIEW);
   // glLoadIdentity(); // 초기화

    drawXAxis();
    glPushMatrix();//행렬 스택에 push
        glRotatef(90, 0, 0, 1);
        drawXAxis();
    glPopMatrix(); //모델 좌표계의 행렬스택중 Top Data삭제
    
    glPushMatrix();
        glRotatef(-90, 0, 1, 0);
        drawXAxis();
    glPopMatrix();
}

void drawCuboid(GLfloat sx, GLfloat sy, GLfloat sz){
    glPushMatrix();
        glScalef(sx, sy, sz);
        glutWireCube(1);
    glPopMatrix();
}

void drawBody(){
    drawAxis();
    drawCuboid(0.5,1,0.2);
}

void drawHead(){
    glPushMatrix();
    glTranslatef(0, 0.55, 0);
    drawAxis();
    drawCuboid(0.3, 0.1, 0.2);
    glPopMatrix();
}

void drawUpperArm(GLfloat Angle){
    glTranslatef(0.25,0.3, 0);
    glRotatef(Angle, 0, 0, 1);
    glTranslatef(0.25,0, 0);
    drawCuboid(0.5, 0.2, 0.2);
}

void drawLowerArm(GLfloat Angle){
    drawAxis();//Axis확인하면서 평행이동하면 편함
    glTranslatef(0.25,0, 0);
    glRotatef(Angle, 0, 0, 1);
    glTranslatef(0.25,0, 0);
    drawAxis();
    drawCuboid(0.5, 0.2, 0.2);
}

void drawHand(){
    glTranslatef(0.35, 0, 0);
    glutWireSphere(0.1, 15, 15);
}

void drawFinger1(){
    glPushMatrix();
        glTranslatef(0.15, 0, 0);
        drawCuboid(0.1, 0.05, 0.05);
    glPopMatrix();
}
void drawFinger2(){
    glPushMatrix();
        glRotatef(30, 0, 0, 1);
        glTranslatef(0.15, 0, 0);
        drawCuboid(0.1, 0.05, 0.05);
    glPopMatrix();
}

void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT);
    //drawAxis();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    drawBody();
    drawHead();
    drawUpperArm(angle_upper);//timer에 따라 angle 변함
    drawLowerArm(angle_lower);//timer에 따라 angle 변함
    drawHand();
    drawFinger1();
    drawFinger2();
    //glFlush();
    glutSwapBuffers(); //애니메이션 사용을 위해서 Flush없애고 스왑버퍼 호출
}

void MyTimer(int value){
    angle_upper += dir_upper; //1도씩 움직이기
    angle_lower += dir_lower; //5도씩 움직이기
    
    if(angle_upper <= 0)
        dir_upper = 1;
    else if(angle_upper >= 60)
        dir_upper = 0;
    
    //std::cout << angle_upper << std::endl;
    if(angle_lower <= -70)
        dir_lower = 5;
    else if(angle_lower >= 100)
        dir_lower = -5;
        
    glutTimerFunc(20, MyTimer, 1);
    glutPostRedisplay();
}

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
    glutCreateWindow("title");
    
    //콜백
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    
    glutTimerFunc(20, MyTimer, 1);
    
    glutMainLoop();
}


