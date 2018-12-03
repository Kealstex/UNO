//
// Created by Kealstex on 03.12.2018.
//

#ifndef UNTITLED1_CARD_H
#define UNTITLED1_CARD_H
#include <algorithm>
#include <vector>
#include <iostream>
#include <ctime>

using namespace std;
struct Card {                           //структура одной карты
    char Color = 'A';                   // цвет карты. RGBYA -  рс«ел—ин?Єлт„Єрн
    int Value = 0;                      // номинал карты
    int Score = 0;                      // дл¤ подсчЄта очков
    bool IsInHand = false;              // фунцци¤ дл¤ определени¤ местонахождени¤ карты
    int x1=0;                           // Левый верхний
    int y1=0;
    int x2=0;                           // Правый нижний
    int y2=0;

};
struct Player {
    vector<Card> deck;
    int score=0;
};
extern Player Player1, Player2;
extern vector<Card> Deck, Discard;      // игральна¤ колода и сброс;
void Shuffle(vector<Card> &v);          //перетусовка карт
void SortIsInHand(vector<Card> &v);                       // сортировка по значению
void InitCard(char color, int value, int score);     //«аполнение карты
void InitDeck();                        //«аполнение колоды
vector<Card> RandomDeck(int count, Player &player); //создание рандомной колоды из count карт

#endif //UNTITLED1_CARD_H
