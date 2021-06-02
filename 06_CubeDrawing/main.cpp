#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>

#define _WINDOW_WIDTH 1200
#define _WINDOW_HEIGHT 1200

//GLfloat MyVertices[8][3] = {{-0.25,-0.25,0.25},{-0.25,0.25,0.25},{0.25,0.25,0.25},{0.25,-0.25,0.25},{-0.25,-0.25,-0.25},{-0.25,0.25,-0.25},{0.25,0.25,-0.25},{0.25,-0.25,-0.25}}; //3차원 공간 상의 각 정점, 너비가 0.5인 큐브
GLfloat defaultCube[8][3] = {{-0.5,-0.5,0.5},{-0.5,0.5,0.5},{0.5,0.5,0.5},{0.5,-0.5,0.5},{-0.5,-0.5,-0.5},{-0.5,0.5,-0.5},{0.5,0.5,-0.5},{0.5,-0.5,-0.5}}; //3차원 공간 상의 각 정점, 너비가 1인 큐브

GLfloat MyColor[8][3] = {{0.2,0.2,0.2},{1.0,0.0,0.0},{1.0,1.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,0.0,1.0},{1.0,1.0,1.0},{0.0,1.0,1.0}};
//각 정점마다 색깔

int list_id;

void drawCube(GLfloat size, GLfloat x, GLfloat y, GLfloat z){
    //defaultCube를 이용해서 새로운 큐브를 만듬
    GLfloat MyCube[8][3];
    for(int i = 0; i < 8; i++){
        MyCube[i][0] = size * defaultCube[i][0] + x; //크기가 늘어난 상태로. 이동
        MyCube[i][1] = size * defaultCube[i][1] + y; // 0은 x축 1은 y축 z는 z축
        MyCube[i][2] = size * defaultCube[i][2] + z;
    }
    
    int index[6][4] = {{0,3,2,1},{2,3,7,6},{7,4,5,6},{1,2,6,5},{0,3,7,4},{0,4,5,1}};
    
    for(int i = 0; i < 6; i++){
        int index0 = index[i][0];
        int index1 = index[i][1];
        int index2 = index[i][2];
        int index3 = index[i][3];
        glBegin(GL_POLYGON);
            glColor3fv(MyColor[index0]); glVertex3fv(MyCube[index0]);
            glColor3fv(MyColor[index1]); glVertex3fv(MyCube[index1]);
            glColor3fv(MyColor[index2]); glVertex3fv(MyCube[index2]);
            glColor3fv(MyColor[index3]); glVertex3fv(MyCube[index3]);
        glEnd();
    }
}

void createList(){
    list_id = glGenLists(1);
    
    glNewList(list_id, GL_COMPILE);
        drawCube(0.1, 0, 0,0);
        drawCube(0.1, 0.8, 0, 0);
        drawCube(0.1, 0 , 0.8, 0);
        drawCube(0.1, 0 , 0, 0.8);
        drawCube(0.1, 0.8, 0.8, 0.8);
    glEndList();
}

void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT);
    glCallList(list_id);
    glFlush();
}

void MyReshape(int width, int height){
    glViewport(0, 0, width, height);
    GLfloat f_w = (GLfloat)width / (GLfloat)_WINDOW_WIDTH;
    GLfloat f_h = (GLfloat)height / (GLfloat)_WINDOW_HEIGHT;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0*f_w, 1.0*f_w, -1.0*f_h, 1.0*f_h, -2, 2);
    gluLookAt(0.5, 0.5, 1, 0, 0, 0, 0, 1, 0);
}

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB); //이중 버퍼링을 위해 이중버퍼 사용
    glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
    glutCreateWindow("Cube Drawing");
    
    //콜백함수
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    
    //디스플레이 리스트
    createList();
    glutMainLoop();
}
