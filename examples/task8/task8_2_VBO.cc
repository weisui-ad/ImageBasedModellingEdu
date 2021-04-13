
//
// Created by sway on 2021/3/11.
//
#include <iostream>
#include <GL/glew.h>

#ifdef  __APPLE__
#include <GLUT/glut.h>

#endif
#ifdef __linux__
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <GL/gl.h>
#endif


#define BUFFER_OFFSET(offset) ((void*) NULL + offset)
#define NumberOf(array) (sizeof(array)/sizeof(array[0]))

GLfloat angle = 0.0;
GLfloat trans =0.0;
GLsizei n_elements;

// 顶点数组索引
GLuint VAO;

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

    glewInit();  // very important, without this statement causes segment error!!
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);  // 平滑模式

    // 创建物体
    make_objects();
}

void display(){

    // 清除深度缓冲器和颜色缓冲器
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    // Render a color-cube consisting of 6 quads with different colors
    glLoadIdentity();
    glTranslatef(1.5f, trans, -7.0f);  // Move right and into the screen
    glRotatef(angle, 0, 1.0, 0.0);// Reset the model-view matrix

    // 3种不同的模式，分别将多边形渲染成点，线和填充的模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(VAO);
    glDrawElements(GL_QUADS, n_elements, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));

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

void spinRotate(void){
    angle = angle +2.0;
    if (angle > 360.0) angle-=360.0;
    if(angle<-360) angle += 360.0;
    glutPostRedisplay();
}

void spinTrans(void){
    trans +=0.1;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
    switch (key) {
        case 'a':
            angle+=1.0;
            glutPostRedisplay();
            break;
        case 'd':
            angle-=1.0;
            glutPostRedisplay();
            break;
        case 'q':
            glutIdleFunc(NULL);
            glutPostRedisplay();
            break;
        case 'w':
            trans += 0.05;
            glutPostRedisplay();
            break;
        case 's':
            trans -= 0.05;
            glutPostRedisplay();
            break;
        default:
            glutPostRedisplay();
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