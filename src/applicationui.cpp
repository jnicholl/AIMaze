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
#include "QueueManager.h"
#include "RunPhase.h"
#include "MusicPlayer.h"
#include <bb/cascades/ScreenIdleMode>

using namespace bb::cascades;
using namespace bb::device;
using namespace bb::data;

#define MAX_WIDTH 200
#define MAX_HEIGHT 200

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
: QObject(app)
, m_levelList(0)
, m_navigationPane(0)
, m_gamePage(0)
, m_mapArea(0)
, m_progressAnimation(0)
, m_queueManager(0)
, m_levelIndex(0)
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
, m_runPhase(new RunPhase(this))
{
	// Connect application signals so we show the menu to pause
	QObject::connect(Application::instance(), SIGNAL(swipeDown()), this, SLOT(showMenu()));
	QObject::connect(Application::instance(), SIGNAL(thumbnail()), this, SLOT(showMenu()));
	QObject::connect(Application::instance(), SIGNAL(invisible()), this, SLOT(showMenu()));

	// Load any stored game state
	QObject::connect(&m_gameState, SIGNAL(gameStateChanged()), this, SLOT(onGameStateChanged()));
	m_gameState.load();

	// Connect tutorial manager signals
	QObject::connect(&m_tutorialManager, SIGNAL(tutorialChanged(int)), this, SIGNAL(tutorialChanged(int)));

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

    // Connect up timers.
//    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerFired()));
//    m_finishTimer.setSingleShot(true);
//    QObject::connect(&m_finishTimer, SIGNAL(timeout()), this, SLOT(unpause()));

    // Connect run-phase signals
    QObject::connect(m_runPhase, SIGNAL(finished()), this, SLOT(onFinished()));
    QObject::connect(m_runPhase, SIGNAL(restartAnimation()), this, SLOT(onRestartAnimation()));

    m_mediaPlayer = new MusicPlayer(this);
    m_mediaPlayer->setSourceUrl(QUrl("app/native/assets/sounds/title.ogg"));
    m_mediaPlayer->play();

	QObject::connect(Application::instance(), SIGNAL(invisible()), m_mediaPlayer, SLOT(stop()));
    QObject::connect(Application::instance(), SIGNAL(awake()), m_mediaPlayer, SLOT(play()));
}

// Takes us back to the level selection screen.
// Generally should only be called from the menu.
void ApplicationUI::back()
{
	m_mediaPlayer->play();
	m_phase = MENU;
	m_navigationPane->pop();
}

// Shows the menu ONLY if we are in run or compile phases.
// Showing the menu also pauses the game from the run phase.
void ApplicationUI::showMenu()
{
	if (m_phase == RUN || m_phase == COMPILE) {
		pause();
		m_gamePage->findChild<Container*>("menuContainer")->setVisible(true);
	}
}

// Pauses the game by stopping both timers and the animation, if in run phase.
// FIXME: Should really store the remaining time and start the next tick as much shorter.
void ApplicationUI::pause()
{
	if (m_phase == RUN) {
		if (m_runPhase)
			m_runPhase->pause();
		else
			qDebug("BAD: ApplicationUI::pause called in run state with no runphase object!");
//		if (m_finishTimer.isActive()) {
//			m_finishTimer.stop();
//		}
//		m_timer.stop();
//		if (m_progressAnimation)
//			m_progressAnimation->stop();
	}
}

// Resumes the game if we are in run phase, starting the timer and progress animation.
// We ignore the finish timer since it is only used at game start?
void ApplicationUI::unpause()
{
	if (m_phase == RUN) {
		if (m_runPhase)
			m_runPhase->resume();
		else
			qDebug("BAD: ApplicationUI::unpause called in run state with no runphase object!");
//		m_timer.start();
//		if (m_progressAnimation && m_robot && !m_robot->finished())
//			m_progressAnimation->play();
	}
}

