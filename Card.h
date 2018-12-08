//
// Created by Kealstex on 03.12.2018.
//

#ifndef UNTITLED1_CARD_H
#define UNTITLED1_CARD_H
#include <algorithm>
#include <vector>
#include <iostream>
#include <ctime>
#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;
struct Card {                                   //структура каждой карты
    char color = 'A';                           // цвет карты. RGBYA
    int Value = 0;                              // Значение карты.
    int Score = 0;                              // Рекорд
    GLfloat x1=0;                               // координата левого верхнего угла
    GLfloat y1=0;
    GLfloat x2=0;                               // координата правого нижнего
    GLfloat y2=0;

};
struct Player {
    vector<Card> deck;
    int score=0;
    int PNumber;
    GLfloat x1,x2,y2;                      // х1 - левый верхний, х2 - правый нижний
};
extern Player Player1, Player2;         // игроки
extern vector<Card> Deck, Discard;      // Игровая колода и колода сброса
extern GLuint textures[5][15];          // массив текстур
extern GLfloat dx, dy, wSide,hSide;     // изменение расстояния между картами, стороны карт
extern double wScreen,hScreen;

void Shuffle(vector<Card> &v);                                               //перемешивание колоды
void SortIsInHand(vector<Card> &v);                                          //Сортировка по цвету
void InitCard(char color, int value, int score);                             //Создание одной карты
void InitDeck();                                                             //Cоздание всех карт
void more(int count, Player &player);                                        //Помещение count карт из Deck в Руки
void DrawCards(int player, Player &players);
void DrawCard(Card card);
void Activity();                                                             // кладет верхнюю карту из колоды в сброс (в начале игры)
void DrawActivity();                                                         // Отрисовывает активную карту
void DrawDeck(char color);                                                             // Отрисовывает колоду
void renderBitmapString(float x, float y, float z, void *font, string String); // функция вывода текста
void DrawButton();                                                              //рисуем кнопку
#endif //UNTITLED1_CARD_H
