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

class PlayingCardSuit {
public:
	enum Suit { CLUBS, DIAMONDS, HEARTS, SPADES };
	Suit m_suit;
	bool m_suit_is_valid;

	//	fundamental relational operations
	bool operator< (const PlayingCardSuit &other) const {
		if (m_suit_is_valid && other.m_suit_is_valid) {
			return static_cast<int>(m_suit) < static_cast<int>(other.m_suit);
		} else {
			return false;
		}
	}
	bool operator==(const PlayingCardSuit &other) const {
		if (m_suit_is_valid && other.m_suit_is_valid) {
			return static_cast<int>(m_suit) == static_cast<int>(other.m_suit);
		} else {
			return false;
		}
	}
	//	derived relational operations
	bool operator<=(const PlayingCardSuit &other) const {
		return   *this < other || *this == other;
	}
	bool operator>=(const PlayingCardSuit &other) const {
		return !(*this < other);
	}
	bool operator> (const PlayingCardSuit &other) const {
		return !(*this < other) && !(*this == other);
	}
	bool operator!=(const PlayingCardSuit &other) const {
		return !(*this == other);
	}

	bool isValidSuit(char c) {
		c = toupper(c);
		switch (c) {
			case 'C':	return true;
			case 'D':	return true;
			case 'H':	return true;
			case 'S':	return true;
			default: break;
		}
		return false;
	}
	bool isValid() const {
		return m_suit_is_valid;
	}

	//	TODO - throw exception on bad 'c'
	Suit toSuit(char c) {
		c = toupper(c);
		if (isValidSuit(c)) {
			switch(c) {
			case 'C':	return CLUBS;
			case 'D':	return DIAMONDS;
			case 'H':	return HEARTS;
			case 'S':	return SPADES;
			default: break;
			}
		}
		return CLUBS;
	}

	std::string toString() const {
		if (m_suit_is_valid) {
			switch(this->m_suit) {
			case CLUBS:
				return std::string("CLUBS");
			case DIAMONDS:
				return std::string("DIAMONDS");
			case HEARTS:
				return std::string("HEARTS");
			case SPADES:
				return std::string("SPADES");
			default:
				break;
			}
		}
		return std::string("PlayingCardSuit in invalid state");
	}

	friend std::ostream& operator<<(std::ostream& out, const PlayingCardSuit &object) {
		out << object.toString();
		return out;
	}

	PlayingCardSuit() :	m_suit(CLUBS), m_suit_is_valid(false) {}
	~PlayingCardSuit() {}
	PlayingCardSuit(const PlayingCardSuit &other) : m_suit(other.m_suit), m_suit_is_valid(other.m_suit_is_valid) {}
	PlayingCardSuit& operator=(const PlayingCardSuit &other) {
		m_suit = other.m_suit;
		m_suit_is_valid = other.m_suit_is_valid;
		return *this;
	}
};

class PlayingCardRank {
public:
	enum Rank {
		TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT,
		NINE, TEN, JACK, QUEEN, KING, ACE
	};
	Rank m_rank;
	bool m_rank_is_valid;

	//	fundamental relational operators
	bool operator< (const PlayingCardRank &object) const {
		if (m_rank_is_valid && object.m_rank_is_valid) {
			return static_cast<int>(m_rank) <  static_cast<int>(object.m_rank);
		} else {
			return false;
		}
	}
	bool operator==(const PlayingCardRank &object) const {
		if (m_rank_is_valid && object.m_rank_is_valid) {
			return static_cast<int>(m_rank) == static_cast<int>(object.m_rank);
		} else {
			return false;
		}
	}
	//	derived relational operators
	bool operator<=(const PlayingCardRank &object) const {
		return *this < object || *this == object;
	}
	bool operator>=(const PlayingCardRank &object) const {
		return !(*this < object);
	}
	bool operator> (const PlayingCardRank &object) const {
		return !(*this < object) && !(*this == object);
	}
	bool operator!=(const PlayingCardRank &object) const {
		return !(*this == object);
	}
	bool isValidRank(char c) {
		c = toupper(c);
		switch(c) {
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'T':
		case 'J':
		case 'Q':
		case 'K':
		case 'A':
			return true;
		default:
			return false;
		}
	}
	bool isValid() const {
		return m_rank_is_valid;
	}

	//	TODO - throw exception on bad char
	Rank toRank(char c) {
		c = toupper(c);
		if (isValidRank(c)) {
			switch(c) {
			case '2':	return TWO;
			case '3':	return THREE;
			case '4':	return FOUR;
			case '5':	return FIVE;
			case '6':	return SIX;
			case '7':	return SEVEN;
			case '8':	return EIGHT;
			case '9':	return NINE;
			case 'T':	return TEN;
			case 'J':	return JACK;
			case 'Q':	return QUEEN;
			case 'K':	return KING;
			case 'A':	return ACE;
			default: 	break;
			}
		}
		return TWO;
	}

	std::string toString() const {
		if (m_rank_is_valid) {
			switch(m_rank) {
			case TWO: 	return std::string("2");
			case THREE: return std::string("3");
			case FOUR: 	return std::string("4");
			case FIVE: 	return std::string("5");
			case SIX: 	return std::string("6");
			case SEVEN: return std::string("7");
			case EIGHT: return std::string("8");
			case NINE: 	return std::string("9");
			case TEN: 	return std::string("T");
			case JACK: 	return std::string("J");
			case QUEEN: return std::string("Q");
			case KING: 	return std::string("K");
			case ACE: 	return std::string("A");
			default: break;
			}
		}
		return std::string("PlayingCardRank in invalid state");
	}

	friend std::ostream& operator<<(std::ostream& out, const PlayingCardRank &object) {
		out << object.toString();
		return out;
	}

	PlayingCardRank() : m_rank(TWO), m_rank_is_valid(false) {}
	~PlayingCardRank() {}
	// not worried about self assignment
	PlayingCardRank(const PlayingCardRank &other) : m_rank(other.m_rank), m_rank_is_valid(other.m_rank_is_valid) {}
	PlayingCardRank& operator=(const PlayingCardRank other) {
		m_rank 			= other.m_rank;
		m_rank_is_valid = other.m_rank_is_valid;
		return *this;
	}
};

// suit has priority over rank in the colating sequence
class PlayingCard {
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

	PlayingCard();
	virtual ~PlayingCard();
	PlayingCard(const PlayingCard &other);
	PlayingCard(const PlayingCardRank &rank, const PlayingCardSuit &suit);
	PlayingCard& operator=(const PlayingCard &other);

	friend std::ostream& operator<<(std::ostream& out, const PlayingCard &object) {
		out << object.toString();
		return out;
	}
};

#endif /* PLAYINGCARD_H_ */
