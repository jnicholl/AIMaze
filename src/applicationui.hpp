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
    Q_SLOT void removeQueuedCommand(int index);

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
    		functionCountChanged(m_functionCount);
    	}
    }

signals:
	void functionCountChanged(int);

private:
    void setQueueValue(int i, CommandType type);
    void setupQueue();

    void setupLevel(const QVariantMap &levelData);

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

    Map *m_map;
    Robot *m_robot;

    enum Phase {
    	COMPILE,
    	RUN
    } m_phase;

    int m_functionCount;
    QList<Function*> m_functions;
    int m_selectedFunction;
    bb::cascades::Container *m_functionHeader;
    QList<bb::cascades::Container*> m_functionActions;

    QStack<FunctionRunner*> m_stack;
};

#endif /* ApplicationUI_HPP_ */
