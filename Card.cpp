//
// Created by Kealstex on 03.12.2018.
//

#include "Card.h"
void InitDeck() {
    //по цвету
    char color[4] = {'R', 'G', 'B', 'Y'};
    //по цвету -> по номиналу
    for (int j = 0; j < 4; j++) {
        for (int i = 1; i < 14; i++) {
            //<10 value = score, >10 score = 20
            InitCard(color[j], i % 13, (i % 13 > 9) ? 20 : i % 13);
        }
        InitCard('A', 13, 50);
        InitCard('A', 14, 50);
    }
     Shuffle(Deck);
     //SortIsInHand(Deck);
    cout << Deck.size();
}
void InitCard(char color, int value, int score) {
    Card card;
    card.Color = color;
    card.Value = value;
    card.Score = score;
    Deck.push_back(card);
}
// тусовка карт
void Shuffle(vector<Card> &v) {
    srand(unsigned(time(nullptr)));
    // не считаем активную карту
    random_shuffle(v.begin()+1, v.end());
}

void SortIsInHand(vector<Card> &v) {
    for (int i = 0; i < v.size(); i++)
        for (int j = i + 1; j < v.size(); j++)
            if (v[i].IsInHand > v[j].IsInHand) {
                Card temp = v[i];
                v[i] = v[j];
                v[j] = temp;
            }
}
// count-card push in player
void RandomDeck(int count, Player &player) {
    int i = 0;
    Card card;

    //Проверка на наличие count карт в стопке
    if (count > Deck.size()) {
        for (int i = 0; i < Discard.size(); i++) {
            card = Discard[i];
            //кладем из отбоя в игровую колоду
            Deck.push_back(card);
            //удаляем карту в отбое
            Discard.erase(Discard.begin()+i);
        }
        //перемешиваем
        //Shuffle(Deck);
    }
    while (i < count && Deck.size()) {
        card = Deck[i];
        player.deck.push_back(card);
       // cout<<"Цдаляем.: "<<Deck[i].Color<<Deck[i].Value<<endl;
        Deck.erase(Deck.begin() + i);
        i++;
    }
    //return;
}
//Рисуем одну карту. Указываем верхний левый и правый нижний
void DrawCard(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, char color) {
    int c = 5;
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
// отрисовка карт
void DrawCards(int player, vector<Card> &v) {
    GLfloat x1 = -1.0f, x2 = x1 + wSide, y1 = -0.4f, y2 = -0.8f;
    if (v.size() < 20)
        dx = (2.0f - v.size() * wSide) / 2.0f;
    else dx = 0;
    x1 += dx;
    x2 += dx;
    GLfloat y = 0.0;   // счетчик ряда
    cout << endl << x1 << " " << x2 << "--->" << dx;

    if (player == 2) {
        y1 = 1;
        y2 = 0.6f;
    }

    for (int i = 0; i < v.size(); i++) {
        if (player == 1)
            DrawCard(x1, y1, x2, y2, v[i].Color);
        else
            DrawCard(x1, y1, x2, y2, 'D');
        v[i].x1 = x1;
        v[i].x2 = x2;
        v[i].y1 = y1;
        v[i].y2 = y2;

        if (y >= 1.0) {
            v[i - 20].y2 = -0.6f;
        }

        y = (i + 2) / 21.0f;
        x1 = x2;
        x2 += wSide;
        //если следующий ряд - обновляем координаты;
        if (y == 1.0) {
            x1 = -1.0f;
            x2 = x1 + wSide;
            if (player == 1) {
                y1 = -0.4f - dy * y;
                y2 = -0.8f - dy * y;
            } else {
                y1 = 1.0f - dy * y;
                y2 = 0.6f - dy * y;
            }
        }
    }
}