// Prepare for the run phase.
// - Set up all the function slideout containers
// - Change the phase
// - Prevent the screen from dimming
// - Start the finish timer to give some time before the real timer starts.
void ApplicationUI::compilePhaseDone()
{
	Container *container = 0;
	for (int i=1; i<=3; i++) { // FIXME: Hard-coded 3 function limit
		for (int j=0; j<DEFAULT_FUNCTION_SIZE; j++) {
			container = m_gamePage->findChild<Container*>(QString("func%1_act%2").arg(i).arg(j+1));
			if (container)
				container->setProperty("actionVisible", false);
			else
				qDebug() << "No container at " << QString("func%1_act%2").arg(i).arg(j+1);
		}
	}

	// Only populate containers for functions, not the main function
	for (int i=1; i<m_functionCount; i++) {
		Function *f = m_functions[i];
		for (int j=0; j<f->commandCount(); j++) {
			container = m_gamePage->findChild<Container*>(QString("func%1_act%2").arg(i).arg(j + 1 + (DEFAULT_FUNCTION_SIZE - f->commandCount())));
			if (container) {
				container->setProperty("imageSource", getImageForCommand(f->at(j)));
				container->setProperty("actionVisible", true);
			} else
				qDebug() << "No container at " << QString("func%1_act%2").arg(i).arg(j + 1 + (DEFAULT_FUNCTION_SIZE - f->commandCount()));
		}
	}

	m_phase = RUN;
	Application::instance()->mainWindow()->setScreenIdleMode(ScreenIdleMode::KeepAwake);
	m_mediaPlayer->stop();
	m_runPhase->init(m_robot, m_queueManager, m_functions);

//	m_finishTimer.start(3000);
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
	m_tutorialManager.setTutorial(levelData["tutorial"].toInt()); // will return 0 if no tutorial field

	// Sanity check for reasonable map data
	if (width < 0 || width > MAX_WIDTH || height < 0 || height > MAX_HEIGHT || width * height != mapData.count()) {
		qDebug("Bad level data: %dx%d with %d elements\n", width, height, mapData.count());
		return;
	}

	int functionCount = 4; // By default we have a main method and three functions
	if (levelData.contains("numFunctions"))
		functionCount = levelData["numFunctions"].toInt() + 1; // but we can reduce the number in the level file.
	if (functionCount < 0) functionCount = 1; // We must have at least one function
	if (functionCount > 3) functionCount = 4; // since that's the main method.

	// Add main method
	Function *f = new Function(moves); // FIXME: Level 8 has too many moves!
	m_functions.append(f);

	// Limit optional functions - Only the optional functions can be limited. TODO?
	QVariantList functionLimits = levelData["functionLimit"].toList();
	for (int i=1; i<functionCount; i++) {
		if (i <= functionLimits.count())
			f = new Function(functionLimits[i-1].toInt());
		else
			f = new Function();
		m_functions.append(f);
		qDebug("Function %d limited to %d\n", i, f->count());
	}

	// Update the Q_PROPERTY so that the QML can update the visible buttons
	setFunctionCount(functionCount);
	qDebug("Function count: %d\n", functionCount);

	// Fill in map data
	int *data = new int[width*height];
	for (int i=0; i<width*height; i++) {
		data[i] = mapData[i].toInt();
	}

	// Create map and robot
	m_map = new Map(height, width, endX, endY, data, m_mapArea, this);
	Label *movesLabel = m_gamePage->findChild<Label*>("movesLeft");
	m_robot = new Robot(m_map, moves, movesLabel, startX, startY, endX, endY, Robot::getDirection(direction), this);

	// Set up planning phase function editor
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

	// Connect robot moves for runtime phase to handle when the robot has finished.
	QObject::connect(m_robot, SIGNAL(moved(int,int)), this, SLOT(robotMoved(int,int)));
}

