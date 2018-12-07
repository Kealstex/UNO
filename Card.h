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

using namespace std;
struct Card {                           //��������� ����� �����
    char Color = 'A';                   // ���� �����. RGBYA - �������?�����
    int Value = 0;                      // ������� �����
    int Score = 0;                      // �� �������� �����
    bool IsInHand = false;              // ������ �� ���������� �������������� �����
    GLfloat x1=0;                           // ����� �������
    GLfloat y1=0;
    GLfloat x2=0;                           // ������ ������
    GLfloat y2=0;

};
struct Player {
    vector<Card> deck;
    int score=0;
    GLfloat x1,x2,y2;                      // х1 - левый верхний, х2 - правый нижний
};
extern Player Player1, Player2;         // игроки
extern vector<Card> Deck, Discard;      // Игровая колода и колода сброса
extern GLuint textures[6];
extern GLfloat dx, dy, wSide,hSide;
extern double wScreen,hScreen;
void Shuffle(vector<Card> &v);          //перемешивание колоды
void SortIsInHand(vector<Card> &v);                       //Сортировка по наличию в руках ( в руках - в конце)
void InitCard(char color, int value, int score);     //Создание одной карты
void InitDeck();                        //Cоздание всех карт
void more(int count, Player &player);   //Помещение count карт из Deck в Руки
void DrawCards(int player, Player &players);
void DrawCard(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, char color);
void Activity();                   // кладет верхнюю карту из колоды в сброс (в начале игры)
void DrawActivity();                    // Отрисовывает активную карту
void DrawDeck();
#endif //UNTITLED1_CARD_H
