#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>

#define _WINDOW_WIDTH 500
#define _WINDOW_HEIGHT 500

GLfloat camPos_x = 0, camPos_y = 5, camPos_z = -7; //위치
GLfloat camAt_x = 0, camAt_y = 0, camAt_z = 0; //바라보는 곳
GLfloat camUp_x = 0, camUp_y = 1, camUp_z = 0; //방향

void MyReshape(int width, int height){
    GLfloat left = 0, bottom = 0;
    glViewport(left, bottom, width-left, height-bottom);
    glMatrixMode(GL_PROJECTION);//투영변환 모드 초기화
    glLoadIdentity();
    GLfloat ratio = (float)(width-left)/(height-bottom); //비율 값
    gluPerspective(40, ratio, 0.1, 1000); //원근 투영
}

void init_light(){
    glEnable(GL_LIGHTING); //빛을 쓰겠다
    glShadeModel(GL_SMOOTH); //구로 셰이딩 쓰겠다 퐁 셰이딩은 GL에 없음
    glEnable(GL_DEPTH_TEST);
    
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPos_x, camPos_y, camPos_z, camAt_x, camAt_y, camAt_z, camUp_x,camUp_y, camUp_z);
    
    glDisable(GL_COLOR_MATERIAL); //display할 때마다 기존의 색상을 무효화시키고 새로 그려라, 이렇게 안하면 reshape될때 색이 바뀜
    GLfloat material_0_specular[] = {1.0, 1.0, 1.0, 1.0};//빛 색깔과 그대로 맞춰주기
    GLfloat material_0_diffuse[] = {0.8,0,0,1.0};
    GLfloat material_0_ambient[] = {0.3,0.3,0.3,1.0};
    GLfloat material_0_shininess[] = {25.0};// 0~128
    //GLfloat material_0_emission[] = {0.0, 0.0, 1.0, 1.0};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_0_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_0_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_0_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_0_shininess);
    //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    
    glutSolidTeapot(0.3);
    
    glEnable(GL_COLOR_MATERIAL);
    glFlush();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
    glutInitWindowPosition(300, 200);
    
    glutCreateWindow("Lighting & Shading");
    
    glClearColor(0,0,0,0.0);
    Init_light();
    
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    
    glutMainLoop();
    return 0;
}
