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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    
    phaserKnob.setSliderStyle(Slider::SliderStyle::Rotary);
    phaserKnob.setTextBoxStyle(juce::TextBoxBelow, true, 100, 25);
    phaserKnob.setRange(0.0f, 1.0f, 0.05f);
    phaserKnob.setValue(0.5f);
    phaserKnob.addListener(this);
    addAndMakeVisible(phaserKnob);
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
    
    phaserKnob.setBounds(getLocalBounds());
}

void RTNPlugInAudioProcessorEditor::sliderValueChanged (Slider *slider){
    
    if (slider == &phaserKnob){
        
        processor.effect = phaserKnob.getValue()
    }
    
}
