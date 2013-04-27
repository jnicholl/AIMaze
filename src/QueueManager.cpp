/*
 * QueueManager.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: jnicholl
 */

#include "QueueManager.h"

#include <bb/cascades/Container>
#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

QueueManager::QueueManager(const ApplicationUI *app, const Page *page, QObject *parent)
	: QObject(parent)
	, m_app(app)
{
	m_qmlDoc = QmlDocument::create("asset:///QueueCommand.qml").parent(this);
	m_qmlDoc->setContextProperty("_app", this);
	m_container = page->findChild<Container*>("queueContainer");
	if (!m_container)
		qFatal("QueueManager: Failed to find queue container");

	reset();
}

void QueueManager::reset()
{
	m_queue.clear();
	m_container->removeAll();
	m_queueCount = 0;

	for (int i=0; i<QUEUE_LIMIT; i++) {
		m_queueCommands[i] = ApplicationUI::CMD_EMPTY;
		Container *c = m_qmlDoc->createRootObject<Container>();
		c->setProperty("index", i);
		c->setProperty("text", QString::number(m_queueCommands[i]));
		m_container->add(c);
		m_queue.append(c);
	}
}

void QueueManager::setValue(int i, ApplicationUI::CommandType type)
{
	qDebug() << "Set " << i << " from " << m_queueCommands[i] << " to " << type;
	m_queueCommands[i] = type;
	m_queue[i]->setProperty("imageSource", ApplicationUI::getImageForCommand(type));
}

void QueueManager::add(ApplicationUI::CommandType type)
{
	if (type == ApplicationUI::CMD_EMPTY)
		return;

	for (int i=0; i<QUEUE_LIMIT; i++) {
		if (m_queueCommands[i] == ApplicationUI::CMD_EMPTY) {
			setValue(i, type);
			m_queueCount++;
			break;
		}
	}
	// TODO: return failure to show user queue full?
}

void QueueManager::remove(int index, bool force)
{
	qDebug() << "Removing queue command at " << index << " (queueCount = " << m_queueCount << ")";
	if (index < 0) return;
	if (index >= m_queueCount) return;
	if (m_queueCommands[index] == ApplicationUI::CMD_EMPTY) return;

	// Avoid removing function calls from queue unless we force it.
	if (!force && index == 0 && !m_app->isFunctionStackEmpty()) return;

	for (int i=index; i<QUEUE_LIMIT-1; i++) {
		setValue(i, m_queueCommands[i+1]);
	}

	setValue(QUEUE_LIMIT-1, ApplicationUI::CMD_EMPTY);
	m_queueCount--;
}

ApplicationUI::CommandType QueueManager::peek() const
{
	return m_queueCommands[0];
}
