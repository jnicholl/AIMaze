/*
 * ElapsedTimer.h
 *
 *  Created on: May 1, 2013
 *      Author: Jeremy
 */

#ifndef ELAPSEDTIMER_H_
#define ELAPSEDTIMER_H_

#include <QtCore/QTimer>
#include <QtCore/QTime>

class ElapsedTimer: public QTimer {
public:
	ElapsedTimer(QObject* parent=0);

	void start();
	void start(int msec);
	int remaining();

private slots:
	void reset();

private:
	QTime m_time;
};

#endif /* ELAPSEDTIMER_H_ */
