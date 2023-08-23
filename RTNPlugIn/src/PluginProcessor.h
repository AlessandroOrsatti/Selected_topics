/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <RTNeural/RTNeural.h>

//==============================================================================
/**
*/
class RTNPlugInAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    RTNPlugInAudioProcessor();
    ~RTNPlugInAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void loadModel(std::ifstream& jsonStream, RTNeural::ModelT<float, 2, 1, RTNeural::LSTMLayerT<float, 2, 24>, RTNeural::DenseT<float, 24, 1>>& model);
    void loadModelRun(std::ifstream& jsonStream, RTNeural::ModelT<float, 2, 1, RTNeural::LSTMLayerT<float, 2, 32>, RTNeural::DenseT<float, 32, 1>> model);
    
    float effect { 0.5 };

private:
    RTNeural::ModelT<float, 2, 1, RTNeural::LSTMLayerT<float, 2, 24>, RTNeural::DenseT<float, 24, 1>> model;
    //std::unique_ptr<RTNeural::ModelT<float, 1, 1, RTNeural::LSTMLayerT<float, 1, 32>, RTNeural::DenseT<float, 32, 1>>> modelRun[2];

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RTNPlugInAudioProcessor)
};
