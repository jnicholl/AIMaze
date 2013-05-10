/*
 * QueueManager.h
 *
 *  Created on: Apr 27, 2013
 *      Author: jnicholl
 */

#ifndef QUEUEMANAGER_H_
#define QUEUEMANAGER_H_

#include <QtCore/QObject>

#include "applicationui.hpp"

namespace bb {
	namespace cascades {
		class Container;
		class Page;
		class ParallelAnimation;
	}
}

class QueueManager: public QObject {
	Q_OBJECT
	Q_PROPERTY(float offset READ offset NOTIFY offsetChanged)
public:
	QueueManager(const ApplicationUI *app, const bb::cascades::Page *page, QObject *parent=0);

	void reset();
	void doneActions();

	void setValue(int i, ApplicationUI::CommandType type);

	// Increment the offset by 1 and start the 500ms animation to get there.
	void animate();
	void resetAnimation();

	Q_INVOKABLE void remove(int index, bool force = false);
	void add(ApplicationUI::CommandType type, int seqNum = -1);
	ApplicationUI::CommandType peek() const;
	bool empty() const { return m_queueCount == 0; }

	float offset() const { return m_offset; }
	void setOffset(float offset) {
		if (m_offset != offset) {
			m_offset = offset;
			emit offsetChanged(offset);
		}
	}
	void showHit(int level);

signals:
	void spaceAvailable(int);
	void moveLeft();
	void offsetChanged(float);

private:
	const ApplicationUI *m_app;
	bb::cascades::QmlDocument *m_qmlDoc;
	bb::cascades::Container *m_container;
	int m_queueCount;
	QList<ApplicationUI::CommandType> m_queueCommands;
	QList<bb::cascades::Container*> m_queue;
	bb::cascades::ParallelAnimation *m_animation;
	float m_offset;
};

#endif /* QUEUEMANAGER_H_ */
