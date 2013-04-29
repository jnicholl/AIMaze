/*
 * SoundManager.cpp
 *
 *  Created on: Apr 28, 2013
 *      Author: jnicholl
 */

#include "SoundManager.h"

#include "qdebug.h"

#define REPORT_OPENAL_ERROR(x) \
	do { \
		ALenum ___report_err = (x); \
		if (___report_err) { \
			qDebug() << "OpenAL error at " << __FILE__ << ":" << __LINE__ << ":\n" << alutGetErrorString(___report_err); \
		} \
	} while (0);

#define REPORT_OPENAL_ERROR_CLEANUP(x,cleanup) \
	do { \
		ALenum ___report_err = (x); \
		if (___report_err) { \
			qDebug() << "OpenAL error at " << __FILE__ << ":" << __LINE__ << ":\n" << alutGetErrorString(___report_err); \
			goto cleanup; \
		} \
	} while (0);

SoundManager::SoundManager(QObject *parent)
	: QObject(parent)
{
	if (!alutInit(NULL, NULL)) {
		REPORT_OPENAL_ERROR(alutGetError());
	}

	alGenSources(SOUNDMANAGER_MAX_SOURCES, m_sources);
	REPORT_OPENAL_ERROR(alGetError());
}

SoundManager::~SoundManager()
{
	for (int i=0; i<SOUNDMANAGER_MAX_SOURCES; i++) {
		alDeleteSources(1, &(m_sources[i]));
		REPORT_OPENAL_ERROR(alGetError());
	}

	unloadSounds();

	alutExit();
	REPORT_OPENAL_ERROR(alGetError());
}

void SoundManager::unloadSounds()
{
	ALuint buffer = 0;
	QVectorIterator<ALuint> iterator(m_soundBuffers);
	while (iterator.hasNext()) {
		buffer = iterator.next();
		alDeleteBuffers(1, &buffer);

		REPORT_OPENAL_ERROR(alGetError());
	}
	m_soundBuffers.clear();
}

bool SoundManager::loadTrack(const QString &filename, ALuint *trackID)
{
	qDebug("Loading track: %s\n", qPrintable(filename));
	FILE* file = 0;

	// Generate buffers to hold audio data.
	ALuint buffer;
	alGenBuffers(1, &buffer);

	REPORT_OPENAL_ERROR_CLEANUP(alGetError(), cleanup);

	// Load sound file.
	file = fopen(filename.toStdString().c_str(), "rb");
	if (!file) {
		qDebug() << "Failed to load audio file " << qPrintable(filename);
		goto cleanup;
	}

	// Read the file header
	char header[12];
	if (fread(header, 1, 12, file) != 12) {
		qDebug() << "Invalid header for audio file " << qPrintable(filename);
		alDeleteBuffers(1, &buffer);
		goto cleanup;
	}

	// Check the file format & load the buffer with audio data.
	if (memcmp(header, "RIFF", 4) == 0) {
		if (!loadWav(file, buffer)) {
			qDebug() << "Invalid wav file: " << qPrintable(filename);
			alDeleteBuffers(1, &buffer);
			goto cleanup;
		}
	}
	else if (memcmp(header, "OggS", 4) == 0) {
		if (!loadOgg(file, buffer)) {
			qDebug() << "Invalid ogg file: " << qPrintable(filename);
			alDeleteBuffers(1, &buffer);
			goto cleanup;
		}
	}
	else {
		qDebug() << "Unsupported audio file: " << qPrintable(filename);
		goto cleanup;
	}

	m_soundBuffers.append(buffer);
	*trackID = buffer;
	return true;

cleanup:
	if (file) {
		fclose(file);
	}
	return false;
}

