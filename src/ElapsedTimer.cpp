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
	connect(this, SIGNAL(timeout()), this, SLOT(reset()));
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

void ElapsedTimer::reset()
{
	m_time.restart();
}

