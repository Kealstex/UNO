#include <GL/gl.h>
#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include <iostream>
#include "Card.h"
#include "SOIL/SOIL.h"
#include <cmath>

//���������� ����������
vector<Card> Deck, Discard;      // ��������� ������ � �����;
Player Player1, Player2;        // ������
int state=0;                    // ��� ��� ( 0 - �������, 1 - ���, 2 - �����, 3 - ������� �����
int xPos=0, yPos=0;

using namespace std;
void InitWindow(){
    glEnable(GL_TEXTURE_2D);
    glClearColor(1,1,1,1);
    glMatrixMode(GL_PROJECTION);
    glutInitWindowPosition(0,0);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}
void display();
GLuint textures[5];
void LoadTextures() {
    string str;
    for (int i=0;i<5;i++){
        str="texture";
        str+= (48+i);
        str+= ".png";
        cout<<str<<endl;
        textures[i] = SOIL_load_OGL_texture(
                str.c_str(),
                SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
}
//��������� �����
GLvoid DrawCard(GLfloat x1,GLfloat y1,GLfloat x2, GLfloat y2, char color){
    int c=4;
    switch(color){
        case 'R':c=0;
            break;
        case 'G':c=1;
            break;
        case 'B':c=2;
            break;
        case 'Y':c=3;
            break;
        //case 'A':c=4;
        //    break;
    }
    glBindTexture(GL_TEXTURE_2D, textures[c]);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);        glVertex3f(x1,y2,0);
    glTexCoord2d(1.0, 0.0);        glVertex3f(x1,y1,0);
    glTexCoord2d(1.0, 1.0);        glVertex3f(x2,y1,0);
    glTexCoord2d(0.0, 1.0);        glVertex3f(x2,y2,0);
    glEnd();
}
// c��������� ���������
void mouse(int button, int state, int x,int y){
    if(button ==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        xPos = x;
        yPos = y;
    }
}
// display() Callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(110, 1366.0f / 768.0f, 50.0f, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0.0, 1.0, 0.0);

    glBegin(GL_QUADS);//фронт1 | левая
    glTexCoord2f(0.0f, 0.0f);
    glVertex3d(1.0, 1.0, 0.0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3d(1.0, -1.0, -0.0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3d(-1.0, -1.0, -0.0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3d(-1.0, 1.0, 0.0);
    glEnd();
    glFlush();

    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT));
    glutCreateWindow("Monopoly");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    InitWindow();
    LoadTextures();
    glutMainLoop();
    return 0;
}
