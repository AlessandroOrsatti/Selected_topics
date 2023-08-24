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
    
    //On.setButtonText ("on/off");
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
    juce::Rectangle<int> box (300, 120, 200, 170);
    g.setColour (juce::Colours::orange);
    g.fillRect (box);

    
    
}

void RTNPlugInAudioProcessorEditor::resized()
{
    auto border = 4;
    auto buttonBorder = getWidth()/3;
    
    phaserKnob.setBounds(border, border, getWidth() - border, getHeight()/2 - border);
    On.setBounds (buttonBorder, buttonBorder, buttonBorder, buttonBorder);
    
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
            audioProcessor.func = true;
        }
        else if (playstate == PlayState::Play){
            On.onClick = [this](){stop();};
            audioProcessor.func = false;
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
