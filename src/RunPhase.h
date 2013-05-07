/*
 * RunPhase.h
 *
 *  Created on: Apr 27, 2013
 *      Author: jnicholl
 */

#ifndef RUNPHASE_H_
#define RUNPHASE_H_

#include <QtCore/QObject>
#include <QtCore/QList>

#include "applicationui.hpp"

#include "ElapsedTimer.h"

class DDRManager;
class QueueManager;
class Function;
class Robot;

struct CommandAction {
	CommandAction(ApplicationUI::CommandType a, int f, int s)
		: action(a)
		, function(f)
		, stackDepth(s)
		, hit(false)
		, hitTime(0)
	{}
	ApplicationUI::CommandType action;
	int function;
	int stackDepth;
	bool hit;
	int hitTime;
};

class RunPhase : public QObject {
	Q_OBJECT
	Q_PROPERTY(int score READ score WRITE setScore NOTIFY scoreChanged)
	Q_PROPERTY(int oldScore READ oldScore WRITE setOldScore NOTIFY oldScoreChanged)
public:
	enum State {
		STOPPED = 0,
		PRELOAD = 1,
		CLAP = 2,
		RUN = 3
	};

	RunPhase(QObject *parent=0);
	void init(Robot *robot, QueueManager *queueMgr, QList<Function*> functions);

	Q_SLOT void fillQueue(int count);
	Q_SLOT void timerFired();

	Q_SLOT void onLevelStart();
	Q_SLOT void onLevelPreStart();

	bool hasNoMoreActions();

	void pause();
	void resume();

	void onCommand(ApplicationUI::CommandType cmd);

	int score() const { return m_score; }
	void setScore(int score) {
		if (m_score != score) {
			setOldScore(m_score);
			m_score = score;
			emit scoreChanged(score);
		}
	}
	void resetScore() {
		setScore(0);
	}

	int oldScore() const { return m_oldScore; }
	void setOldScore(int score) {
		if (m_oldScore != score) {
			m_oldScore = score;
			emit oldScoreChanged(score);
		}
	}

signals:
	void restartAnimation();
	void finished();
	void scoreChanged(int score);
	void oldScoreChanged(int score);

private:
	Q_SLOT void moveRobot();

	QueueManager *m_queueManager;
	QList<Function*> m_functions;
	QList<CommandAction> m_actions;
	int m_actionIndex;
	DDRManager *m_ddrManager;
	Robot *m_robot;

	QTimer m_levelStartTimer;
	QTimer m_clapTimer;
	ElapsedTimer m_timer;
	QTimer m_robotTimer;

	State m_state;
	int m_preloadCount;
	int m_score;
	int m_oldScore;
};

#endif /* RUNPHASE_H_ */
