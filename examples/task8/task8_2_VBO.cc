
//
// Created by sway on 2021/3/11.
//
#include <iostream>
#ifdef  __APPLE__
#include <GL/glew.h>
#include <GLUT/glut.h>

#endif
#ifdef __UNIX__
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#endif


#define BUFFER_OFFSET(offset) ((char*) NULL + offset)
#define NumberOf(array) (sizeof(array)/sizeof(array[0]))

typedef struct{
    GLfloat x, y, z;
} vec3;

typedef struct{
    vec3 xlate; /*Translation*/
    GLfloat angle;
    vec3 axis;
} XForm;

//enum {Cube, NumVAOs};
GLuint VAO;
XForm Xform[]={
        {{-2.0, 0.0, 0.0}, 0.0, {0.0, 1.0, 0.0}}
};
GLfloat angle = 0.0;
GLsizei n_elements;

// 定义属性对应的缓冲器
enum {Vertices, Colors, Elements, NumVBOs};
GLuint buffers[NumVBOs];


void make_objects(){

    // 创建顶点数组对象
    glGenVertexArrays(1, &VAO);

    GLfloat cubeVerts[][3]={
            {-1.0, -1.0, -1.0},
            {-1.0, -1.0, 1.0},
            {-1.0, 1.0, -1.0},
            {-1.0, 1.0, 1.0},
            {1.0, -1.0, -1.0},
            {1.0, -1.0, 1.0},
            {1.0, 1.0, -1.0},
            {1.0, 1.0,  1.0}
    };

    GLfloat cubeColors[][3]={
            {0.0, 0.0, 0.0},
            {0.0, 0.0, 1.0},
            {0.0, 1.0, 0.0},
            {0.0, 1.0, 0.0},
            {1.0, 0.0, 0.0},
            {1.0, 0.0, 1.0},
            {1.0, 1.0, 0.0},
            {1.0, 1.0, 1.0}
    };

    GLubyte cubeIndices[]={
            0,  1,  3,  2,
            4,  6,  7,  6,
            2,  3,  7,  6,
            0,  4,  5,  1,
            0,  2,  6,  4,
            1,  5,  7,  3
    };
    n_elements = NumberOf(cubeIndices);

    // 绑定定点数组
    glBindVertexArray(VAO);

    // 每个属性定义一个buffer
    glGenBuffers(NumVBOs, buffers);

    // 绑定缓冲器，并绑定定点数组数据
    // 顶点坐标
    glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    glEnableClientState(GL_VERTEX_ARRAY);

    // 颜色
    glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_STATIC_DRAW);
    glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    glEnableClientState(GL_COLOR_ARRAY);

    // 面片索引
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);


}
void init(){


    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);  // 平滑模式

    // 创建物体
    make_objects();
}

void display(){

    // 清除深度缓冲器和颜色缓冲器
    glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);

    // 3种不同的模式，分别将多边形渲染成点，线和填充的模式
    // glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
    //  glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPushMatrix();
        glBindVertexArray(VAO);
        glDrawElements(GL_QUADS, n_elements, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h){

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    // 透视投影
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w/(GLfloat)h, 0.01, 10000.0);

    // 定义视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // 相机默认位于原点，指向z轴负方向
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y){
    switch (key) {
        case 'd':
            glutPostRedisplay();
            break;
        default:
            glutPostRedisplay();
            break;
    }
}

void spinDisplay(void){
    angle = angle +2.0;
    if (angle > 360.0) angle =  angle -360.0;
    glutPostOverlayRedisplay();
}

void mouse (int button, int state, int x, int y){
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){
                glutIdleFunc(spinDisplay);
            }
            break;
    }
}
int main(int argc, char*argv[]){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;

}