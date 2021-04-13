//
// Created by sway on 2021/4/5.
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


#define checkImageWidth 64
#define checkImageHeight 64

static GLubyte checkImage [checkImageHeight][checkImageWidth][4];
static GLuint texName;
GLfloat angle = 0.0;
GLfloat trans =0.0;


//床架棋盘格图像
void makeCheckImage(void){

    for (int i=0; i< checkImageHeight; i++){
        for (int j=0; j< checkImageWidth; j++){
           int c = (((i&0x8)==0)^((j&0x8))==0)*255;
           checkImage[i][j][0] = (GLubyte)c;
           checkImage[i][j][1] = (GLubyte)c;
           checkImage[i][j][2] = (GLubyte)c;
           checkImage[i][j][3] = (GLubyte)255;
        }
    }
}


void init(void){

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);  // 开始深度测试

    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 开启像素缓冲区

    /**
     * 生成纹理的过程
     * 1. 生成纹理名称
     * 2. 初次把纹理对象绑定到纹理数据上，包括图像数组和纹理属性
     * 3. 绑定和重新绑定纹理对象
     */

    // 生成纹理名称，任何无符号的整数都可以用来表示纹理对象的名称
    glGenTextures(1, &texName);

    // 创建和使用纹理初次绑定时，OPenGL会创建一个新的纹理对象，并把纹理图像和纹理属性设置为默认值
    glBindTexture(GL_TEXTURE_2D, texName);

    // 设定当纹理坐标超出1的时候对纹理进行重复
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 指定纹理图像在放大和缩小时候的处理方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // 定义二维纹理， 图像的像素格式为rgba
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
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

void display(void){

    // 每次渲染都要清除之前的缓冲器
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 开启纹理渲染
    glEnable(GL_TEXTURE_2D);

    // 将绘图模式设置为 GL_REPLACE 表示经过纹理贴图后，多边形的颜色完全来自与纹理图像
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // 绑定texName的纹理
    glBindTexture(GL_TEXTURE_2D, texName);

    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    // Render a color-cube consisting of 6 quads with different colors
    glLoadIdentity();
    glTranslatef(1.5f, trans, -7.0f);  // Move right and into the screen
    glRotatef(angle, 0, 1.0, 0.0);// Reset the model-view matrix


    // 开始绘制四边形
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
    glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
    glEnd();

    glFlush();
    glutSwapBuffers();
    glDisable(GL_TEXTURE_2D);

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