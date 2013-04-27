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
	}
}

class QueueManager: public QObject {
	Q_OBJECT
public:
	QueueManager(const ApplicationUI *app, const bb::cascades::Page *page, QObject *parent=0);

	void reset();

	void setValue(int i, ApplicationUI::CommandType type);
	Q_INVOKABLE void remove(int index, bool force = false);
	void add(ApplicationUI::CommandType type);
	ApplicationUI::CommandType peek() const;

private:
	const ApplicationUI *m_app;
	bb::cascades::QmlDocument *m_qmlDoc;
	bb::cascades::Container *m_container;
	int m_queueCount;
	ApplicationUI::CommandType m_queueCommands[QUEUE_LIMIT];
	QList<bb::cascades::Container*> m_queue;
};

#endif /* QUEUEMANAGER_H_ */
