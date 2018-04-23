/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================

void processBuffer(AudioBuffer<float> *buffer)
{
    int numberOfChannels = buffer->getNumChannels();
    int numberOfSamples = buffer->getNumSamples();
    
    for (int c=0; c<numberOfChannels; c++)
    {
        const float* input = buffer->getReadPointer(c);
        float* output = buffer->getWritePointer(c);
        for (int n=0; n<numberOfSamples; n++)
        {
            // Example of the functions
            output[n] = input[n]*0.5f;
        }
    }
}

int main (int argc, char* argv[])
{
	if (argc == 1) {
		return 1;
	}
    // Get the filename as a string from the launch options
    String fname(argv[1]);
    //Open the file as a File object
    File inputFile(fname);
    // If the file doesn't exist, end now with error code 2
    if (inputFile.existsAsFile() == false) {
        return 2;
    }
    // Initialise the decoders
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    // Read the audio file
    AudioFormatReader *reader = formatManager.createReaderFor(inputFile);
    // Get the basic information
    int numChannels = reader->numChannels;
    int64 numSamples = reader->lengthInSamples;
    double sampleRate = reader->sampleRate;
    // Allocate some memory as an AudioBuffer with type float
    AudioBuffer <float>buffer(numChannels, numSamples);
    //Read the audio into the buffer
    reader->read(&buffer, 0, numSamples, 0, true, true);
    // We can free the reader
    delete reader;
    
    // Do some processing
    processBuffer(&buffer);
    
    // Begin the exporting
    
    // Get the filename up to the extension code
    // If fname = "~/a/b.wav", then outFname = "~/a/b
    String outFname = fname.upToLastOccurrenceOf(".", false, false);
    
    // Append "-output.wav" to the end of the filename
    outFname = outFname + String("-output.wav");
    // Open the file object. If the file already exists, delete it.
    File audioOutputFile(outFname);
    if (audioOutputFile.existsAsFile()) {
        audioOutputFile.deleteFile();
    }
    
    // Create the output streamers to actually write the data
	FileOutputStream *fileStream = audioOutputFile.createOutputStream();
    WavAudioFormat wavFormat;
    AudioFormatWriter *writer = wavFormat.createWriterFor(fileStream, sampleRate, numChannels, 16, StringPairArray(), 0);
    writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
    writer->flush();
    delete writer;
    return 0;
}
