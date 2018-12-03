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
struct Card {                           //��������� ����� �����
    char Color = 'A';                   // ���� �����. RGBYA - �������?�����
    int Value = 0;                      // ������� �����
    int Score = 0;                      // �� �������� �����
    bool IsInHand = false;              // ������ �� ���������� �������������� �����
    int x1=0;                           // ����� �������
    int y1=0;
    int x2=0;                           // ������ ������
    int y2=0;

};
struct Player {
    vector<Card> deck;
    int score=0;
};
extern Player Player1, Player2;
extern vector<Card> Deck, Discard;      // �������� ������ � �����;
void Shuffle(vector<Card> &v);          //����������� ����
void SortIsInHand(vector<Card> &v);                       // ���������� �� ��������
void InitCard(char color, int value, int score);     //���������� �����
void InitDeck();                        //���������� ������
vector<Card> RandomDeck(int count, Player &player); //�������� ��������� ������ �� count ����

#endif //UNTITLED1_CARD_H
