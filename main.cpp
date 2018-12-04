#include <GL/gl.h>
#include <GL/freeglut.h>
#include "SOIL/SOIL.h"
#include <cmath>
#include "Card.h"

//���������� ����������
vector<Card> Deck, Discard;      // колода сброса и колода игровая;
Player Player1, Player2;        // игроки
int state=0;                    // позиция нажатия
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
//Рисуем одну карту
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
    glTexCoord2d(1.0, 0.0);        glVertex3f(x2,y2,0);
    glTexCoord2d(1.0, 1.0);        glVertex3f(x2,y1,0);
    glTexCoord2d(0.0, 1.0);        glVertex3f(x1,y1,0);
    glEnd();
}
void DrawCards(Player Player1){

}
// получаем координаты если нажата ЛКМ
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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (state==0){
        InitDeck();

    }

    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT));
    glutCreateWindow("UNO");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    InitWindow();
    LoadTextures();
    glutMainLoop();
    return 0;
}
