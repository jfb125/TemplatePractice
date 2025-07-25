/*
 * PlayingCard_test.cpp
 *
 *  Created on: Jul 25, 2025
 *      Author: joe
 */

#include "PlayingCard.h"

bool testPlayingCard() {

	PlayingCard oneCard;
	oneCard.m_rank = PlayingCardRank::ACE;
	oneCard.m_suit = PlayingCardSuit::SPADES;

	std::cout << oneCard << std::endl;
	return true;
}
