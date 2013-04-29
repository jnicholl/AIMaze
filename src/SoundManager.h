/*
 * SoundManager.h
 *
 *  Created on: Apr 28, 2013
 *      Author: jnicholl
 */

#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

#include <QtCore/QObject>
#include <QtCore/QVector>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <vorbis/vorbisfile.h>

#define SOUNDMANAGER_MAX_SOURCES 16

class SoundManager: public QObject {
	Q_OBJECT
public:
	SoundManager(QObject *parent=0);
	~SoundManager();

	/**
	 * Plays a sound.
	 *
	 * @param fileName the name of the file in the soundDirectory.
	 */
	Q_INVOKABLE void play(ALuint index);

	/**
	 * Plays a sound, with modified pitch and gain.
	 *
	 * @param fileName the name of the file in the soundDirectory
	 * @param pitch specifies the pitch to be applied to a sound Range: [0.5-2.0]
	 * @param gain sound gain (volume amplification) Range: ]0.0-  ]
	 */
	bool play(ALuint index, float pitch, float gain);

	bool loadTrack(const QString &filename, ALuint *trackID);
	void unloadSounds();

private:

	// Load the .wav files
	bool loadWav(FILE* file, ALuint buffer);

	// Load the .ogg files
	bool loadOgg(FILE* file, ALuint buffer);

	// Sound buffers.
	QVector<ALuint> m_soundBuffers;

	ALuint m_sources[SOUNDMANAGER_MAX_SOURCES];
};

#endif /* SOUNDMANAGER_H_ */
