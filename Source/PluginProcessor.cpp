/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <algorithm>


//==============================================================================
DistMeAudioProcessor::DistMeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), pDistWindowSize(nullptr)
#endif
{
    addParameter(pDistWindowSize = new AudioParameterFloat("window",
                                                           "windowSize",
                                                           NormalisableRange<float>(0.0f, 1.0f),
                                                           0.3f));
}

DistMeAudioProcessor::~DistMeAudioProcessor()
{
}

//==============================================================================
const String DistMeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistMeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistMeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double DistMeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistMeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistMeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistMeAudioProcessor::setCurrentProgram (int index)
{
}

const String DistMeAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistMeAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DistMeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DistMeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistMeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DistMeAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();
    const int iDistWindowSize = pDistWindowSize->get() * numSamples;


    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    if (iDistWindowSize > 0)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float* channelData = buffer.getWritePointer (channel);
            
            // Increment by window size and don't exceed the buffer length
            for (int i = 0; i < numSamples; i = i < numSamples ? (i + 2*iDistWindowSize) : buffer.getNumSamples())
            {
                if (i + iDistWindowSize > numSamples)
                {
                    // It is the last window
                    std::random_shuffle(&channelData[i], &channelData[numSamples-1]);
                } else
                {
                    std::random_shuffle(&channelData[i], &channelData[i+iDistWindowSize]);
                }
                
            }
        }
    }

}

//==============================================================================
bool DistMeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DistMeAudioProcessor::createEditor()
{
    return new DistMeAudioProcessorEditor (*this);
}

//==============================================================================
void DistMeAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DistMeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistMeAudioProcessor();
}
