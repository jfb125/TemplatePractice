/*
 * PlayingCard.h
 *
 *  Created on: Jul 25, 2025
 *      Author: joe
 */

#ifndef PLAYINGCARD_H_
#define PLAYINGCARD_H_

#include <iostream>
#include <ostream>
#include <iomanip>
#include <string>
#include <cstring>

constexpr int INVALID_PLAYING_CARD_SUIT_VALUE = -1;
enum class PlayingCardSuit { CLUBS, DIAMONDS, HEARTS, SPADES };
bool isValid(PlayingCardSuit);
int toInt(PlayingCardSuit);
bool operator< (PlayingCardSuit u, PlayingCardSuit v);
bool operator==(PlayingCardSuit u, PlayingCardSuit v);
bool operator<=(PlayingCardSuit u, PlayingCardSuit v);
bool operator> (PlayingCardSuit u, PlayingCardSuit v);
bool operator>=(PlayingCardSuit u, PlayingCardSuit v);
bool operator!=(PlayingCardSuit u, PlayingCardSuit v);
std::string toString(PlayingCardSuit);
std::ostream& operator<<(std::ostream &out, PlayingCardSuit);

constexpr int INVALID_PLAYING_CARD_RANK_VALUE = -1;
enum class PlayingCardRank {
		TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT,
		NINE, TEN, JACK, QUEEN, KING, ACE
};
bool isValid(PlayingCardRank);
int toInt(PlayingCardRank);
bool operator< (PlayingCardRank u, PlayingCardRank v);
bool operator==(PlayingCardRank u, PlayingCardRank v);
bool operator<=(PlayingCardRank u, PlayingCardRank v);
bool operator> (PlayingCardRank u, PlayingCardRank v);
bool operator>=(PlayingCardRank u, PlayingCardRank v);
bool operator!=(PlayingCardRank u, PlayingCardRank v);
std::string toString(PlayingCardRank);
std::ostream& operator<<(std::ostream &out, PlayingCardRank);

// suit has priority over rank in the colating sequence
class PlayingCard {
private:
	PlayingCard();
public:
	PlayingCardSuit m_suit;
	PlayingCardRank m_rank;

	bool operator< (const PlayingCard &other) const;
	bool operator==(const PlayingCard &other) const;
	bool operator>=(const PlayingCard &other) const;
	bool operator> (const PlayingCard &other) const;
	bool operator<=(const PlayingCard &other) const;
	bool operator!=(const PlayingCard &other) const;

	int compare(const PlayingCard &other) const;
	bool isValid() const;
	std::string toString() const;

	virtual ~PlayingCard();
	PlayingCard(const PlayingCard &other);
	PlayingCard(const PlayingCardSuit &suit, const PlayingCardRank &rank);
	PlayingCard(const PlayingCardRank &rank, const PlayingCardSuit &suit);
	PlayingCard& operator=(const PlayingCard &other);

	friend std::ostream& operator<<(std::ostream& out, const PlayingCard &object) {
		out << object.toString();
		return out;
	}
};

void sort(PlayingCard **, int size);
void shuffle(PlayingCard **, int size);

#endif /* PLAYINGCARD_H_ */
