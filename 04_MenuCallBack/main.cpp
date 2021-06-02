#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>

#define _WINDOW_WIDTH 300
#define _WINDOW_HEIGHT 300

GLfloat r = 1.0 ,g = 1.0 ,b = 1.0;
GLint bef_x = -1 , bef_y = - 1; //마우스 커서 초기값(이전값)
int mode = 0;

void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT);
    if(mode == 0){
        //..do nothing
    }else if(mode == 1){
        glutWireSphere(0.3, 15, 15);
    }else if(mode == 2){
        glutWireTorus(0.2, 0.5, 15, 15);
    }else
        glutWireTeapot(0.3);
    
    glFlush();
}
void MyReshape(int width, int height){
    glViewport(0, 0, width, height);
    GLfloat f_w = (GLfloat)width / (GLfloat)_WINDOW_WIDTH;
    GLfloat f_h = (GLfloat)height / (GLfloat)_WINDOW_HEIGHT;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0*f_w, 1.0*f_w, -1.0*f_h, 1.0*f_h, -1, 1);
}

void mouseDrag(int cur_x, int cur_y){
    GLint dx, dy; //초기값과 바뀐값 차이
    if(bef_x >= 0 || bef_y >= 0){
        dx = abs(cur_x - bef_x);
        dy = abs(cur_y - bef_y);
        
        r = (r - 0.1)<0?0:r-0.1;
        g = (g - 0.1)<0?0:g-0.1;
        b = (b - 0.1)<0?0:b-0.1;
        
        glColor3f(r, g, b);
        
        glutPostRedisplay();
    }
    
    bef_x = cur_x;
    bef_y = cur_y;
}
void SelectMenu(int value){
    //glutAddMenuEntry에서 지정해준 value값이 이 함수 매개변수로 들어옴
    if(value == 2){
        exit(0);
    }
}

void SelectSubMenu(int value){
    mode = value;
    glutPostRedisplay();
}
int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("title");
    glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
    
    //콜백
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    
    //마우스 콜백
    glutMotionFunc(mouseDrag);
    
    //서브메뉴 콜백
    int subMenu_id = glutCreateMenu(SelectSubMenu);
    glutSetMenu(subMenu_id);//현재 메뉴를 지정해줌
    glutAddMenuEntry("Sphere", 1); //엔트리를 만들고, 지정된 현재메뉴의 콜백 함수의 인자로 value전달
    glutAddMenuEntry("Torus", 2);
    glutAddMenuEntry("Teaport", 3);
    
    //메인메뉴 콜백
    int mainMenu_id = glutCreateMenu(SelectMenu); //메인 메뉴를 만들어줌
    glutSetMenu(mainMenu_id);//현재 메뉴를 지정해줌
    glutAttachMenu(GLUT_RIGHT_BUTTON);//현재 메뉴는 마우스 오른쪽 클릭에 생성됌
    glutAddSubMenu("3D Model", subMenu_id); //서브메뉴로 들어감, 서브메뉴 id 같이
    glutAddMenuEntry("Exit", 2);//엔트리를 만들고, 지정된 현재메뉴의 콜백 함수의 인자로 value전달
    
    glutMainLoop();
}

