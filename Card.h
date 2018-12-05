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
};
extern Player Player1, Player2;         // игроки
extern vector<Card> Deck, Discard;      // Игровая колода и колода сброса
extern GLuint textures[6];
extern GLfloat dx, dy, wSide,hSide;
void Shuffle(vector<Card> &v);          //перемешивание колоды
void SortIsInHand(vector<Card> &v);                       //Сортировка по наличию в руках ( в руках - в конце)
void InitCard(char color, int value, int score);     //Создание одной карты
void InitDeck();                        //Cоздание всех карт
void RandomDeck(int count, Player &player); //Помещение count карт из Deck в Руки
void DrawCards(int player, vector<Card> &v);
void DrawCard(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, char color);
#endif //UNTITLED1_CARD_H
