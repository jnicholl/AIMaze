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
		if (state.contains("scores")) {
			QVariantList scores = state["scores"].toList();
			for (int i=0; i<m_levelAvailable; i++) {
				if (i < scores.count())
					m_scores.append(scores[i].toInt());
				else
					m_scores.append(0);
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
	QVariantList scores;
	for (int i=0; i<m_scores.count(); i++) {
		scores.append(m_scores[i]);
	}
	state["scores"] = scores;
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

void GameState::setLevelComplete(int level, int score)
{
	qDebug("Level available: %d, just complete: %d, score: %d\n", m_levelAvailable, level, score);
	bool doSave = false;
	if (m_levelAvailable <= level) {
		m_levelAvailable = level + 1;
		doSave = true;
		emit gameStateChanged();
	}

	if (m_levelAvailable >= m_scores.count()) {
		for (int i=m_scores.count(); i<=m_levelAvailable; i++) {
			m_scores.append(0);
		}
	}

	if (m_scores[level] < score) {
		m_scores[level] = score;
		qDebug("Scores[%d] = %d\n", level, score);
		doSave = true;
		emit scoresChanged(level);
	}

	if (doSave) {
		save();
	}
}
