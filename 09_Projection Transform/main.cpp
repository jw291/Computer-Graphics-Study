#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <math.h>
#include <iostream>

#define _WINDOW_WIDTH 500
#define _WINDOW_HEIGHT 500

int angle_upper = 0;
int angle_lower = 0;

int dir_upper = 1;
int dir_lower = 1;

GLfloat camPos_x = 0.5, camPos_y = 0.5, camPos_z = 4; //위치
GLfloat camAt_x = 0, camAt_y = 0, camAt_z = 0; //바라보는 곳
GLfloat camUp_x = 0, camUp_y = 1, camUp_z = 0; //방향

void MyReshape(int width, int height){
    GLfloat left = 250, bottom = 300;
    glViewport(left, bottom, width-left, height-bottom);
    //GLfloat f_w = (GLfloat)width / (GLfloat)_WINDOW_WIDTH;
    //glMatrixMode(GL_PROJECTION);
    glMatrixMode(GL_PROJECTION);//투영변환 모드 초기화
    glLoadIdentity();
    //glOrtho(-1.0*f_w, 1.0*f_w, -1.0*f_h, 1.0*f_h, -2, 2);
    GLfloat ratio = (float)(width-left)/(height-bottom); //비율 값
    gluPerspective(40, ratio, 0.1, 10); //원근 투영
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
    
    gluLookAt(camPos_x, camPos_y, camPos_z, camAt_x, camAt_y, camAt_z, camUp_x,camUp_y, camUp_z);
    
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

void CameraAnimationTimer(int value){
    GLfloat Theta = 0.01;
    GLfloat tmp_cam_x = camPos_x;
    //모델의 y축 기준으로 카메라 회전시키기 회전공식 참고
    camPos_x = camPos_x * cos(Theta) + camPos_z * sin(Theta);
    camPos_z = tmp_cam_x * -sin(Theta) + camPos_z * cos(Theta);
    
    
    glutTimerFunc(20, CameraAnimationTimer, 2);
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
    glutTimerFunc(20, CameraAnimationTimer, 2);
    
    glutMainLoop();
}


