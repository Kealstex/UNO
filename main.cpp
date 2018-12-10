#include <GL/freeglut.h>
//#include <zconf.h>
#include "SOIL/SOIL.h"
#include "Game.h"
#include <fstream>

vector<Card> Deck, Discard;      // колода сброса и колода игровая;
Player Player1, Player2;        // игроки
int state = 7;                    // стадии
//7 - меню;
//0 - начало
//1 - ход мой
//2 - проверка на правила;
//3 - ход компа
//4 - нажатие на колоду, чтобы взять карту или положить её в сброс
//5 - конец игры
//6 - выбор цвета карты
int lastId;
int xPos = -100, yPos = -100;
GLfloat wSide = 0.1f, hSide = 0.35f;             // сторoна карты - от ширины и высоты
GLfloat dx = 0.05f, dy = 0.2f;                        // расстояние между картами
double hScreen, wScreen;                              // монитор
bool triger = true;                                   // срабатывание активных карт
bool readScore = true;
char color = 'A';                                      //Cторона показа колоы. А - рубашка
GLuint textures[5][15];
int scoreArr[10];
fstream fin;

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
        }
    }
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
int isCard() {
    for (int i = 0; i < Player1.deck.size(); i++) {
        if (getX() >= Player1.deck[i].x1 && getX() <= Player1.deck[i].x2 &&
            getY() <= Player1.deck[i].y1 && getY() >= Player1.deck[i].y2) {
            return i;
        }

    }
    return -1;
}

//проверяет на нажатие по колоде
bool isDeck() {
    Card card = Deck.back();
    card.x1 = -wSide / 2.0 - 9 * wSide;
    card.x2 = card.x1 + wSide;
    card.y1 = hSide / 2.0;
    card.y2 = card.y1 - hSide;
    if (getX() >= card.x1 && getX() <= card.x2 &&
        getY() <= card.y1 && getY() >= card.y2) {
        return true;
    }
    return false;
}

//Если нажата стрелка вверх -> ходит. Иначе забирает в руких
bool isButton() {
    if (getX() >= 0.5f && getX() < 0.5f + wSide &&
        getY() >= -wSide && getY() <= wSide) {
        cout << "Button clicked." << endl;
        return true;
    }
    return false;
};

bool isNewGame() {
    if (getX() >= -0.9 && getX() <= -0.13 &&
        getY() >= -0.84 && getY() <= -0.55) {
        return true;
    } else return false;
}

bool isExit() {
    if (getX() >= 0.125 && getX() <= 0.9 &&
        getY() <= -0.55 && getY() >= -0.84) {
        return true;
    }
    return false;
}

