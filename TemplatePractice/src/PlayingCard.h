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

#include "ComparableObject.h"

class PlayingCardSuit {
	enum Suit { CLUBS, DIAMONDS, HEARTS, SPADES };
	Suit suit;

	//	fundamental relational operations
	bool operator< (const PlayingCardSuit &other) {
		return suit < other.suit;
	}
	bool operator==(const Playing
		return static_cast<int>(u) == static_cast<int>(v);
	}
};

//	fundamental relational operations
bool operator< (PlayingCardSuit u, PlayingCardSuit v) {
	return static_cast<int>(u) <  static_cast<int>(v);
}
bool operator==(PlayingCardSuit u, PlayingCardSuit v) {
	return static_cast<int>(u) == static_cast<int>(v);
}
//	derived relational operations
bool operator<=(PlayingCardSuit u, PlayingCardSuit v) {
	return   (u <  v) || (u == v);
}
bool operator>=(PlayingCardSuit u, PlayingCardSuit v) {
	return ! (u <  v);
}
bool operator> (PlayingCardSuit u, PlayingCardSuit v) {
	return !((u <  v) || (u == v));
}
bool operator!=(PlayingCardSuit u, PlayingCardSuit v) {
	return !(u == v);
}
std::ostream operator<<(std::ostream out, const PlayingCardSuit object) {
	using Suit = PlayingCardSuit;
	switch(object) {
	case Suit::CLUBS:
		out << "Clubs";
		break;
	case Suit::DIAMONDS:
		out << "Diamonds";
		break;
	case Suit::HEARTS:
		out << "Hearts";
		break;
	case Suit::SPADES:
		out << "Spades";
		break;
	}
	return out;
}

class PlayingCardRank {
	enum Rank {
		TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT,
		NINE, TEN, JACK, QUEEN, KING, ACE
	};
	Rank rank;

	//	fundamental relational operators
	bool operator< (const PlayingCardRank &object) {
		return static_cast<int>(rank) <  static_cast<int>(v.rank);
	}
	bool operator==(PlayingCardRank u, PlayingCardRank v) {
		return static_cast<int>(u) == static_cast<int>(v);
	}
	//	derived relational operators
	bool operator<=(PlayingCardRank u, PlayingCardRank v) {
		return   (u < v) || (u == v);
	}
bool operator> (PlayingCardRank u, PlayingCardRank v) {
	return !((u < v) || (u==v));
}
bool operator>=(PlayingCardRank u, PlayingCardRank v) {
	return !(u < v);
}
bool operator!=(PlayingCardRank u, PlayingCardRank v) {
	return !(u == v);
}
	std::ostream operator<<(std::ostream out, const PlayingCardRank object) {
		switch(object) {
		case Rank::TWO:
		out << '2';
		break;
	case Rank::THREE:
		out << '3';
		break;
	case Rank::FOUR:
		out << '4';
		break;
	case Rank::FIVE:
		out << '5';
		break;
	case Rank::SIX:
		out << '6';
		break;
	case Rank::SEVEN:
		out << '7';
		break;
	case Rank::EIGHT:
		out << '8';
		break;
	case Rank::NINE:
		out << '9';
		break;
	case Rank::TEN:
		out << 'T';
		break;
	case Rank::JACK:
		out << 'J';
		break;
	case Rank::QUEEN:
		out << 'Q';
		break;
	case Rank::KING:
		out << 'K';
		break;
	case Rank::ACE:
		out << 'A';
		break;
	default:
		out << '?';
	}
	return out;
}
}
#define DEFAULT_PLAYING_CARD_SUIT PlayingCardSuit::CLUBS
#define DEFAULT_PLAYING_CARD_RANK PlayingCardRank::TWO

// suit has priority over rank in the colating sequence
class PlayingCard: public ComparableObject {
public:
	PlayingCardSuit m_suit;
	PlayingCardRank m_rank;

	bool operator< (const PlayingCard &other) const;
	bool operator==(const PlayingCard &other) const;
	bool operator>=(const PlayingCard &other) const;
	bool operator> (const PlayingCard &other) const;
	bool operator<=(const PlayingCard &other) const;
	bool operator!=(const PlayingCard &other) const;

	bool operator< (const ComparableObject &other) const;
	bool operator==(const ComparableObject &other) const;
	bool operator>=(const ComparableObject &other) const;
	bool operator> (const ComparableObject &other) const;
	bool operator<=(const ComparableObject &other) const;
	bool operator!=(const ComparableObject &other) const;
	int compare(const ComparableObject &u, const ComparableObject &v);

	PlayingCard();
	virtual ~PlayingCard();
	PlayingCard(const PlayingCard &other);
	PlayingCard& operator=(const PlayingCard &other);
	PlayingCard(PlayingCard &&other);
	PlayingCard& operator=(PlayingCard &&other);

	std::ostream operator<<(std::ostream out, const PlayingCard &object) {
		out << std::setw(2) << object.m_rank << " of " << std::setw(6) << std::left << object.m_suit;
		return out;
	}
};

#endif /* PLAYINGCARD_H_ */
