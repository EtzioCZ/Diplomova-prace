#include "Recording.h"

Recording::Recording()
{
}

Recording::~Recording()
{
    recordingBuffer.~AudioBuffer();
}

void Recording::saveSound(double sampleRate, File fullPath)
{
    fullPath.getFullPathName();
    fullPath.deleteFile();

    writer.reset(format.createWriterFor(new FileOutputStream(fullPath),
        sampleRate,
        1,
        24,
        {},
        0));

    if (writer != nullptr)
    {        
        writer->writeFromAudioSampleBuffer(recordingBuffer, 0, recordingBuffer.getNumSamples());   
        recordingBuffer.clear();
 
    }
}
