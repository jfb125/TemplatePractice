/*
 * PlayingCard.cpp
 *
 *  Created on: Jul 25, 2025
 *      Author: joe
 */

#include "PlayingCard.h"

//	fundamental relational operators
bool PlayingCard::operator< (const PlayingCard &other) const {
	if (m_suit < other.m_suit) {
		return true;
	}
	if (m_suit > other.m_suit) {
		return false;
	}
	return m_rank < other.m_rank;
}
bool PlayingCard::operator==(const PlayingCard &other) const {
	return m_suit == other.m_suit && m_rank == other.m_rank;
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

int PlayingCard::compare(const ComparableObject &u, const ComparableObject &v) {
	if (static_cast<PlayingCard>(u) < static_cast<PlayingCard>(v)) return -1;
	if (static_cast<PlayingCard>(u) > static_cast<PlayingCard>(v)) return  1;
	return 0;
}

bool PlayingCard::operator< (const ComparableObject &other) const {
	return *this < static_cast<PlayingCard>(other);
}
bool PlayingCard::operator==(const ComparableObject &other) const {
	return *this == static_cast<PlayingCard>(other);
}
bool PlayingCard::operator>=(const ComparableObject &other) const {
	return *this >= static_cast<PlayingCard>(other);
}
bool PlayingCard::operator> (const ComparableObject &other) const {
	return *this >  static_cast<PlayingCard>(other);
}
bool PlayingCard::operator<=(const ComparableObject &other) const {
	return *this <= static_cast<PlayingCard>(other);
}
bool PlayingCard::operator!=(const ComparableObject &other) const {
	return *this != static_cast<PlayingCard>(other);
}

PlayingCard::PlayingCard() : m_suit(DEFAULT_PLAYING_CARD_SUIT), m_rank(DEFAULT_PLAYING_CARD_RANK) {
}

PlayingCard::~PlayingCard() {
}

PlayingCard::PlayingCard(const PlayingCard &other) {
	m_suit = other.m_suit;
	m_rank = other.m_rank;
}

PlayingCard::PlayingCard(PlayingCard &&other) {
	m_suit = other.m_suit;
	m_rank = other.m_rank;
}

// this is so trivial that I won't implement the self copy check
PlayingCard& PlayingCard::operator=(const PlayingCard &other) {
	m_suit = other.m_suit;
	m_rank = other.m_rank;
	return *this;
}

// this is so trivial that I won't implement the self copy check
PlayingCard& PlayingCard::operator=(PlayingCard &&other) {
	m_suit = other.m_suit;
	m_rank = other.m_rank;
	return *this;
}

