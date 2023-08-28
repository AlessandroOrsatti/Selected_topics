/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DISTNNAudioProcessorEditor::DISTNNAudioProcessorEditor (DISTNNAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel (&otherLookAndFeel);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (300, 450);
    
    phaserKnob.setSliderStyle(Slider::SliderStyle::Rotary);
    phaserKnob.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    phaserKnob.setRange(0.0f, 1.0f, 0.05f);
    phaserKnob.setValue(0.0f);
    phaserKnob.addListener(this);
    addAndMakeVisible(phaserKnob);
    
    //On.setButtonText ("on/off");
    On.setToggleState(true, NotificationType::dontSendNotification);
    On.setButtonText("DISTORTION");
    On.onClick = [this](){play();};
    On.addListener(this);
    addAndMakeVisible (On);
    imagesDir = juce::File(juce::File::getSpecialLocation(juce::File::userDesktopDirectory).getFullPathName() + "/DistNNutils/Images");
}

DISTNNAudioProcessorEditor::~DISTNNAudioProcessorEditor()
{
}

//==============================================================================
void DISTNNAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    //juce::Rectangle<int> box (300, 120, 200, 170);
    // g.setColour (juce::Colours::orange);
    // g.fillRect (box);
    background = juce::ImageCache::getFromFile(imagesDir.getFullPathName() + "/DIST.png");
    //background = juce::ImageCache::getFromMemory(BinaryData::DIST_png, BinaryData::DIST_pngSize);
    g.drawImageWithin(background, 0, 0, 300, 450, juce::RectanglePlacement::stretchToFit);

    
    
}

void DISTNNAudioProcessorEditor::resized()
{
    auto border = 56;
    auto buttonBorder = getWidth()/3;
    
    phaserKnob.setBounds(border-27, border+40, getWidth() - border, getHeight()/2 - border);
    On.setBounds (buttonBorder, buttonBorder*2.5 +100, buttonBorder, buttonBorder/2);
    
}

void DISTNNAudioProcessorEditor::sliderValueChanged (Slider *slider){
    
    if (slider == &phaserKnob){
        
        audioProcessor.effect = phaserKnob.getValue();
    }
    
}

void DISTNNAudioProcessorEditor::buttonClicked (Button* button){
    
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

void DISTNNAudioProcessorEditor::play(){
    playstate = PlayState::Play;
    On.setToggleState(true, NotificationType::dontSendNotification);
    On.setColour((TextButton::ColourIds::buttonColourId), Colours::limegreen);
}
void DISTNNAudioProcessorEditor::stop(){
    playstate = PlayState::Stop;
    On.setToggleState(false, NotificationType::dontSendNotification);
    On.setColour((TextButton::ColourIds::buttonColourId), Colours::rebeccapurple);
}
