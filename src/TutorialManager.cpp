/*
 * TutorialManager.cpp
 *
 *  Created on: Apr 26, 2013
 *      Author: jnicholl
 */

#include "TutorialManager.h"

#include <bb/cascades/Page>
#include <bb/cascades/Container>

using namespace bb::cascades;

TutorialManager::TutorialManager() {
}

void TutorialManager::init(const Page *page)
{
	m_containers.append(page->findChild<Container*>("tutorial1Container"));
	m_containers.append(page->findChild<Container*>("tutorial2Container"));
	m_containers.append(page->findChild<Container*>("tutorial3Container"));
}

void TutorialManager::reset()
{
	foreach(Container *c, m_containers)
	{
		c->setProperty("state", 0);
	}
}
