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
#include <bb/cascades/Button>
#include <bb/cascades/SequentialAnimation>
#include <bb/cascades/TapHandler>
#include <bb/cascades/Window>

#include "map.h"
#include "robot.h"
#include "function.h"
#include <bb/cascades/ScreenIdleMode>

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
, m_levelIndex(0)
, m_levelAvailable(0)
, m_map(0)
, m_robot(0)
, m_phase(MENU)
, m_functionCount(0)
, m_selectedFunction(0)
, m_functionHeader(0)
, m_shouldShowFunctions(false)
, m_isInF1(false)
, m_isInF2(false)
, m_isInF3(false)
, m_highlightedContainer(0)
, m_tutorial(0)
{
	QObject::connect(Application::instance(), SIGNAL(swipeDown()), this, SLOT(showMenu()));
	QObject::connect(Application::instance(), SIGNAL(thumbnail()), this, SLOT(showMenu()));
	QObject::connect(Application::instance(), SIGNAL(invisible()), this, SLOT(showMenu()));
	loadSavedState();

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
    m_finishTimer.setSingleShot(true);
    QObject::connect(&m_finishTimer, SIGNAL(timeout()), this, SLOT(unpause()));
}

void ApplicationUI::loadSavedState()
{
	QString filePath(QDir::currentPath() + "/data/levelState.json");
	if (QFile::exists(filePath)) {
		JsonDataAccess jda;
		QVariantMap state = jda.load(filePath).toMap();
		if (jda.hasError())
			return;
		if (state.contains("levelAvailable")) {
			int levelAvailable = state["levelAvailable"].toInt();
			if (levelAvailable > 0) {
				setLevelAvailable(levelAvailable);
			}
		}
	}
	qDebug() << "Level available" << m_levelAvailable;
}

void ApplicationUI::saveState()
{
	qDebug() << "saving state to " << QDir::currentPath() + "/data/levelState.json";
	QVariantMap state;
	state["levelAvailable"] = m_levelAvailable;
	QFile file(QDir::currentPath() + "/data/levelState.json");
	if (file.open(QIODevice::WriteOnly)) {
		qDebug() << "opened file okay";
		JsonDataAccess jda;
		jda.save(state, &file);
		if (jda.hasError()) {
			qDebug("Failed to save: Json error\n");
		} else {
			qDebug("Saved");
		}
	}
}

void ApplicationUI::back()
{
	m_phase = MENU;
	m_navigationPane->pop();
}

void ApplicationUI::showMenu()
{
	if (m_phase == RUN || m_phase == COMPILE) {
		pause();
		m_gamePage->findChild<Container*>("menuContainer")->setVisible(true);
	}
}

