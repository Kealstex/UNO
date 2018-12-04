#include <GL/freeglut.h>
#include "SOIL/SOIL.h"
#include "Card.h"

vector<Card> Deck, Discard;      // колода сброса и колода игровая;
Player Player1, Player2;        // игроки
int state = 0;                    // стадии
// 1 - первый игрок
int xPos = 0, yPos = 0;
GLfloat wSide=0.1f, hSide=0.2f;             // сторoна карты - 1/20 от ширины экрана и 1/5 от высоты
GLfloat dx=0.05f,dy = 0.2f;                        // расстояние между картами
double hScreen, wScreen;          // монитор
GLuint textures[6];

using namespace std;

void InitWindow() {
    glEnable(GL_TEXTURE_2D);
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glutInitWindowPosition(0, 0);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

void LoadTextures() {
    string str;
    for (int i = 0; i < 6; i++) {
        str = "texture";
        str += (48 + i);
        str += ".png";
        cout << str << endl;
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

//Рисуем одну карту. Указываем верхний левый и правый нижний
GLvoid DrawCard(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, char color) {
    int c = 4;
    switch (color) {
        case 'R':
            c = 0;
            break;
        case 'G':
            c = 1;
            break;
        case 'B':
            c = 2;
            break;
        case 'Y':
            c = 3;
            break;
        case 'A':
            c = 4;
            break;
        default:c=5;
    }
    glBindTexture(GL_TEXTURE_2D, textures[c]);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex3f(x1, y2, 0);
    glTexCoord2d(1.0, 0.0);
    glVertex3f(x2, y2, 0);
    glTexCoord2d(1.0, 1.0);
    glVertex3f(x2, y1, 0);
    glTexCoord2d(0.0, 1.0);
    glVertex3f(x1, y1, 0);
    
    glEnd();
}

void DrawCards(int player, vector<Card> &v) {
    GLfloat x1 = -1.0f, x2 = x1 + wSide, y1 = -0.4f, y2 = -0.8f;
    if (v.size()<21)dx = (2.0f - v.size()*wSide)/2.0f;
    else dx=0;
    x1+=dx;
    x2+=dx;
    GLfloat y ;                                            //количество рядов
    cout<<endl<<x1<<" "<<x2<<"--->"<<dx;
    //если это первый игрок(я)
    if (player == 1) {
            for (int i = 0; i < v.size(); i++) {
                DrawCard(x1, y1, x2, y2, v[i].Color);
                v[i].x1=x1; v[i].x2 = x2; v[i].y1=y1;v[i].y2=y2;
                x1=x2;
                x2 += wSide;
                y=(i+2)/21.0f;
                //если следующий ряд - обновляем координаты;
                if(y==1.0){
                    x1=-1.0f;
                    x2=x1+wSide;
                    y1=-0.4f - dy*y;
                    y2=-0.8f - dy*y;
                }
            }
        }
    }

// получаем координаты если нажата ЛКМ
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
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
    if (state == 0) {
        InitDeck();
        RandomDeck(23, Player1);
        RandomDeck(7, Player2);

        for (int i = 0; i < Player1.deck.size(); i++) {
            cout << Player1.deck[i].Color << " " << Player1.deck[i].Value << endl;
        }

        DrawCards(1, Player1.deck);
        state = 1;
    }
    if(state==1){
        DrawCards(1, Player1.deck);
    }

    glFlush();
    glutSwapBuffers();
    //glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    //размеры экрана
    hScreen = glutGet(GLUT_SCREEN_HEIGHT);
    wScreen = glutGet(GLUT_SCREEN_WIDTH);
    glutInitWindowSize(wScreen/2, hScreen/2);
    glutCreateWindow("UNO");
    glEnable(GL_DEPTH_TEST);


    //Иниц. дека
    InitDeck();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    InitWindow();
    LoadTextures();
    glutMainLoop();
    return 0;
}
