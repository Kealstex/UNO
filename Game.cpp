//
// Created by kealstex on 06.12.18.
//

#include "Game.h"
bool IsRight(Card card){
    if(card.color==Discard[Discard.size()-1].color || card.Value == Discard[Discard.size()-1].Value){
        return true;
    }
    return false;
}
bool PushInDiscard(int i, Player &player)
{
    if (i<0)
        return false;
    Card card;
    card = player.deck[i];
    //кладем в отбой
    Discard.push_back(card);
    //удаляем карту в отбое
    player.deck.erase(player.deck.begin() + i);
    cout << "Player " << player.PNumber << " Push card " << Discard.back().color << " " << Discard.back().Value << endl;
    return true;
}
int Player2Chose(){
    for (int i = 0; i < Player2.deck.size(); i++) {
        if (IsRight(Player2.deck[i])) {
            return i;
        }
    }
 return -1;
}
