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

#define TIMER_INTERVAL 500
#define TIMER_THRESHOLD 100

RunPhase::RunPhase(QObject *parent)
	: QObject(parent)
	, m_queueManager(0)
	, m_actionIndex(0)
	, m_state(STOPPED)
	, m_score(0)
	, m_oldScore(0)
{
	SoundManager *soundManager = new SoundManager(this); // TODO: Should this be passed in?
	m_ddrManager = new DDRManager(soundManager, this);

	// Set up level start timer
	m_levelStartTimer.setSingleShot(true);
	QObject::connect(&m_levelStartTimer, SIGNAL(timeout()), this, SLOT(onLevelPreStart()));

	// Set up clap timer
	m_clapTimer.setSingleShot(true);
	QObject::connect(&m_clapTimer, SIGNAL(timeout()), this, SLOT(onLevelStart()));

	// Set up main level gameplay timer
	m_timer.setInterval(TIMER_INTERVAL);
	QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerFired()));

	m_robotTimer.setSingleShot(true);
	QObject::connect(&m_robotTimer, SIGNAL(timeout()), this, SLOT(moveRobot()));
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
	m_state = STOPPED;
	setOldScore(0);
	setScore(0);

	m_ddrManager->loadLevelSounds();

	// Generate full list of actions that will be taken.
	QStack<FunctionRunner*> stack;
	stack.push(new FunctionRunner(0, m_functions[0]));
	FunctionRunner *frame = 0;
	int moveLimit = m_functions[0]->count();
	int moveCount = 0;
	while (moveCount < moveLimit && !stack.empty()) {
		frame = stack.top();
		while (frame->finished()) {
			moveCount++;
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
			switch (cmd) {
			case ApplicationUI::CMD_FORWARD:
			case ApplicationUI::CMD_LEFT:
			case ApplicationUI::CMD_RIGHT:
				m_actions.append(CommandAction(cmd, frame->function(), stack.count()));
				m_actions.append(CommandAction(cmd, frame->function(), stack.count()));
				m_actions.append(CommandAction(cmd, frame->function(), stack.count()));
				m_actions.append(CommandAction(cmd, frame->function(), stack.count()));
				break;
			default:
				break;
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
				if (moveCount < moveLimit) {
					stack.push(
						new FunctionRunner(functionIndex,
								m_functions[functionIndex]));
				} else {
					moveCount++; // otherwise terminate here.
				}
			} else if (stack.size() == 1) {
				// Moves count in the main method but we won't count function calls until they're done.
				moveCount++;
			}
		}
	}

	if (m_actions.count() <= 0) {
		// error out
		emit finished();
		return;
	}

	// Add all the actions to the queue.
	for (int i=0; i<m_actions.count(); i++) {
		m_queueManager->add(m_actions[i].action);
	}
	m_queueManager->doneActions();

//	fillQueue(m_actions.count());
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

	m_state = PRELOAD;
	m_preloadCount = 5;
	m_levelStartTimer.start(TIMER_INTERVAL);

}

void RunPhase::fillQueue(int count)
{
	m_actionIndex += count;
}

void RunPhase::pause()
{
	m_actionIndex = 0;
	m_queueManager->resetAnimation();
	m_preloadCount = 5;
	m_levelStartTimer.stop();
	m_clapTimer.stop();
	m_timer.stop();
}

void RunPhase::resume()
{
	setScore(0);
	m_state = PRELOAD;
	m_preloadCount = 5;
	m_levelStartTimer.start(TIMER_INTERVAL);
}

// Countdown: 5 - invisible in 5
//            4 - start clap track, visible in 4
//            3 - 2nd clap beat, visible in 3
//            2 - 3rd clap beat, visible in 2
//            1 - 4th clap beat, visible in 1
//            0 - start music, hit action 0, visible in 0
//            1 - music, hit action 1, visible in -1
//            2 - music, hit action 2, visible in -2
//            3 - music, hit action 3, start either robot animation or repeat ddr
void RunPhase::onLevelPreStart()
{
	switch (m_preloadCount) {
	case 1:
		onLevelStart();
		break;
	case 4:
		m_ddrManager->playClapTrack();
	default:
		m_queueManager->animate();
		m_levelStartTimer.start(TIMER_INTERVAL);
		break;
	}
	m_preloadCount--;
}

void RunPhase::onLevelStart()
{
	m_timer.start();
	emit restartAnimation();
	m_queueManager->animate();
}

void RunPhase::timerFired()
{
	if ((m_actionIndex % 4) == 3) {
		m_robotTimer.start(TIMER_THRESHOLD * 2);
	}

	if ((m_actionIndex % 4) == 0) {
		m_ddrManager->playTrack();
		emit restartAnimation();
	}

	if (!hasNoMoreActions()) {
		m_actionIndex++;
		m_queueManager->animate();
	}
}

bool RunPhase::hasNoMoreActions()
{
	return (m_actionIndex >= m_actions.count());
}

void RunPhase::moveRobot()
{
	ApplicationUI::CommandType cmd = m_actions[m_actionIndex - 1].action;
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

	if (hasNoMoreActions() || m_robot->finished()) {
		m_timer.stop();
		if (!m_robot->finished())
			setScore(-1);
		m_ddrManager->playEndTrack(m_robot->finished());
		m_queueManager->animate(); // hide the queue actions
		emit finished();
	}
}

void RunPhase::onCommand(ApplicationUI::CommandType cmd)
{
	if (m_timer.isActive()) {
		int msec = m_timer.remaining();
		if (msec < 0) {
			qDebug("ERROR: %d < 0\n", msec);
			msec = std::abs(msec);
		}
		int hitTime = msec;
		int currentIndex = m_actionIndex;
		if (msec < TIMER_THRESHOLD) {

		} else if (msec > TIMER_INTERVAL - TIMER_THRESHOLD) {
			currentIndex--;
			hitTime = TIMER_INTERVAL - hitTime;
		} else {
			// ignore, not close enough to anything
			return;
		}

		qDebug("hitTime = %d, msec = %d, currentIndex = %d\n", hitTime, msec, currentIndex);
		if (currentIndex >= 0 && currentIndex < m_actions.count()
				&& cmd == m_actions[currentIndex].action) {
			// We're okay

			if (!m_actions[currentIndex].hit) {
				m_queueManager->showHit(currentIndex);
				m_actions[currentIndex].hit = true;
				m_actions[currentIndex].hitTime = hitTime;
				if (hitTime < 30) {
					setScore(score() + 500);
				} else if (hitTime < 50) {
					setScore(score() + 250);
				} else {
					setScore(score() + 100);
				}
			}
		}
	}
}