char isChoseColor() {
    if (getY() >= 0.095 || getY() <= -0.095) {
        return 0;
    }
    if (getX() <= 0.4 && getX() >= 0.3) {
        return 'R';
    } else if (getX() <= 0.5 && getX() >= 0.4) {
        return 'G';
    } else if (getX() <= 0.6 && getX() >= 0.5) {
        return 'B';
    } else if (getX() <= 0.7 && getX() >= 0.6) {
        return 'Y';
    }
    return 0;
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
    //ход первого игрока
    if (state == 1) {
        //Если активная карта - специальная (смена хода или пропуск хода)
        if ((Discard.back().Value == 10 || Discard.back().Value == 12) && triger) {
            //выключается свойство карты
            triger = false;
            //Игрок пропускает ход, ход переходит
            state = 3;
        }
            //Если активная карта - возьми+2
        else if ((Discard.back().Value == 11) && triger) {
            //игрок берет две карты и может походить
            more(2, Player1);
            state = 1;
            //выключается свойство карты
            triger = false;
        }
            //Если активная карта - +4;
        else if (Discard.back().Value == 14 && triger) {
            more(4, Player1);
            triger = false;
        } else {
            //если нажал по карте - проверить правила
            lastId = isCard();
            if (lastId > -1 && Player1.deck[lastId].color == 'A') {
                /*if(Player1.deck[lastId].Value==14){
                    triger = true;
                }
                lastId = -1;*/
                state = 6;
            }
                //Если нажал по колоде
            else if (isDeck()) {
                //Кладет карту из колоды, если она удовлетворяет правилам
                if (IsRight(Deck.back())) {
                    lastId = -1;
                    state = 4;
                } else {
                    // если нажал на колоде - добавить карту
                    more(1, Player1);
                    state = 3;
                }

            } else {
                state = 2;
            }
        }
        xPos = -100;
        yPos = -100;
    }
        //Проверка на возможность хода
    else if (state == 2) {
        //если выбранная карта соответствует правилам - скинуть её в отбой
        if (IsRight(Player1.deck[lastId])) {
            PushInDiscard(lastId, Player1);
            //если карта специальная или черная +4:
            if ((Discard.back().Value >= 10 && Discard.back().Value <= 12) || Discard.back().Value == 14) {
                triger = true;
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
    }
        //если ход компа
    else if (state == 3) {
        Sleep(1000);
        //Если карта специальная ( пропуск хода )
        if ((Discard.back().Value == 10 || Discard.back().Value == 12) && triger) {
            state = 1;
            cout << "Player 2 deactivate card 10||12" << endl;
            triger = false;
        }
            //Если карта специальная (возьми+2)
        else if ((Discard.back().Value == 11) && triger) {
            more(2, Player2);
            cout << "Player 2 deactivate card 11" << endl;
            state = 3;
            triger = false;
        } else if ((Discard.back().Value == 14) && triger) {
            more(4, Player2);
            triger = false;
        } else {
            //если нет подохядщей карты -> берет одну;
            if (Player2Chose() == -1) {
                cout << "Player 2 Give card " << endl;
                more(1, Player2);
            }
            //Card card = Discard.back();
            //если подходящая карта есть(появилась после взятия) -> кладет в отбой
            if (PushInDiscard(Player2Chose(), Player2)) {
                if ((Discard.back().Value >= 10 && Discard.back().Value <= 12) || Discard.back().Value == 14) {
                    triger = true;
                    cout << "Player 2 activate card 10||11||12||14" << endl;
                }
            }
            state = 1;
            if (Player2.deck.size() == 0) {
                state = 5;
            }
        }

    }
    //нажали по колоде. взятая карта может быть сыграна
    if (state == 4) {
        color = 'B';
        DrawButton();
        if (isButton()) {
            if (getY() < 0) {
                more(1, Player1);
                state = 3;
            } else {
                Card card = Deck.back();
                if (Deck.back().Value >= 10 && Deck.back().Value <= 12 || Deck.back().Value == 14) {
                    triger = true;
                }
                if (Deck.back().color != 'A') {
                    Discard.push_back(card);
                    Deck.pop_back();
                    state = 3;
                } else {
                    //cбить координаты (сохраняется с кнопки)
                    xPos = -100;
                    yPos = -100;
                    lastId = -1;
                    state = 6;
                }
            }
            color = 'A';
        }

    }
    if (state == 6) {
        DrawChoseColor();
        char chose;
        chose = isChoseColor();
        if (chose) {
            Card card;
            //cout<<lastId<<endl;
            if (lastId > -1) {
                card = Player1.deck[lastId];
            } else card = Deck.back();
            card.color = chose;
            if (card.Value == 14) {
                triger = true;
            }
            if (lastId > -1) {
                Player1.deck.erase(Player1.deck.begin() + lastId);
            } else Deck.pop_back();
            Discard.push_back(card);
            state = 3;
        }
        glFlush();

    }
    if (state == 5) {
        if (Player1.deck.size() == 0) {
            // текстура в случае выйгрыша
            DrawBackground(10);
            score();
            if (Player1.score > scoreArr[9] && readScore) {
                scoreArr[9] = Player1.score;
                int temp;
                for (int i = 0; i < 10; i++) {
                    for (int j = 0; j < 10 - i - 1; j++) {
                        if (scoreArr[j] < scoreArr[j + 1]) {
                            temp = scoreArr[j];
                            scoreArr[j] = scoreArr[j + 1];
                            scoreArr[j + 1] = temp;
                        }
                    }
                }
                readScore = false;
                fin.open("score.txt");
                for (int i = 0; i < 10; i++) {
                    fin << scoreArr[i];
                    fin << " ";
                }
                fin.close();

            }
        } else {
            // текстура в случае пройгрыша
            DrawBackground(11);
        }
        glFlush();
        if (isNewGame()) {
            state = 0;
        }
        if (isExit()) {
            exit(0);
        }


    } else if (state == 7) {
        if (!Deck.size()) {
            InitDeck();
        }
        DrawBackground(8);

        /*fstream fin("score.bin",ios_base::binary | ios_base::in);
        for (int i = 0; i < 10; i++) {
            int a;
            fin.read((char*)&a, sizeof(int));
            //cout<<scoreArr[i]<<endl;
            cout<<a<<endl;
            renderBitmapString(-0.85, 0.75-i*0.12,1,GLUT_BITMAP_TIMES_ROMAN_24,to_string(a));
        }
        fin.close();*/
        fin.open("score.txt");
        for (int i = 0; i < 10; i++) {
            fin >> scoreArr[i];
            renderBitmapString(-0.85, 0.75 - i * 0.12, 1, GLUT_BITMAP_TIMES_ROMAN_24, to_string(scoreArr[i]));
        }
        fin.close();
        if (isNewGame()) {
            state = 0;
        }
        if (isExit()) {
            exit(0);
        }
    } else {
        SortIsInHand(Player1.deck);
        DrawDeck(color);
        DrawActivity();
        DrawCards(1, Player1);
        DrawCards(2, Player2);
    }


    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
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
    LoadTextures();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
