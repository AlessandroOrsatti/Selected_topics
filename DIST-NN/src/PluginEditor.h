/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "setLookAndFeel"
//==============================================================================
/**
*/


class DISTNNPlugInAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                       public juce::Slider::Listener,
                                       public juce::Button::Listener
{
public:
    DISTNNPlugInAudioProcessorEditor (DISTNNPlugInAudioProcessor&);
    ~DISTNNPlugInAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (Slider* slider) override;
    void buttonClicked(Button* button) override;
    
    void play();
    void stop();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DISTNNPlugInAudioProcessor& audioProcessor;
    
    enum class PlayState {
        Play,
        Stop
    };
    
    PlayState playstate {PlayState::Play};
    
    OtherLookAndFeel otherLookAndFeel;
    juce::Slider phaserKnob;
    juce::TextButton On;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DISTNNPlugInAudioProcessorEditor)
};
