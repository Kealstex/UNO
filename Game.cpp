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
void PushInDiscard(int i, Player &player)
{
    Card card;
    card = player.deck[i];
    //кладем в отбой
    Discard.push_back(card);
    //удаляем карту в отбое
    player.deck.erase(player.deck.begin() + i);
}
int Player2Chose(){
    Card card;
    int i=-1;
    do{
        i++;
        card = Player2.deck[i];
        if(IsRight(card)){
            return i;
        }
    }while(i < Player2.deck.size());
 return -1;
}
