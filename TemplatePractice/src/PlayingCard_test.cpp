/*
 * PlayingCard_test.cpp
 *
 *  Created on: Jul 25, 2025
 *      Author: joe
 */

#include "PlayingCard.h"

bool testPlayingCard() {

	PlayingCardSuit suit = PlayingCardSuit::CLUBS;
	PlayingCardRank rank = PlayingCardRank::TWO;
	PlayingCard card(suit, rank);
	if (isValid(suit))
		std::cout << "suit is valid: " << suit << std::endl;
	if (isValid(rank))
		std::cout << "rank is valid: " << rank << std::endl;
	if (card.isValid())
		std::cout << "card is valid: " << card << std::endl;
	return true;
}
