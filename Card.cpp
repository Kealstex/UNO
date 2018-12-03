//
// Created by Kealstex on 03.12.2018.
//

#include "Card.h"
void InitDeck() {
    //������ ���������� �����
    char color[5] = {'R', 'G', 'B', 'Y'};
    //�� ����� -> ��������
    for (int j = 0; j < 4; j++) {
        for (int i = 1; i < 26; i++) {
            //<10 value = score, >10 score = 20
            InitCard(color[j], i % 13, (i % 13 > 9) ? 20 : i % 13);
        }
        InitCard('A', 13, 50);
        InitCard('A', 14, 50);
    }
    // Shuffle(Deck);
    SortIsInHand(Deck);
    cout << Deck.size();
}
void InitCard(char color, int value, int score) {
    Card card;
    card.Color = color;
    card.Value = value;
    card.Score = score;
    Deck.push_back(card);
}
// ���������� ������
void Shuffle(vector<Card> &v) {
    srand(unsigned(time(nullptr)));
    random_shuffle(v.begin(), v.end());
}

void SortIsInHand(vector<Card> &v) {
    for (int i = 0; i < v.size(); i++)
        for (int j = i + 1; j < v.size(); j++)
            if (v[i].Color > v[j].Color) {
                Card temp = v[i];
                v[i] = v[j];
                v[j] = temp;
            }
}
// count ���� � ���� ������ player
vector<Card> RandomDeck(int count, Player &player) {
    vector<Card> v;
    int temp, i = 0;
    Card card;
    srand((unsigned) time(nullptr));

    //�������� �� ������� ������� ���������� ����
    if (count >= Deck.size()) {
        for (int i = 0; i < Discard.size(); i++) {
            card = Discard[i];
            //�������� ����� �� ������ ����� � ����� ������
            Deck.push_back(card);
            //������� ����� �� ������ ������
            Discard.erase(Discard.begin()+i);
        }
        Shuffle(Deck);
    }
    while (i < count) {
        temp = rand() % Deck.size();
        card = Deck[temp];
        player.deck.push_back(card);
        cout<<"������: "<<Deck[temp].Color<<Deck[temp].Value<<endl;
        Deck.erase(Deck.begin() + temp );
        i++;
    }
    return v;
}