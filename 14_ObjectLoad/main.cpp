
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

using namespace std;

class CPoint2f{ //2차원 실수 좌표를 가지는 class
public :
    vector <float> d{ 0, 0};
};

class CPoint3f{ //3차원 실수 좌표를 가지는 class
public :
    vector <float> d{ 0, 0, 0};
};

class CPoint3i{ //3차원 정수형 좌표를 가지는 class
public :
    vector <int> d{ 0, 0, 0};
};

class CFace{ //면을 이루는 vertex pairs. 1/1/1 2/2/1 4/3/1 3/4/1 네개의 정점이 하나의 면을 이룬다
public :
    vector <CPoint3i> v_pairs; // 1/1/1이 하나의 v_pairs
};

class CObj{ //Obj가 가지는 정보들
public :
    string name;
    vector <CPoint3f> v;
    vector <CPoint2f> vt; //texture관련
    vector <CPoint3f> vn;
    vector <CFace> f;
};

class CModel{ //하나의 모델엔 여러개의 오브젝트가 존재할 수 있다.
public :
    vector <CObj> objs;
    
    vector<float> my_strock_f(char * str, char* delimeter){//공백 단위로 분리해야함. vn -1.0000 0.00000 0.0000 이런 형태로 들어옴
        vector <float> v;
        char* context;
        char* tok = strtok(str, delimeter);
        
        while(tok != NULL){
            v.push_back(atof(tok)); //문자를 실수형으로
            tok = strtok(NULL, delimeter);
        }
        return v;
    }
    
    vector<string> my_strock_s(char * str, char* delimeter){//공백 단위로 분리해야함. vn -1.0000 0.00000 0.0000 이런 형태로 들어옴
        vector <string> v;
        char* context;
        char* tok = strtok(str, delimeter);
        
        while(tok != NULL){
            v.push_back(tok); //문자를 실수형으로
            tok = strtok(NULL, delimeter);
        }
        return v;
    }
    
    vector<int> my_strock_i(char * str, char* delimeter){//공백 단위로 분리해야함. vn -1.0000 0.00000 0.0000 이런 형태로 들어옴
        vector <int> v;
        char* context;
        char* tok = strtok(str, delimeter);
        
        while(tok != NULL){
            v.push_back(atoi(tok)); //문자를 실수형으로
            tok = strtok(NULL, delimeter);
        }
        return v;
    }
    
    void loadObj(ifstream& fin){
        string line;
        CObj obj_tmp;
        int cnt = 0;
        //이전 obj까지의 누적 벡터수
        //obj파일상에서는 obj들의 전체 vertex개수 ex)65개로 다루지만
        //각각의 오브젝트로 나누어서 생각할 필요가 있음
        int cnt_prev_vertex = 0;
        int cnt_prev_texture = 0;
        int cnt_prev_normal = 0;
        
        while(getline(fin,line)){
            int len =  line.length();
            vector<float> vf;
            vector<string> s;
            vector<int> vi;
            CPoint3f p3;
            CPoint2f p2;
            CPoint3i p3i;
            if(line[0] == 'o' && line[1] == ' '){
                obj_tmp.name = line.substr(2, len-2);
                objs.push_back(obj_tmp);
                if(cnt >0){
                    cnt_prev_vertex += objs[cnt -1].v.size();
                    cnt_prev_texture += objs[cnt -1].vt.size();
                    cnt_prev_normal += objs[cnt -1].vn.size();
                }
                cnt +=1;
            }
            if(line[0] == 'v' && line[1] == ' '){
                vf = my_strock_f((char*)line.substr(2, len-2).c_str(), (char*)" ");
                p3.d = {vf[0],vf[1],vf[2]};
                objs[cnt - 1].v.push_back(p3);
            }
            
            else if(line[0] == 'v' && line[1] == 't'){
                vf = my_strock_f((char*)line.substr(3, len-3).c_str(), (char*)" ");
                p2.d = {vf[0],vf[1]};
                objs[cnt - 1].vt.push_back(p2);
            }
            else if(line[0] == 'v' && line[1] == 'n'){
                vf = my_strock_f((char*)line.substr(3, len-3).c_str(), (char*)" ");
                p3.d = {vf[0],vf[1],vf[2]};
                objs[cnt - 1].vn.push_back(p3);
            }
            else if(line[0] == 'f' && line[1] == ' '){
                s = my_strock_s((char*)line.substr(2, len-2).c_str(), (char*)" ");
                int nVertexes = s.size();
                CFace face_tmp;
                for(int i = 0; i < nVertexes; i++){
                    vi = my_strock_i((char*)s[i].c_str(), (char*)"/");
                    p3i.d = { vi[0] - cnt_prev_vertex,vi[1]  - cnt_prev_texture,vi[2]-cnt_prev_normal};
                    //각각의 오브젝트들의 순서를 처리하기 위해서 이전 누적된 벡터,노말,텍스쳐 개수를 빼줌
                    face_tmp.v_pairs.push_back(p3i);
                }
                objs[cnt -1].f.push_back(face_tmp);
            }
        }
    }
};

