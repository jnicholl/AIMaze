// Navigation pane project template
#include "applicationui.hpp"

#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/ListView>
#include <bb/cascades/Page>
#include <bb/cascades/PaneProperties>
#include <bb/cascades/GroupDataModel>
#include <bb/data/JsonDataAccess>
#include <bb/cascades/Container>
#include <bb/cascades/Label>
#include <bb/cascades/SequentialAnimation>

#include "map.h"
#include "robot.h"
#include "function.h"

using namespace bb::cascades;
using namespace bb::device;
using namespace bb::data;

#define MAX_WIDTH 200
#define MAX_HEIGHT 200

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
: QObject(app)
, m_levelList(0)
, m_gamePage(0)
, m_navigationPane(0)
, m_qmlQueueCommand(0)
, m_queueContainer(0)
, m_mapArea(0)
, m_progressAnimation(0)
, m_queueCount(0)
, m_map(0)
, m_robot(0)
, m_phase(COMPILE)
, m_selectedFunction(0)
, m_functionHeader(0)
{
	// create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    qml->setContextProperty("_app", this);

    // create root object for the UI
    m_navigationPane = qml->createRootObject<NavigationPane>();
    m_levelList = m_navigationPane->findChild<ListView*>("listView");
    if (!m_levelList) qFatal("Failed to find level list in QML");

    // set created root object as a scene
    app->setScene(m_navigationPane);

    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerFired()));
}

void ApplicationUI::back()
{
	m_navigationPane->pop();
}

// FIXME: Should really store the remaining time and start the next tick as much shorter.
void ApplicationUI::pause()
{
	if (m_phase == RUN) {
		m_timer.stop();
		if (m_progressAnimation)
			m_progressAnimation->stop();
	}
}

void ApplicationUI::unpause()
{
	if (m_phase == RUN) {
		m_timer.start();
		if (m_progressAnimation && m_robot && !m_robot->finished())
			m_progressAnimation->play();
	}
}

void ApplicationUI::compilePhaseDone()
{
	m_phase = RUN;
	unpause();
}

void ApplicationUI::setupLevel(const QVariantMap &levelData)
{
	if (m_map) {
		delete m_map;
	}
	if (m_robot) {
		delete m_robot;
	}
	while (!m_functions.empty()) {
		Function *f = m_functions[0];
		m_functions.removeFirst();
		delete f;
	}
	while (!m_stack.empty()) {
		FunctionRunner *f = m_stack.pop();
		delete f;
	}

	m_mapArea = m_gamePage->findChild<Container*>("mapArea");
	if (!m_mapArea) {
		qDebug("Failed to find map area");
		return;
	}
	m_mapArea->removeAll();

	int width = levelData["width"].toInt();
	int height = levelData["height"].toInt();
	int startX = levelData["startX"].toInt();
	int startY = levelData["startY"].toInt();
	QString direction = levelData["startDir"].toString();
	int endX = levelData["endX"].toInt();
	int endY = levelData["endY"].toInt();
	QVariantList mapData = levelData["data"].toList();

	int functionCount = 3;
	if (levelData.contains("numFunctions"))
		functionCount = levelData["numFunctions"].toInt();
	if (functionCount < 0) functionCount = 0;
	if (functionCount > 3) functionCount = 3; // FIXME: Hide function buttons if not allowed.

	QVariantList functionLimits = levelData["functionLimit"].toList();
	for (int i=0; i<functionCount; i++) {
		Function *f;
		if (i < functionLimits.count())
			f = new Function(functionLimits[i].toInt());
		else
			f = new Function();
		m_functions.append(f);
		qDebug("Function %d limited to %d\n", i, f->count());
	}
	setFunctionCount(functionCount);
	qDebug("Function count: %d\n", functionCount);

	if (width < 0 || width > MAX_WIDTH || height < 0 || height > MAX_HEIGHT || width * height != mapData.count()) {
		qDebug("Bad level data: %dx%d with %d elements\n", width, height, mapData.count());
		return;
	}

	int *data = new int[width*height];
	for (int i=0; i<width*height; i++) {
		data[i] = mapData[i].toInt();
	}

	m_map = new Map(height, width, endX, endY, data, m_mapArea, this);
	m_robot = new Robot(m_map, startX, startY, endX, endY, Robot::getDirection(direction), this);

	m_functionHeader = static_cast<Container *>(m_gamePage->findChild<Container*>("functionHeader"));
	m_functionActions.clear();
	m_functionActions.append(static_cast<Container *>(m_gamePage->findChild<Container*>("functionAction1")));
	m_functionActions.append(static_cast<Container *>(m_gamePage->findChild<Container*>("functionAction2")));
	m_functionActions.append(static_cast<Container *>(m_gamePage->findChild<Container*>("functionAction3")));
	m_functionActions.append(static_cast<Container *>(m_gamePage->findChild<Container*>("functionAction4")));
	m_functionActions.append(static_cast<Container *>(m_gamePage->findChild<Container*>("functionAction5")));
	m_functionActions.append(static_cast<Container *>(m_gamePage->findChild<Container*>("functionAction6")));
	m_functionActions.append(static_cast<Container *>(m_gamePage->findChild<Container*>("functionAction7")));
	qDebug("Function header: %p\n", m_functionHeader);
	qDebug("Function actions size: %d\n", m_functionActions.count());

	QObject::connect(m_robot, SIGNAL(moved(int,int)), this, SLOT(robotMoved(int,int)));
}

