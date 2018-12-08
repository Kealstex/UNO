//
// Created by kealstex on 06.12.18.
//

#include "Game.h"
bool IsRight(Card card){
    if(card.color==Discard.back().color || card.Value == Discard.back().Value
    || (card.color== 'A' && Discard.back().color!='A')){
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
            if(Player2.deck[i].color == 'A' && (Player2.deck[i].Value == 13 || Player2.deck[i].Value == 14)){
               switch(rand()%4){
                   case 0:
                       Player2.deck[i].color = 'R';
                       break;
                   case 1:
                       Player2.deck[i].color = 'G';
                       break;
                   case 2:
                       Player2.deck[i].color = 'B';
                       break;
                   case 3:
                       Player2.deck[i].color = 'Y';
                       break;
               }
            }
            return i;
        }
    }
 return -1;
}
void score(){
    Player1.score=0;
    for(int i=0;i<Player2.deck.size();i++){
        Player1.score += Player2.deck[i].Value;
    }
};