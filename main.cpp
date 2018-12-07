#include <GL/freeglut.h>
#include <zconf.h>
#include "SOIL/SOIL.h"
#include "Game.h"

vector<Card> Deck, Discard;      // колода сброса и колода игровая;
Player Player1, Player2;        // игроки
int state = 0;                    // стадии
//0 - начало
//1 - ход мой
//2 - проверка на правила;
//3 - ход компа
//4 - нажатие на колоду, чтобы взять карту

int xPos = 6000, yPos = 6000;
GLfloat wSide = 0.1f, hSide = 0.35f;             // сторoна карты - от ширины и высоты
GLfloat ar = wSide / hSide;
GLfloat dx = 0.05f, dy = 0.2f;                        // расстояние между картами
double hScreen, wScreen;          // монитор
GLuint textures[5][15];

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
    for (int i = 0; i < 5; i++) {
        str = string("textures/texture") + std::to_string(i) + string("-");
        for (int j = 0; j < 15; j++) {
            string str1 = str + std::to_string(j) + string(".png");
            textures[i][j] = SOIL_load_OGL_texture(
                    str1.c_str(),
                    SOIL_LOAD_AUTO,
                    SOIL_CREATE_NEW_ID,
                    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
            );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            //cout << str1 << endl;
        }
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
        return (xPos / (wScreen / 2.0)) - 1;
    }
    return xPos / (wScreen / 2.0) - 1;
}

GLfloat getY() {
    if (yPos <= hScreen / 2.0) {
        return 1 - 2.0f * yPos / hScreen;
    } else return -2.0 * (yPos - hScreen / 2.0f) / hScreen;
}

// возвращает индекс выбранной карты.Иначе -1
int IsCard() {
    for (int i = 0; i < Player1.deck.size(); i++) {
        if (getX() >= Player1.deck[i].x1 && getX() <= Player1.deck[i].x2 &&
            getY() <= Player1.deck[i].y1 && getY() >= Player1.deck[i].y2) {
            //cout<<Player1.deck[i].color<<endl;
            /*yPos=6000;
            xPos=6000;*/
            return i;
        }

    }
    return -1;
}

bool IsDeck() {
    Card card = Deck.back();
    card.x1 =-wSide/2.0-9*wSide;
    card.x2 = card.x1 + wSide;
    card.y1 = hSide/2.0;
    card.y2 = card.y1 - hSide;
    if(getX() >= card.x1 && getX() <= card.x2 &&
       getY() <= card.y1 && getY() >= card.y2){
        return true;
    }
    return false;
}


// получаем координаты если нажата ЛКМ
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        xPos = x;
        yPos = y;
    }
}

int lastId;

void renderBitmapString(float x, float y, float z, void *font, string String)
{
    glRasterPos3f(x, y, z);
    for (int i = 0; i < String.size(); i++) {
        glutBitmapCharacter(font, String[i]);
    }
}
bool triger = true;
// display() Callback function
void display() {
    //cout << state << " " << Deck.size() << " " << Discard.size() << " " << Player1.deck.size() << " " << Player2.deck.size() << endl;
    hScreen = glutGet(GLUT_WINDOW_HEIGHT);
    wScreen = glutGet(GLUT_WINDOW_WIDTH);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //начало игры
    if (state == 0) {
        //очистка колод
        DeckClear();
        //инициализация новой колоды
        InitDeck();
        //раздача по 7 карт
        more(7, Player1);
        more(7, Player2);
        //достаем первую активную карту
        Activity();
        state = 1; // ходит первый игрок

    }
    if (state == 1) {
        if ((Discard.back().Value == 10 || Discard.back().Value == 12) && triger) {
            cout << "Player 1 deactivate card 10||12" << endl;
            state = 3;
            triger = false;
        } else if ((Discard.back().Value == 11) && triger) {
            cout << "Player 1 deactivate card 11" << endl;
            more(2,Player1);
            state = 1;
            triger = false;
        } else {
            //если нажал по карте - проверить правила
            lastId = IsCard();
            if (lastId > -1) {
                state = 2;
                cout << "OK card is OK" << endl;
            } else if (IsDeck()){
                if (IsRight(Deck.back())) {
                    Card card = Deck.back();
                    Discard.push_back(card);
                    Deck.pop_back();
                    cout << "Player 1 Give card " << endl;
                    cout << "Player 1 Push card " << card.color << " " << card.Value << endl;
                } else {
                    // если нажал на колоде - добавить карту
                    more(1,Player1);
                }

                state = 3;
            }
        }
        xPos = 9999;
        yPos = 9999;
        //cout<<"x="<<getX()<<"y="<<getY()<<"\n";
    } else if (state == 2) {
        //если выбранная карта соответствует правилам - скинуть её в отбой
        if (IsRight(Player1.deck[lastId])) {
            PushInDiscard(lastId, Player1);
            if (Discard.back().Value >= 10 && Discard.back().Value <= 12) {
                triger = true;
                cout << "Player 1 activate card 10||11||12" << endl;
            }
            lastId = -1;
            state = 3;
        } else {
            state = 1;
        }
        if (Player1.deck.size() == 0) {
            cout << "You win!";
            state = 5;
        }
        //sleep(5);
    } else if (state == 3) {
        if ((Discard.back().Value == 10 || Discard.back().Value == 12) && triger) {
            state = 1;
            cout << "Player 2 deactivate card 10||12" << endl;
            triger = false;
        } else if ((Discard.back().Value == 11) && triger) {
            more(2,Player2);
            cout << "Player 2 deactivate card 11" << endl;
            state = 3;
            triger = false;
        } else {
            //если нет подохядщей карты -> берет одну;
            if (Player2Chose() == -1) {
                cout << "Player 2 Give card " << endl;
                more(1, Player2);
            }
            //если подходящая карта есть -> кладет в отбой
            Card card = Discard.back();
            if (PushInDiscard(Player2Chose(), Player2)) {
                if (Discard.back().Value >= 10 && Discard.back().Value <= 12) {
                    triger = true;
                    cout << "Player 2 activate card 10||11||12" << endl;
                }
            }
            state = 1;
            if (Player2.deck.size() == 0) {
                cout << "You are loser!";
                state = 5;
            }
        }

    }
    if (state == 5) {
        string str = (Player1.deck.size() == 0 ? string("You win!") : string("You are loser!"));
        renderBitmapString(0, 0, 1, GLUT_BITMAP_TIMES_ROMAN_24, str);
        glFlush();
        glutSwapBuffers();
        glutPostRedisplay();
        sleep(5);
        exit(0);
    } else {
        SortIsInHand(Player1.deck);
        DrawDeck();
        DrawActivity();
        DrawCards(1, Player1);
        DrawCards(2, Player2);
    }



    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
    //sleep(1);
}

int main(int argc, char *argv[]) {
    Player1.PNumber = 1;
    Player2.PNumber = 2;
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
    //glutTimerFunc(timer,)
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    LoadTextures();
    glutMainLoop();
    return 0;
}