void ApplicationUI::setupQueue()
{
	if (!m_qmlQueueCommand) {
		m_qmlQueueCommand = QmlDocument::create("asset:///QueueCommand.qml").parent(this);
		m_qmlQueueCommand->setContextProperty("_app", this);
	}

	m_queueContainer = m_gamePage->findChild<Container*>("queueContainer");
	if (!m_queueContainer) qFatal("Failed to find queue container");

	m_queue.clear();
	m_queueContainer->removeAll();
	m_queueCount = 0;

	for (int i=0; i<QUEUE_LIMIT; i++) {
		m_queueCommands[i] = CMD_EMPTY;
		Container *queueObject = m_qmlQueueCommand->createRootObject<Container>();
		queueObject->setProperty("index", i);
		queueObject->setProperty("text", QString::number(m_queueCommands[i]));
		m_queueContainer->add(queueObject);
		m_queue.append(queueObject);
	}
}

void ApplicationUI::startLevel(const QVariantList &indexPath)
{
	if (!m_gamePage) {
		QmlDocument *qml = QmlDocument::create("asset:///Game.qml").parent(this);
		qml->setContextProperty("_app", this);
		m_gamePage = qml->createRootObject<Page>();
		m_progressAnimation = m_gamePage->findChild<SequentialAnimation*>("progressAnimation");
	}

	m_phase = COMPILE;
	Container *compileContainer = m_gamePage->findChild<Container*>("compilePhaseContainer");
	compileContainer->setVisible(true);

	QVariantMap levelInfo = m_levelList->dataModel()->data(indexPath).toMap();
	QString levelPath = levelInfo["level"].toString();
	JsonDataAccess jda;
	qDebug() << "Attempting to load " << levelPath;
	QVariantMap levelData = jda.load(levelPath).toMap();
	if (jda.hasError()) {
		bb::data::DataAccessError error = jda.error();
		qFatal("JSON loading error: %d: %s", error.errorType(), qPrintable(error.errorMessage()));
		return;
	} else {
		qDebug() << "JSON data loaded OK!";
	}

	setupLevel(levelData);
	setupQueue();

	m_navigationPane->push(m_gamePage);

	m_timer.setInterval(2000);

	m_gamePage->findChild<Container*>("compileFunctionContainer")->setVisible(m_functionCount > 0);
	drawSelectedFunction();
}

QString ApplicationUI::getImageForCommand(CommandType type)
{
	QString text = ""; // invalid URL indicates no image.
	switch (type) {
	case CMD_FORWARD:
		text = "asset:///images/forward.png";
		break;
	case CMD_LEFT:
		text = "asset:///images/left.png";
		break;
	case CMD_RIGHT:
		text = "asset:///images/right.png";
		break;
	case CMD_F1:
		text = "asset:///images/f1.png";
		break;
	case CMD_F2:
		text = "asset:///images/f2.png";
		break;
	case CMD_F3:
		text = "asset:///images/f3.png";
		break;
	case CMD_BLOCKED:
		text = "";
		break;
	default:
		break;
	}
	return text;
}

void ApplicationUI::setQueueValue(int i, CommandType type)
{
	qDebug() << "Set " << i << " from " << m_queueCommands[i] << " to " << type;
	m_queueCommands[i] = type;
	m_queue[i]->setProperty("imageSource", getImageForCommand(type));
}

void ApplicationUI::addQueuedCommand(CommandType type)
{
	if (type == CMD_EMPTY) return;

	if (m_phase == RUN) {
		for (int i=0; i<QUEUE_LIMIT; i++) {
			if (m_queueCommands[i] == CMD_EMPTY) {
				setQueueValue(i, type);
				m_queueCount++;
				break;
			}
		}
		// TODO: return failure to show user queue full?
	} else if (m_phase == COMPILE) {
		qDebug("Add command to function %d\n", m_selectedFunction);
		if (m_selectedFunction >= 0 && m_selectedFunction < m_functions.count()) {
			m_functions[m_selectedFunction]->append(type);
			drawSelectedFunction();
		}
	}
}

