/*
 * MusicPlayer.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: Jeremy
 */

#include "MusicPlayer.h"

#include <bb/multimedia/MediaPlayer>

using namespace bb::multimedia;

MusicPlayer::MusicPlayer(QObject *parent)
	: QObject(parent)
	, m_mediaPlayer(new MediaPlayer(this))
	, m_playing(false)
{
	QObject::connect(m_mediaPlayer, SIGNAL(playbackCompleted()), this, SLOT(loop()));
}

void MusicPlayer::setSourceUrl(const QUrl &url)
{
	m_mediaPlayer->setSourceUrl(url);
}

void MusicPlayer::play()
{
	if (!m_playing) {
		m_playing = true;
		m_mediaPlayer->play();
	}
}

void MusicPlayer::stop()
{
	if (m_playing) {
		m_playing = false;
		m_mediaPlayer->stop();
	}
}

void MusicPlayer::loop()
{
	if (m_playing) {
		m_mediaPlayer->play();
	}
}


