/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>

//==============================================================================

void scaling(AudioBuffer<float> *buffer, float scalingFactor) {
    buffer->applyGain(scalingFactor);
}

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
            if (input[n] >= 0.5) {
                output[n] = 0.5;
            } else if (input[n] >= -0.5) {
                output[n] = -0.5;
            } else {
                output[n] = input[n];
            }
        }
    }
}

int main (int argc, char* argv[])
{
    std::string filename;
    std::cout << "Please enter the file location\n";
    std::cin >> filename;
    // Get the filename as a string from the launch options
    String fname(filename);
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
    std::unique_ptr<FileOutputStream> fileStream = audioOutputFile.createOutputStream();
    WavAudioFormat wavFormat;
    AudioFormatWriter* writer;
    writer = wavFormat.createWriterFor(fileStream.get(), sampleRate, numChannels, 16, StringPairArray(), 0);
    writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
    writer->flush();
    fileStream.release();
    delete writer;
    return 0;
}
