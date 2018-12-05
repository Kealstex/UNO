#include <GL/freeglut.h>
#include "SOIL/SOIL.h"
#include "Card.h"

vector<Card> Deck, Discard;      // колода сброса и колода игровая;
Player Player1, Player2;        // игроки
int state = 0;                    // стадии
// 1 - первый игрок
int xPos = 0, yPos = 0;
GLfloat wSide = 0.1f, hSide = 0.2f;             // сторoна карты - 1/20 от ширины экрана и 1/5 от высоты
GLfloat dx = 0.05f, dy = 0.2f;                        // расстояние между картами
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
        //cout << str << endl;
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
    //начало игры
    if (state == 0) {
        RandomDeck(35, Player1);
        RandomDeck(35, Player2);

        for (int i = 0; i < Player1.deck.size(); i++) {
            cout << Player1.deck[i].Color << " " << Player1.deck[i].Value << endl;
        }

        DrawCards(1, Player1.deck);
        DrawCards(2, Player2.deck);
        //state = 1;
    }
    if (state == 1) {
        DrawCards(1, Player1.deck);
        DrawCards(2, Player2.deck);
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
    glutInitWindowSize(wScreen , hScreen );
    glutCreateWindow("UNO");
    glEnable(GL_DEPTH_TEST);

    //инициализая карт
    InitDeck();
    InitWindow();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    LoadTextures();
    glutMainLoop();
    return 0;
}
