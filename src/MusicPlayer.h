/*
 * MusicPlayer.h
 *
 *  Created on: Apr 29, 2013
 *      Author: Jeremy
 */

#ifndef MUSICPLAYER_H_
#define MUSICPLAYER_H_

#include <QtCore/QObject>
#include <QtCore/QUrl>

namespace bb {
	namespace multimedia {
		class MediaPlayer;
	}
}

class MusicPlayer: public QObject {
	Q_OBJECT
public:
	MusicPlayer(QObject *parent = 0);
	void setSourceUrl(const QUrl &url);
	Q_SLOT void play();
	Q_SLOT void stop();
private:
	Q_SLOT void loop();

	bb::multimedia::MediaPlayer *m_mediaPlayer;
	bool m_playing;
};

#endif /* MUSICPLAYER_H_ */
