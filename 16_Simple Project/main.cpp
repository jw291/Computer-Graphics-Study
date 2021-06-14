#include <OpenGL/OpenGL.h>
#include <opencv2/opencv.hpp>
#include <GLUT/GLUT.h>
#include "ObjLoad.hpp"

using namespace cv;

#define _WINDOW_WIDTH 1000
#define _WINDOW_HEIGHT 1000

GLfloat camPos_x = 0, camPos_y = 0, camPos_z = 10; //위치
GLfloat camAt_x = 0, camAt_y = 0, camAt_z = 0; //바라보는 곳
GLfloat camUp_x = 0, camUp_y = 1, camUp_z = 0; //방향

CModel m;
Mat image[2];
GLuint tex_ids[2] = {1,2};
int nTex = 2;

GLfloat bullet_pos = camPos_z - 2;
bool bullet_fire = false;
bool bHit;

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

void Init_texture(){
    glGenTextures(nTex, tex_ids); //새로운 텍스쳐 만듦 id부여
    glEnable(GL_TEXTURE_2D);
}

void MyReshape(int width, int height){
    GLfloat left = 0, bottom = 0;
    glViewport(left, bottom, width-left, height-bottom);
    glMatrixMode(GL_PROJECTION);//투영변환 모드 초기화
    glLoadIdentity();
    GLfloat ratio = (float)(width-left)/(height-bottom); //비율 값
    gluPerspective(40, ratio, 0.1, 1000); //원근 투영
}

void display_obj(){
    GLfloat x,y,z,nx,ny,nz,tx,ty;
    int v_id, vt_id, vn_id;
    int idx;
    for(int o = 0; o <m.objs.size(); o++){
        if( o == 0){
            idx = 0;
        }else {
            idx = 1;
        }
        glBindTexture(GL_TEXTURE_2D, tex_ids[idx]); //2d로 binding한다
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[idx].cols, image[idx].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[idx].data); //이미지를 텍스쳐로 쓰겠다
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        
        int nFaces = m.objs[o].f.size();
        
        for(int k = 0; k < nFaces; k++){
            int nPoints = m.objs[o].f[k].v_pairs.size();
            glBegin(GL_POLYGON);
            for(int i = 0; i < nPoints; i++){
                v_id = m.objs[o].f[k].v_pairs[i].d[0];//0번째 오브젝트에 k번째면에 i번째 pairs의 0번째 id가 vetex
                vt_id = m.objs[o].f[k].v_pairs[i].d[1];// texture id
                vn_id = m.objs[o].f[k].v_pairs[i].d[2];// normal vector id
               
                x = m.objs[o].v[v_id - 1].d[0];//id로부터 좌표 알아내기
                y = m.objs[o].v[v_id - 1].d[1];
                z = m.objs[o].v[v_id - 1].d[2];
                
                nx = m.objs[o].vn[vn_id - 1].d[0];//id로부터 법선벡터 알아내기
                ny = m.objs[o].vn[vn_id - 1].d[1];
                nz = m.objs[o].vn[vn_id - 1].d[2];
                
                tx = m.objs[o].vt[vt_id - 1].d[0];//id로부터 법선벡터 알아내기
                ty = m.objs[o].vt[vt_id - 1].d[1];
                
                glNormal3f(nx, ny, nz); //법선벡터 지정
                glTexCoord2f(tx, ty);
                glVertex3f(x, y, z); //점찍기
            }
            glEnd();
        }
    }
    glFlush();
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y){
    switch (KeyPressed) {
        case 'a':
            bullet_fire = true;
            bullet_pos = camPos_z - 2;
            break;
    }
    glutPostRedisplay();//MainLoop에 있는 MyDisplay를 다시한번 호출하라.
}

void MyTimer(int value){
    glutTimerFunc(20, MyTimer, 1);
    if(bullet_fire == true){
        bullet_pos -= 0.05;
        if(bullet_pos < 0.1){
            bHit = true;
            bullet_fire = false;
        }
    }
    glutPostRedisplay();
}

void draw_bullet(){
    if(bullet_fire == true){
        glPushMatrix();
        glTranslatef(0,0, bullet_pos);
        glutSolidSphere(0.2, 20, 20);
        glPopMatrix();
    }
}


void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    gluLookAt(camPos_x, camPos_y, camPos_z, camAt_x, camAt_y, camAt_z, camUp_x,camUp_y, camUp_z);
    
    light_global();
    light0_local();
    light_material();
    
    if(bHit == false){
        display_obj();
    }
    draw_bullet();
    glutSwapBuffers();
}


int main(int argc, char** argv) {
    //텍스쳐 이미지 읽어오기
    image[0] = imread("/Users/jaewonna/Desktop/WorkspaceC++/Computer\ Graphics/Computer\ Graphics/15_Texture\ Mapping/bird.jpg", IMREAD_COLOR);
    
    cvtColor(image[0], image[0], COLOR_BGR2RGB);//opencv에서는 BGR로 읽어옴 opengl은 RGB를 처리함
    
    
    //모델 읽어오기
    string filepath = "/Users/jaewonna/Desktop/WorkspaceC++/Computer\ Graphics/Computer\ Graphics/14_ObjectLoad/monkey.obj";
    ifstream fin(filepath);
    
    
    m.loadObj(fin);
    
    fin.close();
    
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
    glutInitWindowPosition(300, 200);
    
    glutCreateWindow("Texure Mapping");
    
    glClearColor(0,0,0,0.0);
    
    Init_light();
    Init_texture();
    
    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
    glutTimerFunc(20, MyTimer, 1);
    glutReshapeFunc(MyReshape);
    
    glutMainLoop();
    return 0;
}

