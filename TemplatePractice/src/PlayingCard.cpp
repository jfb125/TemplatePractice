/*
 * PlayingCard.cpp
 *
 *  Created on: Jul 25, 2025
 *      Author: joe
 */

#include "PlayingCard.h"

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
	result << std::setw(2) << m_rank << " of " << std::setw(6) << std::left << m_suit;
	return result.str();
}

bool PlayingCard::isValid() const {
	return m_rank.isValid() && m_suit.isValid();
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
	return *this != other;
}

PlayingCard::PlayingCard() {}
PlayingCard::~PlayingCard() {}

PlayingCard::PlayingCard(const PlayingCardRank &rank, const PlayingCardSuit &suit) : m_suit(suit), m_rank(rank) {}
PlayingCard::PlayingCard(const PlayingCard &other) : m_suit(other.m_suit), m_rank(other.m_rank) {}

// this is so trivial that I won't implement the self copy check
PlayingCard& PlayingCard::operator=(const PlayingCard &other) {
	m_suit = other.m_suit;
	m_rank = other.m_rank;
	return *this;
}

