//
// Created by kealstex on 06.12.18.
//

#ifndef UNTITLED1_GAME_H
#define UNTITLED1_GAME_H
#include "Card.h"
bool IsRight(Card card);
int Player2Chose();                             //возвращает индекс подходящей карты,иначе -1
bool PushInDiscard(int i, Player &player);      // кладет карту с индексом i игрока player в отбой
#endif //UNTITLED1_GAME_H
