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
	m_time.start();
	QTimer::start();
}

void ElapsedTimer::start(int msec)
{
	m_time.start();
	QTimer::start(msec);
}

int ElapsedTimer::remaining()
{
	return interval() - m_time.elapsed();
}

void ElapsedTimer::myreset()
{
	m_time.restart();
}


