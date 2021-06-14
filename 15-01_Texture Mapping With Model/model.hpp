#include <vector>
#include <string>
#include <fstream>
#include <iostream>

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
