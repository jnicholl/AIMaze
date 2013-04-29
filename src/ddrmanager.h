/*
 * ddrmanager.h
 *
 *  Created on: Apr 24, 2013
 *      Author: Jeremy
 */

#ifndef DDRMANAGER_H_
#define DDRMANAGER_H_

#include <QtCore/QObject>
#include <QtCore/QVector>

class SoundManager;

class DDRManager: public QObject {
	Q_OBJECT
public:
	enum State {
		START = 0,
		WIN = -1,
		LOSE = -2
	};

	DDRManager(SoundManager *soundMgr, QObject *parent = 0);
	virtual ~DDRManager();

	bool loadLevelSounds();
	Q_SLOT void start();
	Q_SLOT void onTimer();

	Q_SLOT void setRepeat(bool repeat) { m_repeatTrack = repeat; }
	Q_SLOT void setWin(bool win) { m_win = win; }

	void playEndTrack(bool won);
	void playTrack();

signals:
	void levelDone(bool won);

private:
	SoundManager *m_soundMgr;

	unsigned m_winTrack;
	unsigned m_loseTrack;
	unsigned m_trackCount;
	QVector<unsigned> m_tracks;

	int m_state;
	bool m_repeatTrack;
	bool m_win;
};

#endif /* DDRMANAGER_H_ */