void ApplicationUI::drawSelectedFunction()
{
	if (m_functionCount == 0) {
		return;
	}

	qDebug("Drawing selected function %d\n", m_selectedFunction);
	switch (m_selectedFunction) {
	case 0:
		m_functionHeader->setProperty("imageSource", "asset:///images/f1.png");
		break;
	case 1:
		m_functionHeader->setProperty("imageSource", "asset:///images/f2.png");
		break;
	case 2:
		m_functionHeader->setProperty("imageSource", "asset:///images/f3.png");
		break;
	}

	int i=0;
	Function *f = m_functions[m_selectedFunction];
	qDebug("Function[%d] count: %d\n", m_selectedFunction, f->count());
	for (; i<f->count(); i++) {
		qDebug("Setting source for action[%d] from source[%d] (%d)\n", i+1, i, (*f)[i]);
		m_functionActions[i]->setProperty("imageSource", getImageForCommand((*f)[i]));
		m_functionActions[i]->setVisible(true);
	}
	for (; i<m_functionActions.count(); i++) {
		qDebug("Hiding action[%d]\n", i+1);
		m_functionActions[i]->setVisible(false);
	}
}

void ApplicationUI::selectNextFunction()
{
	m_selectedFunction++;
	if (m_selectedFunction >= m_functionCount)
		m_selectedFunction = 0;
	drawSelectedFunction();
}

void ApplicationUI::tapForward()
{
	addQueuedCommand(CMD_FORWARD);
}

void ApplicationUI::tapLeft()
{
	addQueuedCommand(CMD_LEFT);
}

void ApplicationUI::tapRight()
{
	addQueuedCommand(CMD_RIGHT);
}

void ApplicationUI::tapF1()
{
	addQueuedCommand(CMD_F1);
}

void ApplicationUI::tapF2()
{
	addQueuedCommand(CMD_F2);
}

void ApplicationUI::tapF3()
{
	addQueuedCommand(CMD_F3);
}

void ApplicationUI::tapViewFunctions()
{
	// FIXME!
}

void ApplicationUI::removeQueuedCommand(int index)
{
	qDebug() << "Removing queue command at " << index << " (queueCount = " << m_queueCount << ")";
	if (index < 0) return;
	if (index >= m_queueCount) return;
	if (m_queueCommands[index] == CMD_EMPTY) return;

	for (int i=index; i<QUEUE_LIMIT-1; i++) {
		setQueueValue(i, m_queueCommands[i+1]);
	}

	setQueueValue(QUEUE_LIMIT-1, CMD_EMPTY);
	m_queueCount--;
}

void ApplicationUI::removeFunctionCommand(int index)
{
	qDebug("Removing %d from functions[%d]\n", index, m_selectedFunction);
	m_functions[m_selectedFunction]->remove(index);
	drawSelectedFunction();
}

void ApplicationUI::timerFired()
{
	qDebug() << "Timer fired";
	FunctionRunner *frame = 0;
	bool shouldRemove = true;
	CommandType cmd = m_queueCommands[0];

	qDebug() << "Stack empty?";
	if (!m_stack.empty()) {
		qDebug() << "No";
		frame = m_stack.top();
		while (frame->finished()) {
			qDebug() << "Frame finished";
			frame = m_stack.pop();
			qDebug() << "Deleting";
			delete frame;
			qDebug() << "Deleted";
			if (m_stack.empty()) {
				qDebug() << "Stack really empty";
				// Finished all function calls, remove the function on the queue
				removeQueuedCommand(0);
				cmd = m_queueCommands[0];
				break;
			}
			frame = m_stack.top();
		}

		if (!m_stack.empty()) {
			qDebug() << "Stack not empty";
			cmd = frame->step();
			shouldRemove = false; // FIXME: We should really leave the currently executing function in the queue.
		}

		frame = 0;
	}

	qDebug() << "Cmd: " << cmd;
	switch (cmd) {
	case CMD_FORWARD:
		m_robot->moveForward();
		break;
	case CMD_LEFT:
		m_robot->turnLeft();
		break;
	case CMD_RIGHT:
		m_robot->turnRight();
		break;
	case CMD_F1:
		frame = new FunctionRunner(m_functions[0]);
		shouldRemove = false;
		break;
	case CMD_F2:
		frame = new FunctionRunner(m_functions[1]);
		shouldRemove = false;
		break;
	case CMD_F3:
		frame = new FunctionRunner(m_functions[2]);
		shouldRemove = false;
		break;
	default:
		break;
	}

	if (frame) {
		m_stack.push(frame);
	}

	if (shouldRemove)
		removeQueuedCommand(0);

	if (!m_robot->finished())
		m_progressAnimation->play();
}

void ApplicationUI::robotMoved(int x, int y)
{
	Q_UNUSED(x);
	Q_UNUSED(y);
	if (m_robot->finished()) {
		qDebug() << "Finished";
		m_timer.stop();
	}
}