void ApplicationUI::setupQueue()
{
	if (!m_queueManager)
		m_queueManager = new QueueManager(this, m_gamePage, this);

	m_queueManager->reset();
//	if (!m_qmlQueueCommand) {
//		m_qmlQueueCommand = QmlDocument::create("asset:///QueueCommand.qml").parent(this);
//		m_qmlQueueCommand->setContextProperty("_app", this);
//	}
//
//	m_queueContainer = m_gamePage->findChild<Container*>("queueContainer");
//	if (!m_queueContainer) qFatal("Failed to find queue container");
//
//	m_queue.clear();
//	m_queueContainer->removeAll();
//	m_queueCount = 0;
//
//	for (int i=0; i<QUEUE_LIMIT; i++) {
//		m_queueCommands[i] = CMD_EMPTY;
//		Container *queueObject = m_qmlQueueCommand->createRootObject<Container>();
//		queueObject->setProperty("index", i);
//		queueObject->setProperty("text", QString::number(m_queueCommands[i]));
//		m_queueContainer->add(queueObject);
//		m_queue.append(queueObject);
//	}
}

// Main entry point into a new level. This is what gets called from the level select listview.
void ApplicationUI::startLevel(const QVariantList &indexPath)
{
	// We only need to create the game page once.
	if (!m_gamePage) {
		QmlDocument *qml = QmlDocument::create("asset:///Game.qml").parent(this);
		qml->setContextProperty("_app", this);
		m_gamePage = qml->createRootObject<Page>();
		m_tutorialManager.init(m_gamePage);
		m_progressAnimation = m_gamePage->findChild<SequentialAnimation*>("progressAnimation");
	}

	if (indexPath.count() > 0)
		m_levelIndex = indexPath[0].toInt();
	else
		m_levelIndex = 0; // Uhoh?

	m_phase = COMPILE;
	m_selectedFunction = 0;
	setShouldShowFunctions(false);
	setIsInFunction(0);
	Container *compileContainer = m_gamePage->findChild<Container*>("compilePhaseContainer");
	compileContainer->setVisible(true);

	m_tutorialManager.reset();
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

//	m_timer.setInterval(2000);

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

void ApplicationUI::addQueuedCommand(CommandType type)
{
	if (type == CMD_EMPTY) return;

	if (m_phase == RUN) {
		m_runPhase->onCommand(type);
	} else if (m_phase == COMPILE) {
		qDebug("Add command to function %d\n", m_selectedFunction);
		if (m_selectedFunction >= 0 && m_selectedFunction <= m_functions.count()) {
			m_functions[m_selectedFunction]->append(type);
			drawSelectedFunction();
			computePath();
		}
	}
}

void ApplicationUI::computePath()
{
	m_map->eraseOverlay();
	if (m_functions[0]->count() <= 0) {
		return;
	}

	Robot::Direction dir = m_robot->direction();
	int x = m_robot->x();
	int y = m_robot->y();
	QStack<FunctionRunner*> stack;
	stack.push(new FunctionRunner(0, m_functions[0]));
	FunctionRunner *frame = 0;
	int moveLimit = m_functions[0]->count();
	int moveCount = 0;
	while (moveCount < moveLimit && !stack.empty()) {
		frame = stack.top();
		while (frame->finished()) {
			moveCount++;
//			qDebug("COMPUTE_PATH: moveCount = %d\n", moveCount);
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
				switch (dir) {
				case Robot::UP:
					if (m_map->positionAvailable(x,y-1)) {
						m_map->drawOverlayConnection(x, y, x, y-1);
						y = y-1;
					}
					break;
				case Robot::DOWN:
					if (m_map->positionAvailable(x,y+1)) {
						m_map->drawOverlayConnection(x, y, x, y+1);
						y = y+1;
					}
					break;
				case Robot::LEFT:
					if (m_map->positionAvailable(x-1,y)) {
						m_map->drawOverlayConnection(x, y, x-1, y);
						x = x-1;
					}
					break;
				case Robot::RIGHT:
					if (m_map->positionAvailable(x+1,y)) {
						m_map->drawOverlayConnection(x, y, x+1, y);
						x = x+1;
					}
					break;
				}

				break;
			case ApplicationUI::CMD_LEFT:
				switch (dir) {
				case Robot::UP:
					dir = Robot::LEFT;
					break;
				case Robot::LEFT:
					dir = Robot::DOWN;
					break;
				case Robot::DOWN:
					dir = Robot::RIGHT;
					break;
				case Robot::RIGHT:
					dir = Robot::UP;
					break;
				}
				break;
			case ApplicationUI::CMD_RIGHT:
				switch (dir) {
				case Robot::UP:
					dir = Robot::RIGHT;
					break;
				case Robot::LEFT:
					dir = Robot::UP;
					break;
				case Robot::DOWN:
					dir = Robot::LEFT;
					break;
				case Robot::RIGHT:
					dir = Robot::DOWN;
					break;
				}
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
				// We can only push functions on if we have space.
				if (moveCount < moveLimit) {
//					qDebug("COMPUTE_PATH: pushing function, moveCount = %d\n", moveCount);
					stack.push(
						new FunctionRunner(functionIndex,
								m_functions[functionIndex]));
				} else {
					moveCount++; // otherwise terminate here.
//					qDebug("COMPUTE_PATH: NOT pushing function, moveCount = %d\n", moveCount);
				}
			} else if (stack.size() == 1) {
				// Moves count in the main method but we won't count function calls until they're done.
				moveCount++;
//				qDebug("COMPUTE_PATH: Moves count in the main, moveCount = %d\n", moveCount);
			}
		}
	}
}