bool SoundManager::loadWav(FILE* file, ALuint buffer)
{
	unsigned char stream[12];

	// Verify the wave fmt magic value meaning format.
	if (fread(stream, 1, 8, file) != 8 || memcmp(stream, "fmt ", 4) != 0 ) 	{
		qDebug() << "Failed to verify the magic value for the wave file format.";
		return false;
	}

	unsigned int section_size;
	section_size  = stream[7]<<24;
	section_size |= stream[6]<<16;
	section_size |= stream[5]<<8;
	section_size |= stream[4];

	// Check for a valid pcm format.
	if (fread(stream, 1, 2, file) != 2 || stream[1] != 0 || stream[0] != 1) {
		qDebug() << "Unsupported audio file format (must be a valid PCM format).";
		return false;
	}

	// Get the channel count (16-bit little-endian).
	int channels;
	if (fread(stream, 1, 2, file) != 2) {
		qDebug() << "Failed to read the wave file's channel count.";
		return false;
	}
	channels  = stream[1]<<8;
	channels |= stream[0];

	// Get the sample frequency (32-bit little-endian).
	ALuint frequency;
	if (fread(stream, 1, 4, file) != 4) {
		qDebug() << "Failed to read the wave file's sample frequency.";
		return false;
	}

	frequency  = stream[3]<<24;
	frequency |= stream[2]<<16;
	frequency |= stream[1]<<8;
	frequency |= stream[0];

	// The next 6 bytes hold the block size and bytes-per-second.
	// We don't need that info, so just read and ignore it.
	// We could use this later if we need to know the duration.
	if (fread(stream, 1, 6, file) != 6) {
		qDebug() << "Failed to read past the wave file's block size and bytes-per-second.";
		return false;
	}

	// Get the bit depth (16-bit little-endian).
	int bits;
	if (fread(stream, 1, 2, file) != 2) {
		qDebug() << "Failed to read the wave file's bit depth.";
		return false;
	}
	bits  = stream[1]<<8;
	bits |= stream[0];

	// Now convert the given channel count and bit depth into an OpenAL format.
	ALuint format = 0;
	if (bits == 8) {
		if (channels == 1)
			format = AL_FORMAT_MONO8;
		else if (channels == 2)
			format = AL_FORMAT_STEREO8;
	}
	else if (bits == 16) {
		if (channels == 1)
			format = AL_FORMAT_MONO16;
		else if (channels == 2)
			format = AL_FORMAT_STEREO16;
	}
	else {
		qDebug() << "Incompatible wave file format: ( " << channels << ", " << bits << ")";
		return false;
	}

	// Check against the size of the format header as there may be more data that we need to read.
	if (section_size > 16) {
		unsigned int length = section_size - 16;

		// Extension size is 2 bytes.
		if (fread(stream, 1, length, file) != length) {
			qDebug() << "Failed to read extension size from wave file.";
			return false;
		}
	}

	// Read in the rest of the file a chunk (section) at a time.
	while (true) {
		// Check if we are at the end of the file without reading the data.
		if (feof(file)) {
			qDebug() << "Failed to load wave file; file appears to have no data.";
			return false;
		}

		// Read in the type of the next section of the file.
		if (fread(stream, 1, 4, file) != 4) {
			qDebug() << "Failed to read next section type from wave file.";
			return false;
		}

		// Data chunk.
		if (memcmp(stream, "data", 4) == 0) {
			// Read how much data is remaining and buffer it up.
			unsigned int dataSize;
			if (fread(&dataSize, sizeof(int), 1, file) != 1) {
				qDebug() << "Failed to read size of data section from wave file.";
				return false;
			}

			char* data = new char[dataSize];
			if (fread(data, sizeof(char), dataSize, file) != dataSize) 	{
				qDebug() << "Failed to load wave file; file is missing data.";
				delete data;
				return false;
			}

			alBufferData(buffer, format, data, dataSize, frequency);
			REPORT_OPENAL_ERROR(alGetError());
			delete data;

			// We've read the data, so return now.
			return true;
		}
		// Other chunk - could be any of the following:
		// - Fact ("fact")
		// - Wave List ("wavl")
		// - Silent ("slnt")
		// - Cue ("cue ")
		// - Playlist ("plst")
		// - Associated Data List ("list")
		// - Label ("labl")
		// - Note ("note")
		// - Labeled Text ("ltxt")
		// - Sampler ("smpl")
		// - Instrument ("inst")
		else {
			// Store the name of the chunk so we can report errors informatively.
			char chunk[5] = { 0 };
			memcpy(chunk, stream, 4);

			// Read the chunk size.
			if (fread(stream, 1, 4, file) != 4) {
				qDebug() << "Failed to read size of " << chunk << "chunk from wave file.";
				return false;
			}

			section_size  = stream[3]<<24;
			section_size |= stream[2]<<16;
			section_size |= stream[1]<<8;
			section_size |= stream[0];

			// Seek past the chunk.
			if (fseek(file, section_size, SEEK_CUR) != 0) {
				qDebug() << "Failed to seek past " << chunk << "in wave file.";
				return false;
			}
		}
	}

	return true;
}

