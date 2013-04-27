/*
 * TutorialManager.h
 *
 *  Created on: Apr 26, 2013
 *      Author: jnicholl
 */

#ifndef TUTORIALMANAGER_H_
#define TUTORIALMANAGER_H_

#include <QtCore/QObject>
#include <QtCore/QList>

namespace bb {
	namespace cascades {
		class Container;
		class Page;
	}
}

class TutorialManager: public QObject {
	Q_OBJECT
public:
	TutorialManager();

	// Initialized once when the page is created
	void init(const bb::cascades::Page *page);
	// Reset each time between levels
	void reset();

	int tutorial() const { return m_current; }
	void setTutorial(int tutorial) {
		if (m_current != tutorial) {
			qDebug("Setting tutorial = %d\n", tutorial);
			m_current = tutorial;
			emit tutorialChanged(m_current);
		}
	}

signals:
	void tutorialChanged(int);

private:
	QList<bb::cascades::Container*> m_containers;
	int m_current; // 0 for no tutorial, otherwise 1,2,3.
};

#endif /* TUTORIALMANAGER_H_ */
