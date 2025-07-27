/*
 * PlayingCard_test.cpp
 *
 *  Created on: Jul 25, 2025
 *      Author: joe
 */

#include "PlayingCard.h"

bool isSorted(PlayingCard *deck, int size) {

	for (int i = 1; i != size; i++) {
		if (deck[i] < deck[i-1])
			return false;
	}
	return true;
}

void displayDeck(PlayingCard *deck, int size) {

	constexpr int suits_in_deck = 4;
	constexpr int ranks_in_suit = 13;
	int i = 0;
	for (int row = 0; row != suits_in_deck; row++) {
		for (int col = 0; col != ranks_in_suit; col++) {
			std::cout << deck[i++] << "  ";
			if (i == size)
				break;
		}
		std::cout << std::endl;
	}
}

bool testPlayingCard() {

	constexpr int cards_in_deck = 52;
	PlayingCard deck[cards_in_deck];

	int i = 0;
	PlayingCardSuit suit = PlayingCardSuit::CLUBS;
	do {
		PlayingCardRank rank = PlayingCardRank::TWO;
		do {
			deck[i++] = std::move(PlayingCard(suit, rank));
//			deck[i++](suit,rank); // = PlayingCard(suit, rank);
			if (rank == PlayingCardRank::ACE)
				break;
			++rank;
		} while(1);
		if (suit == PlayingCardSuit::SPADES)
			break;
		++suit;
	} while (1);

	std::cout << " **** PRIOR TO SHUFFLING ****, deck is "
			  << (isSorted(deck, cards_in_deck) ? "sorted" : "not sorted") << std::endl;
	displayDeck(deck, cards_in_deck);
	std::cout << std::endl;
	shuffle(deck, cards_in_deck);
	std::cout << " ****  AFTER SHUFFLING   ****, deck is "
			  << (isSorted(deck, cards_in_deck) ? "sorted" : "not sorted") << std::endl;
	displayDeck(deck, cards_in_deck);
	std::cout << std::endl;
	sort(deck, cards_in_deck);
	std::cout << " ****   AFTER SORTING   **** , deck is "
			  << (isSorted(deck, cards_in_deck) ? "sorted" : "not sorted") << std::endl;
	displayDeck(deck, cards_in_deck);
	std::cout << std::endl;
	return true;
}
