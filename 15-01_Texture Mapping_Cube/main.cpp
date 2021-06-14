#include <OpenGL/OpenGL.h>
#include <opencv2/opencv.hpp>
#include <GLUT/GLUT.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace cv;

#define _WINDOW_WIDTH 500
#define _WINDOW_HEIGHT 500

GLfloat camPos_x = 6, camPos_y =2, camPos_z = 5; //위치
GLfloat camAt_x = 0, camAt_y = 0, camAt_z = 0; //바라보는 곳
GLfloat camUp_x = 0, camUp_y = 1, camUp_z = 0; //방향

void light_global(){
    GLfloat light_global_ambient[] = {0.3,0.3,0.3, 1}; //전역 주변광 색깔 지정
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_global_ambient);//전역 주변광을 주겠다.
}

void light0_local(){
    GLfloat light_0_pos[] = {2.0,2.0,5.0,1.0}; //빛 위치 지정 공 바로 위에
    GLfloat light_0_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_0_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_0_ambient[] = {1.0 , 1.0, 1.0, 1.0};//백색광 = 태양
     
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_0_pos); //0번째 Light Position지정 개수는 GPU에 따라 다름
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_0_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_0_ambient);
}
void light_material(){
    glDisable(GL_COLOR_MATERIAL); //display할 때마다 기존의 색상을 무효화시키고 새로 그려라, 이렇게 안하면 reshape될때 색이 바뀜
    GLfloat material_0_specular[] = {1.0, 1.0, 1.0, 1.0};//빛 색깔과 그대로 맞춰주기
    GLfloat material_0_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat material_0_ambient[] = {0.3, 0.3, 0.3, 1.0};
    GLfloat material_0_shininess[] = {25.0};// 0~128
    //GLfloat material_0_emission[] = {0.0, 0.0, 1.0, 1.0};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_0_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_0_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_0_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_0_shininess);
    //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    //glEnable(GL_COLOR_MATERIAL);
}
void Init_light(){
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

void Init_texture(uchar* pImageArray, int width, int height){
    GLuint tex_id = 1;
    glGenTextures(1, &tex_id); //새로운 텍스쳐 만듦 id부여
    glBindTexture(GL_TEXTURE_2D, tex_id); //2d로 binding한다
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pImageArray); //이미지를 텍스쳐로 쓰겠다
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
}

void display_objs(){
    GLfloat v1[3] = {-1, -1, 1};
    GLfloat v2[3] = {-1, 1, 1};
    GLfloat v3[3] = {1, 1, 1};
    GLfloat v4[3] = {1, -1, 1};
    GLfloat v5[3] = {-1, -1, -1};
    GLfloat v6[3] = {-1, 1, -1};
    GLfloat v7[3] = {1, 1, -1};
    GLfloat v8[3] = {1, -1, -1};
    
    //v1 ~ v4 앞쪽면
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, 1);
    
        glTexCoord2f(0.0, 1.0); //Texture에서 0,0에 해당하는 지점을 코디네이트하겠다
        glVertex3f(v1[0], v1[1], v1[2]);
    
        glTexCoord2f(0.0, 0.0);
        glVertex3f(v2[0], v2[1], v2[2]);
    
        glTexCoord2f(1.0,0.0);
        glVertex3f(v3[0], v2[1], v3[2]);
    
        glTexCoord2f(1.0, 1.0);
        glVertex3f(v4[0], v4[1], v4[2]);
    glEnd();
    
    //v5 ~ v8 뒤쪽면
    glBegin(GL_POLYGON);
         glNormal3f(0, 0, -1);
     
         glTexCoord2f(0.0, 1.0); //Texture에서 0,0에 해당하는 지점을 코디네이트하겠다
         glVertex3f(v8[0], v8[1], v8[2]);
     
         glTexCoord2f(0.0, 0.0);
         glVertex3f(v7[0], v7[1], v7[2]);
     
         glTexCoord2f(1.0,0.0);
         glVertex3f(v6[0], v6[1], v6[2]);
     
         glTexCoord2f(1.0, 1.0);
         glVertex3f(v5[0], v5[1], v5[2]);
     glEnd();
    
    //v5 ~ v8 뒤쪽면
     glBegin(GL_POLYGON);
          glNormal3f(0, 0, -1);
      
          glTexCoord2f(0.0, 1.0); //Texture에서 0,0에 해당하는 지점을 코디네이트하겠다
          glVertex3f(v8[0], v8[1], v8[2]);
      
          glTexCoord2f(0.0, 0.0);
          glVertex3f(v7[0], v7[1], v7[2]);
      
          glTexCoord2f(1.0,0.0);
          glVertex3f(v6[0], v6[1], v6[2]);
      
          glTexCoord2f(1.0, 1.0);
          glVertex3f(v5[0], v5[1], v5[2]);
      glEnd();
}


void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    gluLookAt(camPos_x, camPos_y, camPos_z, camAt_x, camAt_y, camAt_z, camUp_x,camUp_y, camUp_z);
    
    light_global();
    light0_local();
    light_material();
    
    display_objs();
    glFlush();
}

void MyReshape(int width, int height){
    GLfloat left = 0, bottom = 0;
    glViewport(left, bottom, width-left, height-bottom);
    glMatrixMode(GL_PROJECTION);//투영변환 모드 초기화
    glLoadIdentity();
    GLfloat ratio = (float)(width-left)/(height-bottom); //비율 값
    gluPerspective(40, ratio, 0.1, 1000); //원근 투영
}
int main(int argc, char** argv) {
    Mat image = imread("/Users/jaewonna/Desktop/WorkspaceC++/Computer\ Graphics/Computer\ Graphics/15_Texture\ Mapping/bird.jpg", IMREAD_COLOR);
    
    cvtColor(image, image, COLOR_BGR2RGB);//opencv에서는 BGR로 읽어옴 opengl은 RGB를 처리함
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
    glutInitWindowPosition(300, 200);
    
    glutCreateWindow("Texure Mapping");
    
    glClearColor(0,0,0,0.0);
    
    Init_light();
    Init_texture(image.data, image.cols, image.rows);
    
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    
    glutMainLoop();
    return 0;
}