#define _WINDOW_WIDTH 500
#define _WINDOW_HEIGHT 500

GLfloat camPos_x = 5, camPos_y =2, camPos_z = -10; //위치
GLfloat camAt_x = 0, camAt_y = 0, camAt_z = 0; //바라보는 곳
GLfloat camUp_x = 0, camUp_y = 1, camUp_z = 0; //방향

CModel m;

void light_material(){
    glDisable(GL_COLOR_MATERIAL); //display할 때마다 기존의 색상을 무효화시키고 새로 그려라, 이렇게 안하면 reshape될때 색이 바뀜
    GLfloat material_0_specular[] = {1.0, 1.0, 1.0, 1.0};//빛 색깔과 그대로 맞춰주기
    GLfloat material_0_diffuse[] = {0.8, 0.0, 0.3, 1.0};
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

void light_global(){
    GLfloat light_global_ambient[] = {0.3,0.3,0.3, 1}; //전역 주변광 색깔 지정
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_global_ambient);//전역 주변광을 주겠다.
}

void light0_local(){
    GLfloat light_0_pos[] = {3.0,3.0,-3.0,1.0}; //빛 위치 지정 공 바로 위에
    GLfloat light_0_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_0_diffuse[] = {0.0, 0.0, 1.0, 1.0};
    GLfloat light_0_ambient[] = {1.0 , 1.0, 1.0, 1.0};//백색광 = 태양
     
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_0_pos); //0번째 Light Position지정 개수는 GPU에 따라 다름
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_0_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_0_ambient);
}

void display_obj(){
    GLfloat x,y,z,nx,ny,nz;
    int v_id, vt_id, vn_id;
    for(int o = 0; o <m.objs.size(); o++){
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
                glNormal3f(nx, ny, nz); //법선벡터 지정
                glVertex3f(x, y, z); //점찍기
            }
            glEnd();
        }
    }
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

void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(camPos_x, camPos_y, camPos_z, camAt_x, camAt_y, camAt_z, camUp_x,camUp_y, camUp_z);
    
    light_global();
    light0_local();
    light_material();
    display_obj();
    
}

void Init_light(){
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

int main(int argc, char** argv) {
    string filepath = "/Users/jaewonna/Desktop/WorkspaceC++/Computer\ Graphics/Computer\ Graphics/14_ObjectLoad/monkey.obj";
    ifstream fin(filepath);
    
    
    m.loadObj(fin);
    
    fin.close();
    
    //gl 관련
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
    glutInitWindowPosition(300, 200);
    
    glutCreateWindow("OBJLoad");
    
    glClearColor(0,0,0,0.0);
    
    Init_light();
    
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    
    glutMainLoop();
    return 0;
}
