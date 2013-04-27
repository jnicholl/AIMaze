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
	void setLevelComplete(int level);

signals:
	void gameStateChanged();

private:
	int m_levelAvailable;
};

#endif /* GAMESTATE_H_ */
