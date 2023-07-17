/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

// not this:
//#include <JuceHeader.h>
// but this:
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <torch/torch.h>
#include <torch/script.h>
#include <iostream>
#include <cmath>
#include <RTNeural/RTNeural.h>


#include "PluginProcessor.h"
#include "ClickableArea.h"



//==============================================================================
/**
*/



class NeuralPluginEditor  : public juce::AudioProcessorEditor,
                          // listen to buttons
                          public juce::Button::Listener,
                          // listen to AudioThumbnail
                          public juce::ChangeListener,
                          public juce::FileDragAndDropTarget,
                          private juce::Timer
                          

{
public:
    NeuralPluginEditor (NeuralPluginProcessor&);
    ~NeuralPluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* btn) override;

    juce::AudioBuffer<float> getAudioBufferFromFile(juce::File file);
    
    juce::File absolutePath = juce::File::getCurrentWorkingDirectory().getParentDirectory();



    
    //VISUALIZER
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void thumbnailChanged();
    
    void displayOut(juce::AudioBuffer<float>& buffer, juce::AudioThumbnail& thumbnailOut);

    void paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds, at::string Phrase);

    void paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds, juce::AudioThumbnail& thumbnailWav, juce::Colour color);

    void paintCursorInput(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds, juce::AudioThumbnail& thumbnailWav, juce::Colour color);

    void paintCursorFirstEffect(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds, juce::AudioThumbnail& thumbnailWav, juce::Colour color);

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void loadFile(const juce::String& path);


    //CREATE WAV
    void CreateWavQuick(torch::Tensor yFirstEffectTensor, juce::String path, juce::String name);


    //RTNeural::ModelT<float, 1, 1, RTNeural::LSTMLayerT<float, 1, 8>, RTNeural::DenseT<float, 8, 1>>;

    //LOAD RTNEURAL MODEL
    void loadModel(std::ifstream& jsonStream, RTNeural::ModelT<float, 1, 1, RTNeural::LSTMLayerT<float, 1, 8>, RTNeural::DenseT<float, 8, 1>>& model);



private:

    juce::String inputFileName;

    juce::File docsDir;
    juce::File filesDir;
    juce::File modelsDir;
    juce::File imagesDir;


    enum TransportState
    {
      Stopped,
      Starting,
      Stopping,
      Playing
    };

    TransportState state;
    
    juce::Image background;
    
    juce::ImageComponent imageKit;
    juce::ImageComponent imageFirstEffect;
    juce::ImageComponent downloadIcon;
    juce::ImageComponent play;
    juce::ImageComponent stop;

    //buttons
    juce::ImageButton testButton;
    juce::ImageButton openButton;
    juce::ImageButton playButton;
    juce::ImageButton stopButton;

    juce::ImageButton downloadFirstEffectButton;
    
    juce::ImageButton playFirstEffectButton;
    juce::ImageButton stopFirstEffectButton;
    juce::ImageComponent browseImage;
    juce::ImageComponent separate;

    ClickableArea areaFirstEffect;

    ClickableArea areaFull;



    
    //VISUALIZER
    
    juce::AudioThumbnail* thumbnail;
    juce::AudioThumbnailCache* thumbnailCache;

    juce::AudioThumbnail* thumbnailFirstEffectOut;
    juce::AudioThumbnailCache* thumbnailCacheFirstEffectOut;

    
    //------------------------------------------------------------------------------------
    
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> playSource;
    juce::File myFile;
    juce::File myFileOut;
    void transportStateChanged(TransportState newState, juce::String id);

    juce::AudioBuffer<float> bufferY;
    //juce::AudioBuffer<float> bufferOut;

    //std::vector<float> audioPoints;
    
    //audioPoints.call_back(new float (args));
    bool paintOut{ false };

    void timerCallback() override
    {
        repaint();
    }
    

    //output tensors
    at::Tensor yFirstEffect;
    at::Tensor tensorOut;

    NeuralPluginProcessor& audioProcessor;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralPluginEditor)
};


