/*
 * ElapsedTimer.cpp
 *
 *  Created on: May 1, 2013
 *      Author: Jeremy
 */

#include "ElapsedTimer.h"

ElapsedTimer::ElapsedTimer(QObject* parent)
	: QTimer(parent)
{
	QObject::connect(this, SIGNAL(timeout()), this, SLOT(myreset()));
}

void ElapsedTimer::start()
{
	qDebug("Start");
	m_time.start();
	QTimer::start();
}

void ElapsedTimer::start(int msec)
{
	qDebug("Start");
	m_time.start();
	QTimer::start(msec);
}

int ElapsedTimer::remaining()
{
	return interval() - m_time.elapsed();
}

void ElapsedTimer::myreset()
{
	qDebug("Reset");
	m_time.restart();
}


