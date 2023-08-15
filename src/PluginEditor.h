/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class RTNPlugInAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                       public juce::Slider::Listener
{
public:
    RTNPlugInAudioProcessorEditor (RTNPlugInAudioProcessor&);
    ~RTNPlugInAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RTNPlugInAudioProcessor& audioProcessor;
    
    juce::Slider phaserKnob;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RTNPlugInAudioProcessorEditor)
};
