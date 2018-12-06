//
// Created by kealstex on 06.12.18.
//

#include "Game.h"
bool IsRight(Card card){
    if(card.Color==Discard[Discard.size()-1].Color){
        return true;
    }
    return false;
}
void Push(int i, Player &player)
{
    Card card;
    card = player.deck[i];
    //кладем в отбой
    Discard.push_back(card);
    //удаляем карту в отбое
    player.deck.erase(player.deck.begin() + i);
}
