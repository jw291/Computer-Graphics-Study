#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>

#define _WINDOW_WIDTH 500
#define _WINDOW_HEIGHT 500

GLfloat camPos_x = 0, camPos_y = 5, camPos_z = -7; //위치
GLfloat camAt_x = 0, camAt_y = 0, camAt_z = 0; //바라보는 곳
GLfloat camUp_x = 0, camUp_y = 1, camUp_z = 0; //방향

GLfloat Light0_pos_x = 5.0, Light0_pos_y = 5.0, Light0_pos_z = 0.0;

void MaterialLightInit(GLfloat am_R, GLfloat am_G, GLfloat am_B, GLfloat am_A , GLfloat di_R, GLfloat di_G, GLfloat di_B, GLfloat di_A){
    
    glDisable(GL_COLOR_MATERIAL); //display할 때마다 기존의 색상을 무효화시키고 새로 그려라, 이렇게 안하면 reshape될때 색이 바뀜
    GLfloat material_0_specular[] = {1.0, 1.0, 1.0, 1.0};//빛 색깔과 그대로 맞춰주기
    GLfloat material_0_diffuse[] = {di_R, di_G, di_B, di_A};
    GLfloat material_0_ambient[] = {am_R, am_G, am_B, am_A};
    GLfloat material_0_shininess[] = {25.0};// 0~128
    //GLfloat material_0_emission[] = {0.0, 0.0, 1.0, 1.0};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_0_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_0_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_0_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_0_shininess);
    //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}
void drawFloor(GLfloat y){
    GLfloat x = -2.5, z = -2.5, w = 5;
    
    MaterialLightInit(0, 0, 1.0, 1.0, 0, 0, 0.8, 1.0);
    glBegin(GL_POLYGON);
         glNormal3f(-2, 1, 0);
        //법선벡터는 따로 해줘야 쉐이딩이 적용된다.
        //법선 벡터의 방향에 따라서 평면이 볼록&오목(곡면 설정)하다라고 인식함 이를 이용해서 적절하게 음영을 주는거임
        //하지만 이렇게되면 법선벡터의 크기가 1이 아니기 때문에 Normalize해줘야함.
         glVertex3f(x,y,z);
         glVertex3f(x,y,z+w);
    
         glNormal3f(2, 1, 0);
         glVertex3f(x+w,y,z+w);
         glVertex3f(x+w,y,z);
    
        //이렇게 설정하면 평면이 오목하게돼서 왼쪽 두 점은 더 어두워지고
        //오른쪽 두 점은 약간 더 어두워짐
         
       
         
    glEnd();
}

void drawWall(GLfloat z){
    GLfloat x = -2.5, y = -2.5, w = 5;
    //glColor3f(0.2, 0.6, 0.2);
    MaterialLightInit(1.0, 1.0, 0, 1.0, 1.0, 1.0, 0, 1.0);
    glNormal3f(0, 0, -1); //법선벡터는 따로 해줘야 쉐이딩이 적용된다.
    glBegin(GL_POLYGON);
         glVertex3f(x,y,z);
         glVertex3f(x,y+w,z);
         glVertex3f(x+w,y+w,z);
         glVertex3f(x+w,y,z);
    glEnd();
}

void MyReshape(int width, int height){
    GLfloat left = 0, bottom = 0;
    glViewport(left, bottom, width-left, height-bottom);
    glMatrixMode(GL_PROJECTION);//투영변환 모드 초기화
    glLoadIdentity();
    GLfloat ratio = (float)(width-left)/(height-bottom); //비율 값
    gluPerspective(40, ratio, 0.1, 1000); //원근 투영
}
void drawSphere(){
    MaterialLightInit(1.0, 0, 0, 1.0, 0.8, 0, 0, 1.0);
    
    glutSolidSphere(0.3, 20, 20);
}
void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPos_x, camPos_y, camPos_z, camAt_x, camAt_y, camAt_z, camUp_x,camUp_y, camUp_z);
    
    drawFloor(-2.5);
    drawWall(2.5);
    drawSphere();
    //glColor3f(0.8, 0.2, 0.0);
    
    glEnable(GL_COLOR_MATERIAL);
    glFlush();
}

void Init_light(){
    
    GLfloat bNonDirectionalLight = 1.0; //DirectionalLight가 아니다.
    glEnable(GL_LIGHTING); //빛을 쓰겠다
    glShadeModel(GL_SMOOTH); //구로 셰이딩 쓰겠다 퐁 셰이딩은 GL에 없음
    glEnable(GL_DEPTH_TEST);
    
    
    GLfloat light_global_ambient[] = {0.3,0.3,0.3, 1}; //전역 주변광 색깔 지정
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_global_ambient);//전역 주변광을 주겠다.
    
    GLfloat light_0_pos[] = {Light0_pos_x, Light0_pos_y, Light0_pos_z, bNonDirectionalLight}; //빛 위치 지정 공 바로 위에
    GLfloat light_0_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_0_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_0_ambient[] = {1.0 , 1.0, 1.0, 1.0};//백색광 = 태양
    
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_0_pos); //0번째 Light Position지정 개수는 GPU에 따라 다름
   // glLightfv(GL_LIGHT0, GL_SPECULAR, light_0_specular);
   // glLightfv(GL_LIGHT0, GL_DIFFUSE, light_0_diffuse);
   // glLightfv(GL_LIGHT0, GL_AMBIENT, light_0_ambient);
    
    glEnable(GL_NORMALIZE);//법선벡터 정규화 크기를 1로 바꾸기
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

