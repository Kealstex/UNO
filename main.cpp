#include <GL/freeglut.h>
#include "SOIL/SOIL.h"
#include "Card.h"

vector<Card> Deck, Discard;      // колода сброса и колода игровая;
Player Player1, Player2;        // игроки
int state = 0;                    // стадии
//0 - начало
//1 - ход мой
//2 - проверка на правила;

int xPos = 6000, yPos = 6000;
GLfloat wSide = 0.1f, hSide = 0.35f;             // сторoна карты - 1/20 от ширины экрана и 1/5 от высоты
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

//очищаем колоды
void DeckClear() {
    Deck.clear();
    Discard.clear();
    Player1.deck.clear();
    Player2.deck.clear();
}

// преобразование координат в нужный вид ( -1..1 )
GLfloat getX() {
    if (xPos <= wScreen / 2.0) {
        return 2.0f * xPos / wScreen - 1;
    } else return 2.0 * (xPos - wScreen / 2.0f) / wScreen;
}

GLfloat getY() {
    if (yPos <= hScreen / 2.0) {
        return 1 - 2.0f * yPos / hScreen;
    } else return -2.0 * (yPos - hScreen / 2.0f) / hScreen;
}

// получаем координаты если нажата ЛКМ
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        xPos = x;
        yPos = y;
    }
}

// возвращает индекс выбранной карты.Иначе -1
int IsCard() {
    for (int i = 0; i < Player1.deck.size(); i++) {


        if (abs(getX()) <= abs(Player1.deck[i].x2) && abs(getX()) >= abs(Player1.deck[i].x1)
            && abs(getY()) <= abs(Player1.deck[i].y2) && abs(getY()) >= abs(Player1.deck[i].y1)) {
            //cout<<"YEEEEEEEEEEES IS IT "<<Player1.deck[i].Color;
            //state = ;
            return i;
        }
        //cout<<"X="<<Player1.deck[i].x1<<" "<<Player1.deck[i].x2<<" "<<getX()<<endl;
        //cout<<"Y="<<Player1.deck[i].y1<<" "<<Player1.deck[i].y2<<" "<<getY()<<endl;
    }
    return -1;
}

// display() Callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //опрнднляем позицию курсора
    //начало игры
    if (state == 0) {
        //очистка колод
        DeckClear();
        //инициализация новой колоды
        InitDeck();
        //раздача по 7 карт
        more(7, Player1);
        more(38, Player2);
        //достаем первую активную карту
        PushInDiscard();
        state = 1; // ходит первый игрок

    }
    if (state == 1) {
        //если нажал по карте - проверить правила
        if (IsCard() > -1) {
            state = 2;
        }
        //cout<<"x="<<getX()<<"y="<<getY()<<"\n";
    }
    if (state == 2) {

    }
    DrawDeck();
    DrawActivity();
    DrawCards(1, Player1);
    DrawCards(2, Player2);
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    //размеры экрана
    hScreen = glutGet(GLUT_SCREEN_HEIGHT);
    wScreen = glutGet(GLUT_SCREEN_WIDTH);
    glutInitWindowSize(wScreen, hScreen);
    glutCreateWindow("UNO");
    glEnable(GL_DEPTH_TEST);
    InitWindow();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    LoadTextures();
    glutMainLoop();
    return 0;
}
