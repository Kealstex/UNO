//
// Created by Kealstex on 03.12.2018.
//

#include "Card.h"

void InitDeck() {
    //по цвету
    char color[4] = {'R', 'G', 'B', 'Y'};
    //по цвету -> по номиналу
    for (int j = 0; j < 4; j++) {
        for (int i = 1; i < 26; i++) {
            //<10 value = score, >10 score = 20


            InitCard(color[j], i % 13, (i % 13 > 9) ? 20 : i % 13);
        }
        InitCard('A', 13, 50);
        InitCard('A', 14, 50);
    }
    Shuffle(Deck);
}

void InitCard(char color, int value, int score) {
    Card card;
    card.color = color;
    card.Value = value;
    card.Score = score;
    Deck.push_back(card);
}
//очищаем колоды
void DeckClear() {
    Deck.clear();
    Discard.clear();
    Player1.deck.clear();
    Player2.deck.clear();
}
// тусовка карт
void Shuffle(vector<Card> &v) {
    srand(unsigned(time(nullptr)));
    // не считаем активную карту
    random_shuffle(v.begin() + 1, v.end());
}

void SortIsInHand(vector<Card> &v) {
    for (int i = 0; i < v.size(); i++)
        for (int j = i + 1; j < v.size() - 1; j++)
            if (v[i].color > v[j].color) {
                Card temp = v[i];
                v[i] = v[j];
                v[j] = temp;
            }
}

// count-card push in player
void more(int count, Player &player) {
    int i = 0;
    Card card;
    if (Deck.size() < count) {
        while (Discard.size() != 1) {
            if (Discard[0].color!='A' && (Discard[0].Value == 13 || Discard[0].Value == 14)){
                Discard[0].color = 'A';
            }
            Deck.push_back(Discard[0]);
            Discard.erase(Discard.begin());
        }
        Shuffle(Deck);
    }
    while (i < count && Deck.size()) {
        card = Deck.back();
        player.deck.push_back(card);
        Deck.pop_back();
        i++;
    }
}

//Рисуем одну карту. Указываем верхний левый и правый нижний
void DrawCard(Card card) {
    int color = 0, value;
    switch (card.color) {
        case 'R':
            color = 0;
            break;
        case 'G':
            color = 1;
            break;
        case 'B':
            color = 2;
            break;
        case 'Y':
            color = 3;
            break;
        case 'A':
            color = 4;
            break;
    }
    value = card.Value;
    glBindTexture(GL_TEXTURE_2D, textures[color][value]);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex3f(card.x1, card.y2, 0);
    glTexCoord2d(1.0, 0.0);
    glVertex3f(card.x2, card.y2, 0);
    glTexCoord2d(1.0, 1.0);
    glVertex3f(card.x2, card.y1, 0);
    glTexCoord2d(0.0, 1.0);
    glVertex3f(card.x1, card.y1, 0);

    glEnd();
}

// отрисовка карты
void DrawCards(int player, Player &players) {
    GLfloat x1 = -1.0f, x2 = x1 + wSide, y1 = -hSide, y2 = y1 - hSide;
    GLfloat y = 0.0;   // счетчик ряда
    if (players.deck.size() < 20) {
        dx = (2.0f - players.deck.size() * wSide) / 2.0f;
    } else dx = 0;
    x1 += dx;
    x2 += dx;
    if (player == 2) {
        y1 = 1;
        y2 = 1 - hSide;
    }
    for (int i = 0; i < players.deck.size(); i++) {
        players.deck[i].x1 = x1;
        players.deck[i].x2 = x2;
        players.deck[i].y1 = y1;
        players.deck[i].y2 = y2;
        //TODO отрисовка рубашки боту
        if (player == 1) {
            DrawCard(players.deck[i]);}
        else{
            Card card = players.deck[i];
            card.color = 'A';
            card.Value = 0;
            DrawCard(card);}
        //узнает ряд,если =1, то второй
        if (y >= 1.0) {
            players.deck[i - 20].y2 = -0.6f;
        }

        y = (i + 2) / 21.0f;
        x1 = x2;
        x2 += wSide;
        //если следующий ряд - обновляем координаты;
        if (y == 1.0) {
            x1 = -1.0f;
            x2 = x1 + wSide;
            if (player == 1) {
                y1 = -hSide - dy * y;
                y2 = -2 * hSide - dy * y;
            } else {
                y1 = 1.0f - dy * y;
                y2 = 1.0 - hSide - dy * y;
            }
        }
    }
}

void Activity() {
    Card card;
    int i = 1;
    //запомнаем последнюю

    do {
        card = Deck[Deck.size() - i];
        i++;
    } while (card.color == 'A');
    Discard.push_back(card);
    Deck.pop_back();
}

void DrawActivity() {
    if(!Discard.size()){
        return;
    }
    Card card = Discard.back();
    card.x1 = -wSide / 2.0;
    card.x2 = card.x1 + wSide;
    card.y1 = hSide / 2.0;
    card.y2 = card.y1 - hSide;
    DrawCard(card);
}

void DrawDeck(char color) {
    Card card = Deck[Deck.size() -1];
    card.x1 = -wSide / 2.0 - 9 * wSide;
    card.x2 = card.x1 + wSide;
    card.y1 = hSide / 2.0;
    card.y2 = card.y1 - hSide;
    //если нужно отрисовать рубашкой,иначе рисует своим цветом
    if(color=='A'){
        card.color = color;
        card.Value = 0;
    }
    DrawCard(card);
    renderBitmapString(-wSide / 2.0 - 9 * wSide, hSide / 2.0 + 0.05, 0, GLUT_BITMAP_TIMES_ROMAN_24,
                       string("Deck size:") + to_string(Deck.size()));
}
void renderBitmapString(float x, float y, float z, void *font, string String)
{
    glRasterPos3f(x, y, z);
    for (int i = 0; i < String.size(); i++) {
        glutBitmapCharacter(font, String[i]);
    }
}
//рисуем кнопку
void DrawButton(){
    Card card;
    card.color='A';
    card.Value=4;
    card.x1 = 0.5f,
    card.x2 = 0.5f +wSide;
    card.y1 = wSide;
    card.y2 = 0;
    //стрелка вверх
    DrawCard(card);
    // стрелка вниз
    card.y2 = 0;
    card.y1 = -wSide;
    DrawCard(card);
}
void DrawBackground(int color){
    Card card;
    card.x1 = -1;
    card.x2 = 1;
    card.y1 = 1;
    card.y2 = -1;
    card.color = 'A';
    card.Value = color;
    DrawCard(card);
    renderBitmapString(-wSide/2.0-9*wSide, hSide/2.0+0.05,0, GLUT_BITMAP_TIMES_ROMAN_24, string("Deck size:") + to_string(Deck.size()));
}
void DrawChoseColor(){
    Card chose;
    chose.x1 = 0.3f;
    chose.x2 = 0.3f + 4*wSide;
    chose.y1 = wSide;
    chose.y2 = -wSide;
    chose.color = 'A';
    chose.Value = 12;
    DrawCard(chose);
}