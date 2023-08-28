/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "setLookAndFeel.h"
//==============================================================================
/**
*/


class DISTNNAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                       public juce::Slider::Listener,
                                       public juce::Button::Listener
{
public:
    DISTNNAudioProcessorEditor (DISTNNAudioProcessor&);
    ~DISTNNAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (Slider* slider) override;
    void buttonClicked(Button* button) override;
    
    void play();
    void stop();

private:
    
    juce::Image background;
    DISTNNAudioProcessor& audioProcessor;
    
    enum class PlayState {
        Play,
        Stop
    };
    
    PlayState playstate {PlayState::Play};
    
    OtherLookAndFeel otherLookAndFeel;
    juce::Slider phaserKnob;
    juce::TextButton On;
    juce::File imagesDir;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DISTNNAudioProcessorEditor)
};