bool SoundManager::loadOgg(FILE* file, ALuint buffer)
{
	OggVorbis_File ogg_file;
	vorbis_info* info;
	ALenum format;
	int result;
	int section;
	unsigned int size = 0;

	rewind(file);

	if ((result = ov_open(file, &ogg_file, NULL, 0)) < 0) {
		fclose(file);
		qDebug() << "Failed to open ogg file.";
		return false;
	}

	info = ov_info(&ogg_file, -1);

	if (info->channels == 1)
		format = AL_FORMAT_MONO16;
	else
		format = AL_FORMAT_STEREO16;

	// size = #samples * #channels * 2 (for 16 bit).
	unsigned int data_size = ov_pcm_total(&ogg_file, -1) * info->channels * 2;
	char* data = new char[data_size];

	while (size < data_size) {
		result = ov_read(&ogg_file, data + size, data_size - size, 0, 2, 1, &section);
		if (result > 0) {
			size += result;
		}
		else if (result < 0) {
			delete data;
			qDebug() << "Failed to read ogg file; file is missing data.";
			return false;
		}
		else {
			break;
		}
	}

	if (size == 0) {
		delete data;
		qDebug() << "Filed to read ogg file; unable to read any data.";
		return false;
	}

	alBufferData(buffer, format, data, data_size, info->rate);

	REPORT_OPENAL_ERROR(alGetError());

	delete data;
	ov_clear(&ogg_file);

	// ov_clear actually closes the file pointer as well.
	file = 0;

	return true;
}

bool SoundManager::play(ALuint bufferID, float pitch, float gain)
{
    static uint sourceIndex = 0;

    if (bufferID != 0) {
        // Increment which source we are using, so that we play in a "free" source.
        sourceIndex = (sourceIndex + 1) % SOUNDMANAGER_MAX_SOURCES;

        // Get the source in which the sound will be played.
        ALuint source = m_sources[sourceIndex];

        if (alIsSource (source) == AL_TRUE) {

        	alSourceStop(source);

            // Attach the buffer to an available source.
            alSourcei(source, AL_BUFFER, bufferID);

            REPORT_OPENAL_ERROR(alGetError());

            // Set the source pitch value.
            alSourcef(source, AL_PITCH, pitch);

            REPORT_OPENAL_ERROR(alGetError());

            // Set the source gain value.
            alSourcef(source, AL_GAIN, gain);

            REPORT_OPENAL_ERROR(alGetError());

            // Play the source.
            alSourcePlay(source);

            REPORT_OPENAL_ERROR(alGetError());
        }
    } else {
        // The buffer was not found.
        return false;
    }

    return true;
}

void SoundManager::play(ALuint index)
{
    // Play the sound with default gain and pitch values.
    if (!play(index, 1.0f, 1.0f))
    	qDebug("Failed to play");
}
