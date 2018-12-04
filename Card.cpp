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
        Shuffle(Deck);
    }
    while (i < count && i < Deck.size()) {
        card = Deck[i];
        player.deck.push_back(card);
        cout<<"Цдаляем.: "<<Deck[i].Color<<Deck[i].Value<<endl;
        Deck.erase(Deck.begin() + i);
        i++;
    }
    //return;
}