/*
 * PlayingCard.cpp
 *
 *  Created on: Jul 25, 2025
 *      Author: joe
 */

#include "PlayingCard.h"

/*	******************************************************************	*/
/*	******************************************************************	*/
/*						enum class PlayingCardSuit						*/
/*	******************************************************************	*/
/*	******************************************************************	*/

//enum class PlayingCardSuit { CLUBS, DIAMONDS, HEARTS, SPADES };
//constexpr int INVALID_PLAYING_CARD_SUIT_VALUE = -1;
bool isValid(PlayingCardSuit suit) {
	switch(suit) {
	case PlayingCardSuit::CLUBS:
	case PlayingCardSuit::DIAMONDS:
	case PlayingCardSuit::HEARTS:
	case PlayingCardSuit::SPADES:
		return true;
	default:
		return false;
	}
}
int toInt(PlayingCardSuit suit) {
	if (isValid(suit)) {
		return static_cast<int>(suit);
	} else {
		return INVALID_PLAYING_CARD_SUIT_VALUE;
	}
}
//	fundamental relational operators
bool operator< (PlayingCardSuit u, PlayingCardSuit v) {
	if (isValid(u) && isValid(v)) {
		return static_cast<int>(u) < static_cast<int>(v);
	} else {
		return false;
	}
}
bool operator==(PlayingCardSuit u, PlayingCardSuit v) {
	if (isValid(u) && isValid(v)) {
		return static_cast<int>(u) == static_cast<int>(v);
	} else {
		return false;
	}
}
bool operator<=(PlayingCardSuit u, PlayingCardSuit v) {
	return u < v || u == v;
}
bool operator> (PlayingCardSuit u, PlayingCardSuit v) {
	return !(u < v || u == v);
}
bool operator>=(PlayingCardSuit u, PlayingCardSuit v) {
	return !(u < v);
}
bool operator!=(PlayingCardSuit u, PlayingCardSuit v) {
	return !(u == v);
}
PlayingCardSuit& operator++(PlayingCardSuit &suit) {
	if (isValid(suit)) {
		switch(suit) {
		case PlayingCardSuit::CLUBS:
			suit = PlayingCardSuit::DIAMONDS;
			break;
		case PlayingCardSuit::DIAMONDS:
			suit = PlayingCardSuit::HEARTS;
			break;
		case PlayingCardSuit::HEARTS:
			suit = PlayingCardSuit::SPADES;
			break;
		case PlayingCardSuit::SPADES:
			suit = PlayingCardSuit::CLUBS;
			break;
		case PlayingCardSuit::INVALID:
		default:
			suit = PlayingCardSuit::INVALID;
		}
	} else {
		suit = PlayingCardSuit::INVALID;
	}
	return suit;
}
std::string toString(PlayingCardSuit suit) {
	if (isValid(suit)) {
		switch(suit) {
		case PlayingCardSuit::CLUBS:
			return std::string("C");
		case PlayingCardSuit::HEARTS:
			return std::string("H");
		case PlayingCardSuit::DIAMONDS:
			return std::string("D");
		case PlayingCardSuit::SPADES:
			return std::string("S");
		default:
			break;
		}
	}
	return std::string("toString(PlayingCardSuit) returns Invalid PlayingCardSuit");
}
std::ostream& operator<<(std::ostream &out, PlayingCardSuit suit) {
	out << toString(suit);
	return out;
}


/*	******************************************************************	*/
/*	******************************************************************	*/
/*						enum class PlayingCardRank						*/
/*	******************************************************************	*/
/*	******************************************************************	*/

