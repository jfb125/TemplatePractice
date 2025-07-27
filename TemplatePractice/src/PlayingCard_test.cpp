/*
 * PlayingCard_test.cpp
 *
 *  Created on: Jul 25, 2025
 *      Author: joe
 */

#include "PlayingCard.h"

void displayDeck(PlayingCard deck[]) {
	int i = 0;
	for (int row = 0; row != 4; row++) {
		for (int col = 0; col != 13; col++) {
			std::cout << deck[i++] << "  ";
		}
		std::cout << std::endl;
	}
}

bool testPlayingCard() {

	PlayingCard deck[52];
	int i = 0;
	PlayingCardSuit suit = PlayingCardSuit::CLUBS;
	do {
		PlayingCardRank rank = PlayingCardRank::TWO;
		do {
			deck[i++] = *(new PlayingCard(suit, rank));
			if (rank == PlayingCardRank::ACE)
				break;
			++rank;
		} while(1);
		if (suit == PlayingCardSuit::SPADES)
			break;
		++suit;
	} while (1);

	std::cout << " **** PRIOR TO SHUFFLING **** " << std::endl;
	displayDeck(deck);
	std::cout << std::endl;
	shuffle(deck, 52);
	std::cout << " **** AFTER SHUFFLING **** " << std::endl;
	displayDeck(deck);
	std::cout << std::endl;
	sort(deck, 52);
	std::cout << " **** AFTER SORTING ****" << std::endl;
	displayDeck(deck);
	std::cout << std::endl;
	return true;
}
