/*
 * PlayingCard_test.cpp
 *
 *  Created on: Jul 25, 2025
 *      Author: joe
 */

#include "PlayingCard.h"

bool testPlayingCard() {


	PlayingCard *deck[52];
	int i = 0;
	for (PlayingCardSuit suit = PlayingCardSuit::CLUBS;  ; ++suit) {
		for (PlayingCardRank rank = PlayingCardRank::TWO; ; ++rank) {
			deck[i++] = new PlayingCard(suit, rank);
			if (rank == PlayingCardRank::ACE)
				break;
		}
		if (suit == PlayingCardSuit::SPADES)
			break;
	}


	for (int j = 0; j != 13; j++) {
		for (int k = 0; k != 4; k++) {
			std::cout << "(" << k << ", " << j << "): " << deck[13*k+j]->toString() << ", ";
		}
		std::cout << std::endl;
	}
	return true;
}