void ApplicationUI::drawSelectedFunction()
{
	qDebug("Drawing selected function %d\n", m_selectedFunction);
	switch (m_selectedFunction) {
	case 0:
		m_functionHeader->setProperty("imageSource", "asset:///images/main.png");
		break;
	case 1:
		m_functionHeader->setProperty("imageSource", "asset:///images/f1.png");
		break;
	case 2:
		m_functionHeader->setProperty("imageSource", "asset:///images/f2.png");
		break;
	case 3:
		m_functionHeader->setProperty("imageSource", "asset:///images/f3.png");
		break;
	}

	int i=0;
	Function *f = m_functions[m_selectedFunction];
	qDebug("Function[%d] count: %d\n", m_selectedFunction, f->count());
	for (; i<f->count(); i++) {
		qDebug("Setting source for action[%d] from source[%d] (%d)\n", i+1, i, (*f)[i]);
		m_functionActions[i]->setProperty("imageSource", getImageForCommand((*f)[i]));
		m_functionActions[i]->setProperty("showing", true);
	}
	for (; i<m_functionActions.count(); i++) {
		qDebug("Hiding action[%d]\n", i+1);
		m_functionActions[i]->setProperty("showing", false);
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

//void ApplicationUI::removeQueuedCommand(int index, bool force)
//{
//	m_queueManager->remove(index, force);
////	qDebug() << "Removing queue command at " << index << " (queueCount = " << m_queueCount << ")";
////	if (index < 0) return;
////	if (index >= m_queueCount) return;
////	if (m_queueCommands[index] == CMD_EMPTY) return;
////	if (!force && index == 0 && !m_stack.empty()) return;
////
////	for (int i=index; i<QUEUE_LIMIT-1; i++) {
////		setQueueValue(i, m_queueCommands[i+1]);
////	}
////
////	setQueueValue(QUEUE_LIMIT-1, CMD_EMPTY);
////	m_queueCount--;
//}

void ApplicationUI::removeFunctionCommand(int index)
{
	qDebug("Removing %d from functions[%d]\n", index, m_selectedFunction);
	m_functions[m_selectedFunction]->remove(index);
	drawSelectedFunction();
	computePath();
}

void ApplicationUI::highlightFunction(int function, int pc)
{
	if (m_highlightedContainer) {
		m_highlightedContainer->setProperty("highlighted", false);
	}

	Container *container = m_gamePage->findChild<Container*>(QString("func%1_act%2").arg(function).arg((DEFAULT_FUNCTION_SIZE-m_functions[function]->commandCount())+pc+1));
	if (container) {
		m_highlightedContainer = container;
		m_highlightedContainer->setProperty("highlighted", true);
	} else {
		qDebug() << "Unable to find container: " << QString("func%1_act%2").arg(function).arg((DEFAULT_FUNCTION_SIZE-m_functions[function]->commandCount())+pc+1);
	}
}

void ApplicationUI::timerFired()
{
//	m_runPhase->timerFired(m_robot);
//	FunctionRunner *frame = 0;
//	bool shouldRemove = true;
//	bool countsAsMove = true;
//	CommandType cmd = m_queueManager->peek();
//
//	if (!m_stack.empty()) {
//		frame = m_stack.top();
//		while (frame->finished()) {
//			frame = m_stack.pop();
//			delete frame;
//			if (m_stack.empty()) {
//				// Finished all function calls, remove the function on the queue
//				m_queueManager->remove(0, true);
//				cmd = m_queueManager->peek();
//				break;
//			}
//			frame = m_stack.top();
//		}
//
//		if (!m_stack.empty()) {
//			setIsInFunction(frame->function());
//			highlightFunction(frame->function(), frame->pc());
//			cmd = frame->step();
//			countsAsMove = false;
//			shouldRemove = false;
//		}
//
//		frame = 0; // We re-use this later if the command was a function call.
//	}
//
//	if (m_stack.empty()) {
//		setIsInFunction(0);
//		if (m_highlightedContainer) {
//			m_highlightedContainer->setProperty("highlighted", false);
//		}
//	}
//
//	qDebug() << "Cmd: " << cmd;
//	switch (cmd) {
//	case CMD_FORWARD:
//		m_robot->moveForward();
//		break;
//	case CMD_LEFT:
//		m_robot->turnLeft();
//		break;
//	case CMD_RIGHT:
//		m_robot->turnRight();
//		break;
//	case CMD_F1:
//		frame = new FunctionRunner(1, m_functions[1]);
//		countsAsMove = true;
//		shouldRemove = false;
//		break;
//	case CMD_F2:
//		frame = new FunctionRunner(2, m_functions[2]);
//		countsAsMove = true;
//		shouldRemove = false;
//		break;
//	case CMD_F3:
//		frame = new FunctionRunner(3, m_functions[3]);
//		countsAsMove = true;
//		shouldRemove = false;
//		break;
//	default:
//		break;
//	}
//
//	if (frame) {
//		m_stack.push(frame);
//	}
//
//	if (shouldRemove)
//		m_queueManager->remove(0, true);
//
//	if (countsAsMove) {
//		m_robot->decrementMoves();
//		if (m_robot->hasNoPower(!m_stack.empty())) {
//			m_robot->setImageForPower(false);
//			QTimer::singleShot(500, this, SLOT(processFinish()));
//			//processFinish(m_robot->finished());
//			return;
//		}
//	}
//	if (m_runPhase->hasNoMoreActions()) {
//		QTimer::singleShot(500, this, SLOT(processFinish()));
//		return;
//	}
//
//	if (!m_robot->finished())
//		m_progressAnimation->play();
//	else
//		QTimer::singleShot(500, this, SLOT(processFinish()));
}

void ApplicationUI::onFinished()
{
	QTimer::singleShot(500, this, SLOT(processFinish()));
}

void ApplicationUI::onRestartAnimation()
{
	m_progressAnimation->play();
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
		m_gameState.setLevelComplete(m_levelIndex);
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
//	if (m_robot->finished()) {
//		qDebug() << "Finished";
//		m_timer.stop();
//	}
}

void ApplicationUI::nextLevel()
{
	int levelCount = m_levelList->dataModel()->childCount(QVariantList());
	m_mediaPlayer->play();
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
	m_mediaPlayer->play();
	indexPath.append(m_levelIndex);
	startLevel(indexPath);
}
