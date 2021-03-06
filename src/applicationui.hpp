// Navigation pane project template
#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>
#include <QtCore/QList>
#include <QtCore/QTimer>
#include <bb/cascades/OrientationSupport>
#include <bb/device/DisplayInfo>

namespace bb {
	namespace cascades {
		class Application;
		class Container;
		class ListView;
		class NavigationPane;
		class Page;
		class QmlDocument;
		class SequentialAnimation;
	}
}

class Map;
class Robot;
class Function;
class FunctionRunner;

#define QUEUE_LIMIT 5

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int functionCount READ functionCount NOTIFY functionCountChanged)
    Q_PROPERTY(int levelAvailable READ levelAvailable NOTIFY levelAvailableChanged)
    Q_PROPERTY(bool showFunctions READ shouldShowFunctions NOTIFY showFunctionsChanged)
    Q_PROPERTY(bool isInF1 READ isInF1 NOTIFY isInF1Changed)
    Q_PROPERTY(bool isInF2 READ isInF2 NOTIFY isInF2Changed)
    Q_PROPERTY(bool isInF3 READ isInF3 NOTIFY isInF3Changed)
    Q_PROPERTY(int tutorial READ tutorial WRITE setTutorial NOTIFY tutorialChanged)

public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() {}

    enum CommandType {
    	CMD_EMPTY = 0,
    	CMD_FORWARD = 1,
    	CMD_LEFT = 2,
    	CMD_RIGHT = 3,
    	CMD_F1 = 4,
    	CMD_F2 = 5,
    	CMD_F3 = 6,
    	CMD_BLOCKED = 255
    };

    void addQueuedCommand(CommandType type);

    Q_SLOT void back();
    Q_SLOT void unpause();
    Q_SLOT void pause();
    Q_SLOT void compilePhaseDone();

    Q_SLOT void nextLevel();
    Q_SLOT void retry();
    Q_SLOT void clickMenuButton();
    Q_SLOT void showMenu();

    Q_SLOT void startLevel(const QVariantList &indexPath);

    // Handle sidebar commands
    Q_SLOT void tapForward();
    Q_SLOT void tapLeft();
    Q_SLOT void tapRight();
    Q_SLOT void tapF1();
    Q_SLOT void tapF2();
    Q_SLOT void tapF3();
    Q_SLOT void tapViewFunctions();

    // Click on queued command
    Q_SLOT void removeQueuedCommand(int index, bool force=false);

    // Click on function command (to remove it)
    Q_SLOT void removeFunctionCommand(int index);
    Q_SLOT void selectNextFunction();

    Q_SLOT void timerFired();

    Q_SLOT void robotMoved(int x, int y);

    void drawSelectedFunction();

    int functionCount() const { return m_functionCount; }
    void setFunctionCount(int count) {
    	if (count != m_functionCount) {
    		m_functionCount = count;
    		emit functionCountChanged(m_functionCount);
    	}
    }

    int levelAvailable() const { return m_levelAvailable; }
    void setLevelAvailable(int available) {
    	if (available != m_levelAvailable) {
    		m_levelAvailable = available;
    		emit levelAvailableChanged(m_levelAvailable);
    	}
    }

    bool shouldShowFunctions() const { return m_shouldShowFunctions; }
    void setShouldShowFunctions(bool val) {
    	if (val != m_shouldShowFunctions) {
    		m_shouldShowFunctions = val;
    		emit showFunctionsChanged(m_shouldShowFunctions);
    	}
    }

    bool isInF1() const { return m_isInF1; }
    void setIsInF1(bool val) {
    	if (val != m_isInF1) {
    		m_isInF1 = val;
    		emit isInF1Changed(m_isInF1);
    	}
    }
    bool isInF2() const { return m_isInF2; }
    void setIsInF2(bool val) {
    	if (val != m_isInF2) {
    		m_isInF2 = val;
    		emit isInF2Changed(m_isInF2);
    	}
    }
    bool isInF3() const { return m_isInF3; }
    void setIsInF3(bool val) {
    	if (val != m_isInF3) {
    		m_isInF3 = val;
    		emit isInF3Changed(m_isInF3);
    	}
    }
    void setIsInFunction(int index) {
    	qDebug() << "set is in function " << index;
    	switch (index) {
    	case 0:
			setIsInF1(true);
			setIsInF2(false);
			setIsInF3(false);
			break;
    	case 1:
			setIsInF1(false);
			setIsInF2(true);
			setIsInF3(false);
			break;
    	case 2:
			setIsInF1(false);
			setIsInF2(false);
			setIsInF3(true);
			break;
    	default:
    		setIsInF1(false);
    		setIsInF2(false);
    		setIsInF3(false);
    		break;
    	}
    }

    Q_SLOT void processFinish();

    int tutorial() const { return m_tutorial; }
    void setTutorial(int tutorial) {
    	if (tutorial != m_tutorial) {
    		m_tutorial = tutorial;
    		emit tutorialChanged(m_tutorial);
    	}
    }

signals:
	void functionCountChanged(int);
	void levelAvailableChanged(int);
	void showFunctionsChanged(bool);
	void isInF1Changed(bool);
	void isInF2Changed(bool);
	void isInF3Changed(bool);
	void tutorialChanged(int);

private:
	void loadSavedState();
	void saveState();

    void setQueueValue(int i, CommandType type);
    void setupQueue();

    void setupLevel(const QVariantMap &levelData);

    void highlightFunction(int func, int pc);

    static QString getImageForCommand(CommandType type);

    bb::cascades::ListView *m_levelList;
    bb::cascades::Page *m_gamePage;
    bb::cascades::NavigationPane *m_navigationPane;
    bb::cascades::QmlDocument *m_qmlQueueCommand;
    bb::cascades::Container *m_queueContainer;
    bb::cascades::Container *m_mapArea;

    bb::cascades::SequentialAnimation *m_progressAnimation;

    int m_queueCount;
    CommandType m_queueCommands[QUEUE_LIMIT];
    QList<bb::cascades::Container*> m_queue;
    QTimer m_timer;
    QTimer m_finishTimer;

    int m_levelIndex;
    int m_levelAvailable;
    Map *m_map;
    Robot *m_robot;

    enum Phase {
    	COMPILE,
    	RUN,
    	FINISHED,
    	MENU
    } m_phase;

    int m_movesRemaining;
    int m_functionCount;
    QList<Function*> m_functions;
    int m_selectedFunction;
    bb::cascades::Container *m_functionHeader;
    QList<bb::cascades::Container*> m_functionActions;

    bool m_shouldShowFunctions;

    QStack<FunctionRunner*> m_stack;

    bool m_isInF1;
    bool m_isInF2;
    bool m_isInF3;
    bb::cascades::Container *m_highlightedContainer;

    int m_tutorial; // 0 for no tutorial, otherwise 1,2,3.
};

#endif /* ApplicationUI_HPP_ */
