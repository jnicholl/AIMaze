/*
 * GameState.h
 *
 *  Created on: Apr 26, 2013
 *      Author: jnicholl
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <QtCore/QObject>

class GameState: public QObject {
	Q_OBJECT
public:
	GameState();
	void load();
	void save();

	int levelAvailable() const;
	void setLevelComplete(int level, int score);
	int scoreForLevel(int level) {
		if (level >= 0 && level < m_scores.count()) {
			return m_scores[level];
		}
		return 0;
	}

signals:
	void gameStateChanged();
	void scoresChanged(int);

private:
	int m_levelAvailable;
	QList<int> m_scores;
};

#endif /* GAMESTATE_H_ */
