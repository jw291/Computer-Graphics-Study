//
//  main.cpp
//  10_Visibility Detection
//
//  Created by jaewon na on 2021/06/06.
//  Copyright © 2021 jaewon na. All rights reserved.
//
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <math.h>
#include <iostream>

#define _WINDOW_WIDTH 700
#define _WINDOW_HEIGHT 700

GLfloat camPos_x = 0.5, camPos_y = 1, camPos_z = 1; //위치
GLfloat camAt_x = 0, camAt_y = 0, camAt_z = 0; //바라보는 곳
GLfloat camUp_x = 0, camUp_y = 1, camUp_z = 0; //방향

void NumberOfClipPlane(){//절단면 개수
    int num;
    glGetIntegerv(GL_MAX_CLIP_PLANES, &num);
    std::cout << num <<std::endl;
    //결과 : 6개
}

void InitLight(){
    GLfloat mat_diffuse[] = {0.5, 0.4, 0.3, 1.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_ambient[] = {0.5, 0.4, 0.3, 1.0};
    GLfloat mat_shininess[] = {50.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat light_ambient[] = {0.3, 0.3, 0.3, 1.0};
    GLfloat light_position[] = {-3, 2, 3.0, 0.0};
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void InitVisibility(){
    //glEnable(GL_CULL_FACE); //평면
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    
    glFrontFace(GL_CW);
    //정점의 순서는 앞면에서 바라보는 기준으로 반시계 방향이다
    //함수(glFrontFace)를 사용하여 정점 순서 기준을 변경할 수 있다.(반시계 -> 시계)
    //모델링 된 오브젝트가 어떤 방향을 앞면으로 잡았는지에 따라 다름
    //glCullFace(GL_BACK);
    //뒤쪽 면은 그리지 않겠다.
    glEnable(GL_DEPTH_TEST); //테스트 활성화
    
    glEnable(GL_CLIP_PLANE0);//클립 평면 중 0번 enable
}

void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPos_x, camPos_y, camPos_z, camAt_x, camAt_y, camAt_z, camUp_x,camUp_y, camUp_z);
    
    //InitVisiblity에서 clip enable해주었지만
    //초기화한 다음에 잘라야해서 Display로 가져옴
    GLdouble eq[4] = {0,0,1,0}; // y = 0인 평면 위쪽만 살리고 아래를 자름
    glClipPlane(GL_CLIP_PLANE0, eq);
    
    glutSolidTeapot(0.2);
    glutSwapBuffers();
}

void MyReshape(int width, int height){
    GLfloat left = 0, bottom = 0;
    glViewport(left, bottom, width-left, height-bottom);
    glMatrixMode(GL_PROJECTION);//투영변환 모드 초기화
    glLoadIdentity();
    GLfloat ratio = (float)(width-left)/(height-bottom); //비율 값
    gluPerspective(40, ratio, 0.1, 10); //원근 투영
}

void MyTimer(int value){
    GLfloat Theta = 0.01;
    GLfloat tmp_cam_x = camPos_x;
    //모델의 y축 기준으로 카메라 회전시키기 회전공식 참고
    camPos_x = camPos_x * cos(Theta) + camPos_z * sin(Theta);
    camPos_z = tmp_cam_x * -sin(Theta) + camPos_z * cos(Theta);
    
    glutTimerFunc(20, MyTimer, 1);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Visibility Detection");
    glClearColor(0,0,0,0.0);
    
    InitLight();
    InitVisibility();
    
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutTimerFunc(20, MyTimer, 1);
    
    NumberOfClipPlane(); //쓸 수 있는 절단면 6개
    
    glutMainLoop();
    
    
}