// FIXME: Should really store the remaining time and start the next tick as much shorter.
void ApplicationUI::pause()
{
	if (m_phase == RUN) {
		if (m_finishTimer.isActive()) {
			m_finishTimer.stop();
		}
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
	Container *container = 0;
	for (int i=0; i<3; i++) {
		for (int j=0; j<DEFAULT_FUNCTION_SIZE; j++) {
			container = m_gamePage->findChild<Container*>(QString("func%1_act%2").arg(i+1).arg(j+1));
			if (container)
				container->setProperty("actionVisible", false);
			else
				qDebug() << "No container at " << QString("func%1_act%2").arg(i+1).arg(j+1);
		}
	}
	for (int i=0; i<m_functionCount; i++) {
		Function *f = m_functions[i];
		for (int j=0; j<f->commandCount(); j++) {
			container = m_gamePage->findChild<Container*>(QString("func%1_act%2").arg(i+1).arg(j + 1 + (DEFAULT_FUNCTION_SIZE - f->commandCount())));
			if (container) {
				container->setProperty("imageSource", getImageForCommand(f->at(j)));
				container->setProperty("actionVisible", true);
			} else
				qDebug() << "No container at " << QString("func%1_act%2").arg(i+1).arg(j + 1 + (DEFAULT_FUNCTION_SIZE - f->commandCount()));
		}
	}

	m_phase = RUN;
	Application::instance()->mainWindow()->setScreenIdleMode(ScreenIdleMode::KeepAwake);

	m_finishTimer.start(3000);
	//QTimer::singleShot(3000, this, SLOT(unpause()));
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
	int moves = levelData["totalMoves"].toInt();
	setTutorial(levelData["tutorial"].toInt()); // will return 0 if no tutorial field

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
	Label *movesLabel = m_gamePage->findChild<Label*>("movesLeft");
	m_robot = new Robot(m_map, moves, movesLabel, startX, startY, endX, endY, Robot::getDirection(direction), this);

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

	if (indexPath.count() > 0)
		m_levelIndex = indexPath[0].toInt();
	else
		m_levelIndex = 0; // Uhoh?

	m_phase = COMPILE;
	m_selectedFunction = 0;
	setShouldShowFunctions(false);
	setIsInFunction(-1);
	Container *compileContainer = m_gamePage->findChild<Container*>("compilePhaseContainer");
	compileContainer->setVisible(true);

	m_gamePage->findChild<Container*>("tutorial1Container")->setProperty("state", 0);
	m_gamePage->findChild<Container*>("tutorial2Container")->setProperty("state", 0);
	m_gamePage->findChild<Container*>("tutorial3Container")->setProperty("state", 0);
	m_gamePage->findChild<Container*>("progressBar")->setTranslationX(0);
	m_gamePage->findChild<Container*>("creditsContainer")->setVisible(false);

	m_gamePage->findChild<Button*>("menuButton")->setText("Continue");
	m_gamePage->findChild<Container*>("menuContainer")->setVisible(false);

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
	setShouldShowFunctions(!m_shouldShowFunctions);
}

void ApplicationUI::removeQueuedCommand(int index, bool force)
{
	qDebug() << "Removing queue command at " << index << " (queueCount = " << m_queueCount << ")";
	if (index < 0) return;
	if (index >= m_queueCount) return;
	if (m_queueCommands[index] == CMD_EMPTY) return;
	if (!force && index == 0 && !m_stack.empty()) return;

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

void ApplicationUI::highlightFunction(int function, int pc)
{
	if (m_highlightedContainer) {
		m_highlightedContainer->setProperty("highlighted", false);
	}

	Container *container = m_gamePage->findChild<Container*>(QString("func%1_act%2").arg(function+1).arg((DEFAULT_FUNCTION_SIZE-m_functions[function]->commandCount())+pc+1));
	if (container) {
		m_highlightedContainer = container;
		m_highlightedContainer->setProperty("highlighted", true);
	} else {
		qDebug() << "Unable to find container: " << QString("func%1_act%2").arg(function+1).arg((DEFAULT_FUNCTION_SIZE-m_functions[function]->commandCount())+pc+1);
	}
}

void ApplicationUI::timerFired()
{
	FunctionRunner *frame = 0;
	bool shouldRemove = true;
	bool countsAsMove = true;
	CommandType cmd = m_queueCommands[0];

	if (!m_stack.empty()) {
		frame = m_stack.top();
		while (frame->finished()) {
			frame = m_stack.pop();
			delete frame;
			if (m_stack.empty()) {
				// Finished all function calls, remove the function on the queue
				removeQueuedCommand(0, true);
				cmd = m_queueCommands[0];
				break;
			}
			frame = m_stack.top();
		}

		if (!m_stack.empty()) {
			setIsInFunction(frame->function());
			highlightFunction(frame->function(), frame->pc());
			cmd = frame->step();
			countsAsMove = false;
			shouldRemove = false;
		}

		frame = 0; // We re-use this later if the command was a function call.
	}

	if (m_stack.empty()) {
		setIsInFunction(-1);
		if (m_highlightedContainer) {
			m_highlightedContainer->setProperty("highlighted", false);
		}
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
		frame = new FunctionRunner(0, m_functions[0]);
		countsAsMove = true;
		shouldRemove = false;
		break;
	case CMD_F2:
		frame = new FunctionRunner(1, m_functions[1]);
		countsAsMove = true;
		shouldRemove = false;
		break;
	case CMD_F3:
		frame = new FunctionRunner(2, m_functions[2]);
		countsAsMove = true;
		shouldRemove = false;
		break;
	default:
		break;
	}

	if (frame) {
		m_stack.push(frame);
	}

	if (shouldRemove)
		removeQueuedCommand(0, true);

	if (countsAsMove) {
		m_robot->decrementMoves();
		if (m_robot->hasNoPower(!m_stack.empty())) {
			m_robot->setImageForPower(false);
			QTimer::singleShot(500, this, SLOT(processFinish()));
			//processFinish(m_robot->finished());
			return;
		}
	}

	if (!m_robot->finished())
		m_progressAnimation->play();
	else
		QTimer::singleShot(500, this, SLOT(processFinish()));
}

void ApplicationUI::clickMenuButton()
{
	if (m_phase == FINISHED) {
		if (m_robot->finished()) {
			nextLevel();
		} else {
			retry();
		}
	} else {
		// Just entered pause menu from game
		unpause();
	}
}

void ApplicationUI::processFinish()
{
	bool win = m_robot->finished();
	pause();
	m_phase = FINISHED;
	Application::instance()->mainWindow()->setScreenIdleMode(ScreenIdleMode::Normal);
	QString text, buttonText;
	if (win) {
		int levelCount = m_levelList->dataModel()->childCount(QVariantList());
		if (m_levelIndex >= levelCount-1) {
			m_gamePage->findChild<Container*>("creditsContainer")->setVisible(true);
			return;
		}
//		text = "You won";
		buttonText = "Next level";
		qDebug("Level available: %d, levelIndex = %d\n", m_levelAvailable, m_levelIndex);
		if (m_levelAvailable <= m_levelIndex) {
			setLevelAvailable(m_levelIndex+1);
			saveState();
		}
	} else {
//		text = "You lost";
		buttonText = "Retry";
	}
//	m_gamePage->findChild<Label*>("menuTitle")->setText(text);
	m_gamePage->findChild<Button*>("menuButton")->setText(buttonText);
	m_gamePage->findChild<Container*>("menuContainer")->setVisible(true); // FIXME: need to reset labels
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

void ApplicationUI::nextLevel()
{
	int levelCount = m_levelList->dataModel()->childCount(QVariantList());
	if (m_levelIndex >= levelCount-1) {
		back();
	} else {
		QVariantList indexPath;
		indexPath.append(m_levelIndex+1);
		startLevel(indexPath);
	}
}

void ApplicationUI::retry()
{
	QVariantList indexPath;
	indexPath.append(m_levelIndex);
	startLevel(indexPath);
}
