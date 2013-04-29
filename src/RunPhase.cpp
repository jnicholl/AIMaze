/*
 * RunPhase.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: jnicholl
 */

#include "RunPhase.h"

#include "DDRManager.h"
#include "QueueManager.h"
#include "SoundManager.h"
#include "function.h"
#include "robot.h"


RunPhase::RunPhase(QObject *parent)
	: QObject(parent)
	, m_queueManager(0)
	, m_actionIndex(0)
{
	SoundManager *soundManager = new SoundManager(this); // TODO: Should this be passed in?
	m_ddrManager = new DDRManager(soundManager, this);

	// Set up level start timer
	m_levelStartTimer.setSingleShot(true);
	QObject::connect(&m_levelStartTimer, SIGNAL(timeout()), this, SLOT(onLevelStart()));

	// Set up main level gameplay timer
	m_timer.setInterval(2000);
	QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerFired()));
}

void RunPhase::init(Robot *robot, QueueManager *queueMgr, QList<Function*> functions) {
	if (m_queueManager != queueMgr) {
		m_queueManager = queueMgr;
		QObject::connect(m_queueManager, SIGNAL(spaceAvailable(int)), this, SLOT(fillQueue(int)));
	}
	m_robot = robot;
	m_functions = functions;
	m_actions.clear();
	m_actionIndex = 0;

	m_ddrManager->loadLevelSounds();

	// Generate full list of actions that will be taken.
	QStack<FunctionRunner*> stack;
	stack.push(new FunctionRunner(0, m_functions[0]));
	FunctionRunner *frame = 0;
	int moveLimit = m_functions[0]->count();
	int moveCount = 0;
	while (moveCount <= moveLimit && !stack.empty()) {
		frame = stack.top();
		while (frame->finished()) {
			frame = stack.pop();
			delete frame;
			if (stack.empty()) {
				// Finished all function calls, we are done
				break;
			}
			frame = stack.top();
		}
		if (!stack.empty()) {
			ApplicationUI::CommandType cmd = frame->step();
			m_actions.append(CommandAction(cmd, frame->function()));

			if (stack.size() == 1) {
				// Moves count in the main method
				moveCount++;
			}

			int functionIndex = -1;
			switch (cmd) {
			case ApplicationUI::CMD_F1:
				functionIndex = 1;
				break;
			case ApplicationUI::CMD_F2:
				functionIndex = 2;
				break;
			case ApplicationUI::CMD_F3:
				functionIndex = 3;
				break;
			default:
				break;
			}
			if (functionIndex > 0) {
				if (stack.size() > 1) {
					// Function calls count everywhere
					moveCount++;
				}
				stack.push(
						new FunctionRunner(functionIndex,
								m_functions[functionIndex]));
			}
		}
	}

	fillQueue(QUEUE_LIMIT);
	for (int i = 0; i < m_actions.count(); i++) {
		switch (m_actions[i].action) {
		case ApplicationUI::CMD_FORWARD:
			qDebug("Forward");
			break;
		case ApplicationUI::CMD_LEFT:
			qDebug("Left");
			break;
		case ApplicationUI::CMD_RIGHT:
			qDebug("Right");
			break;
		case ApplicationUI::CMD_F1:
			qDebug("F1");
			break;
		case ApplicationUI::CMD_F2:
			qDebug("F2");
			break;
		case ApplicationUI::CMD_F3:
			qDebug("F3");
			break;
		default:
			qDebug("BAD: command %d\n", m_actions[i].action);
			break;
		}
	}

	m_levelStartTimer.start(3000);
}

void RunPhase::fillQueue(int count)
{
	if (count > m_actions.count() - m_actionIndex)
		count = m_actions.count() - m_actionIndex;

	for (int i=0; i<count; i++) {
		//if (m_actions[m_actionIndex].function == 0) // FIXME: Currently not adding function actions
		m_queueManager->add(m_actions[m_actionIndex].action);
		m_actionIndex++;
	}
}

void RunPhase::pause()
{
	m_levelStartTimer.stop();
	m_timer.stop();
}

void RunPhase::resume()
{
	m_timer.start(); // FIXME: Properly handle resuming level start or fractional timer
}

void RunPhase::onLevelStart()
{
	m_timer.start();
	emit restartAnimation();
}

void RunPhase::timerFired()
{
	ApplicationUI::CommandType cmd = m_queueManager->peek();
	switch (cmd) {
	case ApplicationUI::CMD_FORWARD:
		m_robot->moveForward();
		break;
	case ApplicationUI::CMD_LEFT:
		m_robot->turnLeft();
		break;
	case ApplicationUI::CMD_RIGHT:
		m_robot->turnRight();
		break;
	default:
		break;
	}
	m_queueManager->remove(0, true);

	if (hasNoMoreActions() || m_robot->finished()) {
		m_timer.stop();
		m_ddrManager->playEndTrack(m_robot->finished());
		emit finished();
	} else {
		m_ddrManager->playTrack();
		emit restartAnimation();
	}
}

bool RunPhase::hasNoMoreActions()
{
	if (m_actionIndex < m_actions.count())
		return false;
	return m_queueManager->empty();
}

