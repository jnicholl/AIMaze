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

class DDRManager;
class QueueManager;
class Function;
class Robot;

struct CommandAction {
	CommandAction(ApplicationUI::CommandType a, int f)
		: action(a)
		, function(f) {}
	ApplicationUI::CommandType action;
	int function;
};

class RunPhase : public QObject {
	Q_OBJECT
public:
	RunPhase(QObject *parent=0);
	void init(Robot *robot, QueueManager *queueMgr, QList<Function*> functions);

	Q_SLOT void fillQueue(int count);
	Q_SLOT void timerFired();

	Q_SLOT void onLevelStart();

	bool hasNoMoreActions();

	void pause();
	void resume();

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
	QTimer m_timer;
};

#endif /* RUNPHASE_H_ */
