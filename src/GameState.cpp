/*
 * GameState.cpp
 *
 *  Created on: Apr 26, 2013
 *      Author: jnicholl
 */

#include "GameState.h"

#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>
#include <QtCore/QDir>

#include <bb/data/JsonDataAccess>

using namespace bb::data;

GameState::GameState()
	: m_levelAvailable(0)
{
}

void GameState::load()
{
	QString filePath(QDir::currentPath() + "/data/levelState.json");
	if (QFile::exists(filePath)) {
		JsonDataAccess jda;
		QVariantMap state = jda.load(filePath).toMap();
		if (jda.hasError())
			return;
		if (state.contains("levelAvailable")) {
			int levelAvailable = state["levelAvailable"].toInt();
			if (levelAvailable > 0) {
				m_levelAvailable = levelAvailable;
				emit gameStateChanged();
			}
		}
	}
	qDebug() << "Level available" << m_levelAvailable;
}

void GameState::save()
{
	qDebug() << "Saving state to " << QDir::currentPath() + "/data/levelState.json";
	QVariantMap state;
	state["levelAvailable"] = m_levelAvailable;
	QFile file(QDir::currentPath() + "/data/levelState.json");
	if (file.open(QIODevice::WriteOnly)) {
		JsonDataAccess jda;
		jda.save(state, &file);
		if (jda.hasError()) {
			qDebug("Failed to save: Json error\n");
		} else {
			qDebug("Saved");
		}
	}
}

int GameState::levelAvailable() const
{
	return m_levelAvailable;
}

void GameState::setLevelComplete(int level)
{
	qDebug("Level available: %d, just complete: %d\n", m_levelAvailable, level);
	if (m_levelAvailable <= level) {
		m_levelAvailable = level + 1;
		save();
		emit gameStateChanged();
	}
}
