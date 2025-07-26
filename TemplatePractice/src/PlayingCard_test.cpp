/*
 * PlayingCard_test.cpp
 *
 *  Created on: Jul 25, 2025
 *      Author: joe
 */

#include "PlayingCard.h"

bool testPlayingCard() {

	PlayingCard oneCard;
	oneCard.m_rank = *(new PlayingCardRank(PlayingCardRank::Rank::ACE));
	oneCard.m_suit = *(new PlayingCardSuit(PlayingCardSuit::Suit::SPADES));
	std::cout << oneCard << std::endl;
	return true;
}
