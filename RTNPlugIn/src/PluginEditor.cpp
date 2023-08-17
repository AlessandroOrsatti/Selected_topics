/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RTNPlugInAudioProcessorEditor::RTNPlugInAudioProcessorEditor (RTNPlugInAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel (&otherLookAndFeel);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    
    phaserKnob.setSliderStyle(Slider::SliderStyle::Rotary);
    phaserKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 25);
    phaserKnob.setRange(0.0f, 1.0f, 0.05f);
    phaserKnob.setValue(0.0f);
    phaserKnob.addListener(this);
    addAndMakeVisible(phaserKnob);
    
    On.setButtonText ("on/off");
    On.setToggleState(true, NotificationType::dontSendNotification);
    On.onClick = [this](){play();};
    On.addListener(this);
    addAndMakeVisible (On);
}

RTNPlugInAudioProcessorEditor::~RTNPlugInAudioProcessorEditor()
{
}

//==============================================================================
void RTNPlugInAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    
    
}

void RTNPlugInAudioProcessorEditor::resized()
{
    auto border = 4;
    auto area = getLocalBounds();
    auto dialArea = area.removeFromTop (area.getHeight() / 2);
    
    phaserKnob.setBounds(dialArea);
    
    auto buttonHeight = 30;
    
    On.setBounds (area.removeFromTop (buttonHeight).reduced (border));
    
}

void RTNPlugInAudioProcessorEditor::sliderValueChanged (Slider *slider){
    
    if (slider == &phaserKnob){
        
        audioProcessor.effect = phaserKnob.getValue();
    }
    
}

void RTNPlugInAudioProcessorEditor::buttonClicked (Button* button){
    
   if (button == &On)
    {
        if (playstate == PlayState::Stop){
            On.onClick = [this](){play();};
        }
        else if (playstate == PlayState::Play){
            On.onClick = [this](){stop();};
        }
    }
    
}

void RTNPlugInAudioProcessorEditor::play(){
    playstate = PlayState::Play;
    On.setToggleState(true, NotificationType::dontSendNotification);
    On.setColour((TextButton::ColourIds::buttonColourId), Colours::limegreen);
}
void RTNPlugInAudioProcessorEditor::stop(){
    playstate = PlayState::Stop;
    On.setToggleState(false, NotificationType::dontSendNotification);
    On.setColour((TextButton::ColourIds::buttonColourId), Colours::rebeccapurple);
}
