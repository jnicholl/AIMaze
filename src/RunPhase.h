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
	{}
	ApplicationUI::CommandType action;
	int function;
	int stackDepth;
};

class RunPhase : public QObject {
	Q_OBJECT
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

signals:
	void restartAnimation();
	void finished();

private:
	QueueManager *m_queueManager;
	QList<Function*> m_functions;
	QList<CommandAction> m_actions;
	int m_actionIndex;
	DDRManager *m_ddrManager;
	Robot *m_robot;

	QTimer m_levelStartTimer;
	QTimer m_clapTimer;
	ElapsedTimer m_timer;

	State m_state;
	int m_preloadCount;
};

#endif /* RUNPHASE_H_ */