//constexpr int INVALID_PLAYING_CARD_SUIT_VALUE = -1;
//enum class PlayingCardRank {
//		TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT,
//		NINE, TEN, JACK, QUEEN, KING, ACE
//};
bool isValid(PlayingCardRank rank) {
	switch(rank) {
	case PlayingCardRank::TWO:
	case PlayingCardRank::THREE:
	case PlayingCardRank::FOUR:
	case PlayingCardRank::FIVE:
	case PlayingCardRank::SIX:
	case PlayingCardRank::SEVEN:
	case PlayingCardRank::EIGHT:
	case PlayingCardRank::NINE:
	case PlayingCardRank::TEN:
	case PlayingCardRank::JACK:
	case PlayingCardRank::QUEEN:
	case PlayingCardRank::KING:
	case PlayingCardRank::ACE:
		return true;
	default:
		break;
	}
	return false;
}
int toInt(PlayingCardRank rank) {
	if (isValid(rank)) {
		return static_cast<int>(rank);
	} else {
		return INVALID_PLAYING_CARD_RANK_VALUE;
	}
}
//	fundamental relational operators
bool operator< (PlayingCardRank u, PlayingCardRank v) {
	if (isValid(u) && isValid(v)) {
		return static_cast<int>(u) < static_cast<int>(v);
	} else {
		return false;
	}
}
bool operator==(PlayingCardRank u, PlayingCardRank v) {
	if (isValid(u) && isValid(v)) {
		return static_cast<int>(u) == static_cast<int>(v);
	} else {
		return false;
	}
}
//	derived relational operators
bool operator<=(PlayingCardRank u, PlayingCardRank v) {
	return u < v || u == v;
}
bool operator> (PlayingCardRank u, PlayingCardRank v) {
	return !(u < v) && !(u == v);
}
bool operator>=(PlayingCardRank u, PlayingCardRank v) {
	return !(u < v);
}
bool operator!=(PlayingCardRank u, PlayingCardRank v) {
	return !(u == v);
}
PlayingCardRank& operator++(PlayingCardRank &rank) {
	using _Rank = PlayingCardRank;
	if (isValid(rank)) {
		switch(rank) {
		case PlayingCardRank::TWO:
			rank = _Rank::THREE;
			break;
		case PlayingCardRank::THREE:
			rank = _Rank::FOUR;
			break;
		case PlayingCardRank::FOUR:
			rank = _Rank::FIVE;
			break;
		case PlayingCardRank::FIVE:
			rank = _Rank::SIX;
			break;
		case PlayingCardRank::SIX:
			rank = _Rank::SEVEN;
			break;
		case PlayingCardRank::SEVEN:
			rank = _Rank::EIGHT;
			break;
		case PlayingCardRank::EIGHT:
			rank = _Rank::NINE;
			break;
		case PlayingCardRank::NINE:
			rank = _Rank::TEN;
			break;
		case PlayingCardRank::TEN:
			rank = _Rank::JACK;
			break;
		case PlayingCardRank::JACK:
			rank = _Rank::QUEEN;
			break;
		case PlayingCardRank::QUEEN:
			rank = _Rank::KING;
			break;
		case PlayingCardRank::KING:
			rank = _Rank::ACE;
			break;
		case PlayingCardRank::ACE:
			rank = _Rank::TWO;
			break;
		default:
			rank = _Rank::INVALID;
			break;
		}
	} else {
		rank = PlayingCardRank::INVALID;
	}
	return rank;
}
std::string toString(PlayingCardRank rank) {
	if (isValid(rank)) {
		switch(rank)
		{
		case PlayingCardRank::TWO:		return std::string("2");
		case PlayingCardRank::THREE:	return std::string("3");
		case PlayingCardRank::FOUR:		return std::string("4");
		case PlayingCardRank::FIVE:		return std::string("5");
		case PlayingCardRank::SIX:		return std::string("6");
		case PlayingCardRank::SEVEN:	return std::string("7");
		case PlayingCardRank::EIGHT:	return std::string("8");
		case PlayingCardRank::NINE:		return std::string("9");
		case PlayingCardRank::TEN:		return std::string("T");
		case PlayingCardRank::JACK:		return std::string("J");
		case PlayingCardRank::QUEEN:	return std::string("Q");
		case PlayingCardRank::KING:		return std::string("K");
		case PlayingCardRank::ACE:		return std::string("A");
		default:
			break;
		}
	}
	return std::string("toString(PlayingCardRank) returns Invalid PlayingCardRank");
}
std::ostream& operator<<(std::ostream &out, PlayingCardRank rank) {
	out << toString(rank);
	return out;
}

//	this is necessary to fix a namespace collision with 'PlayingCard::isValid(void)'
bool isCardValid(PlayingCard card) {
	return isValid(card.m_rank) && isValid(card.m_suit);
}

/*	******************************************************************	*/
/*	******************************************************************	*/
/*							class PlayingCard							*/
/*	******************************************************************	*/
/*	******************************************************************	*/

//	TODO - throw on either one being in invalid state
int PlayingCard::compare(const PlayingCard &other) const {
	if (isValid() && other.isValid()) {
		if (*this > other)  return  1;
		if (*this < other)	return -1;
		return 0;
	} else {
		return 0;
	}
}

std::string PlayingCard::toString() const {
	std::stringstream result;
	result << m_rank << " of " << m_suit;
	return result.str();
}

bool PlayingCard::isValid(void) const {
	return isCardValid(*this);
}

//	fundamental relational operators
bool PlayingCard::operator< (const PlayingCard &other) const {
	if (isValid() && other.isValid()) {
		if (m_suit < other.m_suit) {
			return true;
		}
		if (m_suit > other.m_suit) {
			return false;
		}
		return m_rank < other.m_rank;
	}
	return false;
}
bool PlayingCard::operator==(const PlayingCard &other) const {
	if (isValid() && other.isValid()) {
		return m_suit == other.m_suit && m_rank == other.m_rank;
	} else {
		return false;
	}
}
//	derived relational operators
bool PlayingCard::operator>=(const PlayingCard &other) const {
	return !(*this < other);
}
bool PlayingCard::operator> (const PlayingCard &other) const {
	return !(*this < other || *this == other);
}
bool PlayingCard::operator<=(const PlayingCard &other) const {
	return (*this < other) || (*this == other);
}
bool PlayingCard::operator!=(const PlayingCard &other) const {
	return !(*this == other);
}

//PlayingCard::PlayingCard() {}
PlayingCard::~PlayingCard() {}
PlayingCard::PlayingCard() : m_suit(PlayingCardSuit::INVALID), m_rank(PlayingCardRank::INVALID) {}
PlayingCard::PlayingCard(const PlayingCard &other) : m_suit(other.m_suit), m_rank(other.m_rank) {}
PlayingCard::PlayingCard(const PlayingCardSuit &suit, const PlayingCardRank &rank) : m_suit(suit), m_rank(rank) {}
PlayingCard::PlayingCard(const PlayingCardRank &rank, const PlayingCardSuit &suit) : m_suit(suit), m_rank(rank) {}
// this is so trivial that I won't implement the self copy check
PlayingCard& PlayingCard::operator=(const PlayingCard &other) {
	m_suit = other.m_suit;
	m_rank = other.m_rank;
	return *this;
}

void sort(PlayingCard **array, int size) {
	return;
}
void shuffle(PlayingCard **array, int size) {
	return;
}
