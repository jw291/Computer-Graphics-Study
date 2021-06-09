#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>

#define _WINDOW_WIDTH 500
#define _WINDOW_HEIGHT 500

GLfloat camPos_x = 0, camPos_y = 1, camPos_z = -2; //위치
GLfloat camAt_x = 0, camAt_y = 0, camAt_z = 0; //바라보는 곳
GLfloat camUp_x = 0, camUp_y = 1, camUp_z = 0; //방향

GLfloat Light0_pos_x = 2.0, Light0_pos_y = 2.0, Light0_pos_z = -2.0;
GLfloat bNonDirectionalLight = 1.0;
void MyReshape(int width, int height){
    GLfloat left = 0, bottom = 0;
    glViewport(left, bottom, width-left, height-bottom);
    glMatrixMode(GL_PROJECTION);//투영변환 모드 초기화
    glLoadIdentity();
    GLfloat ratio = (float)(width-left)/(height-bottom); //비율 값
    gluPerspective(40, ratio, 0.1, 1000); //원근 투영
}

void Init_light(){
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
     
    //전역광
    GLfloat light_global_ambient[] = {0.3,0.3,0.3, 1}; //전역 주변광 색깔 지정
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_global_ambient);
    
    //조명 설정 & 스포트라이트
    glEnable(GL_LIGHT0);
    GLfloat light_0_pos[] = {Light0_pos_x, Light0_pos_y, Light0_pos_z, bNonDirectionalLight}; //빛 위치 지정 공 바로 위에
    GLfloat light_0_dir[] = {-1, -1, 1};
    GLfloat light_0_SpotAngle[] = {3};
    GLfloat light_0_SpotExp1[] = {10.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_0_pos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_0_dir);
    glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, light_0_SpotAngle);
    glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, light_0_SpotExp1);
    
    //주변광, 확산광, 경면광
    GLfloat light_0_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_0_diffuse[] = {0, 0, 1.0, 1.0};
    GLfloat light_0_ambient[] = {1.0 , 1.0, 1.0, 1.0};//백색광 = 태양
    
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_0_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_0_ambient);
    
    //물체 반사광
    glDisable(GL_COLOR_MATERIAL); //display할 때마다 기존의 색상을 무효화시키고 새로 그려라, 이렇게 안하면 reshape될때 색이 바뀜
    GLfloat material_0_specular[] = {1.0, 1.0, 1.0, 1.0};//빛 색깔과 그대로 맞춰주기
    GLfloat material_0_diffuse[] = {0.8,0,0.3,1.0};
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
