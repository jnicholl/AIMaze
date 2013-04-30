/*
 * ddrmanager.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: Jeremy
 */

#include "ddrmanager.h"

#include <QDir>
#include <bb/data/JsonDataAccess>

#include "soundmanager.h"


DDRManager::DDRManager(SoundManager *soundMgr, QObject *parent)
	: QObject(parent)
{
	m_soundMgr = soundMgr;
}

DDRManager::~DDRManager()
{
}

bool DDRManager::loadLevelSounds()
{
	unsigned i=0;

	m_tracks.clear();
	m_soundMgr->unloadSounds();

	bb::data::JsonDataAccess jda;
	QVariantMap levelData = jda.load(QDir::currentPath() + "/app/native/assets/sounds/level1.json").toMap();
	if (jda.hasError()) {
		qDebug("Failed to load level data");
		return false;
	}

	qDebug("Found level data");
	unsigned track;
	if (!m_soundMgr->loadTrack(QDir::currentPath() + "/app/native/assets/sounds/" + levelData["start"].toString(), &track))
		goto load_fail;
	qDebug("Loaded first track");
	m_tracks.append(track);
	if (!m_soundMgr->loadTrack(QDir::currentPath() + "/app/native/assets/sounds/" + levelData["win"].toString(), &m_winTrack))
		goto load_fail;
	if (!m_soundMgr->loadTrack(QDir::currentPath() + "/app/native/assets/sounds/" + levelData["lose"].toString(), &m_loseTrack))
		goto load_fail;

	qDebug("Loaded ending tracks");
	m_trackCount = levelData["tracks"].toInt();
	for (i=0; i<m_trackCount; i++) {
		if (!m_soundMgr->loadTrack(QDir::currentPath() + "/app/native/assets/sounds/" + levelData["trackname"].toString().arg(i+1), &track))
			goto load_fail;
		qDebug("Loaded main track %d", i+1);
		m_tracks.append(track);
	}
	return true;

load_fail:
	m_soundMgr->unloadSounds();
	return false;
}

//void DDRManager::start()
//{
//	if (m_tracks.empty())
//		return;
//
//	m_repeatTrack = false;
//	m_win = false;
//	m_state = START;
//	m_soundMgr->play(m_tracks[0]);
//}
//
//void DDRManager::onTimer()
//{
//	if (m_state == WIN || m_state == LOSE) {
//		qCritical("Timer should not be firing in WIN or LOSE state!");
//	}
//
//	if (!m_repeatTrack || m_state == START) { // Never repeat the start track
//		if (m_state + 1 <= m_trackCount) // 0-based index
//			m_state++;
//		else {
//			if (m_win)
//				m_state = WIN;
//			else
//				m_state = LOSE;
//		}
//	}
//
//	switch (m_state) {
//	case WIN:
//		m_soundMgr->play(m_winTrack);
//		break;
//	case LOSE:
//		m_soundMgr->play(m_loseTrack);
//		break;
//	default:
//		m_soundMgr->play(m_tracks[m_state]);
//		break;
//	}
//}

void DDRManager::playClapTrack()
{
	m_soundMgr->play(m_tracks[0]);
}

void DDRManager::playEndTrack(bool won)
{
	if (won)
		m_soundMgr->play(m_winTrack);
	else
		m_soundMgr->play(m_loseTrack);
}

void DDRManager::playTrack() // FIXME: Use state?
{
	static int index = 1;
	index = (index+1)%2;
	m_soundMgr->play(m_tracks[index+1]);
}
