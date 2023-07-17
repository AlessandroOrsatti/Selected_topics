/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include <iostream>
#include <filesystem>
//#include <RTNeural/RTNeural.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utils.cpp"


#include <torch/torch.h>
#include <torch/script.h>
#include <cmath>
#include <chrono>
#include <thread>
#include <chrono>

//==============================================================================
NeuralPluginEditor::NeuralPluginEditor (NeuralPluginProcessor& p)
    : AudioProcessorEditor (&p), formatManager(), audioProcessor(p), state(Stopped),
    areaFirstEffect{}, areaFull{}

{

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 500);

    //create system directory
    docsDir = juce::File::getSpecialLocation(juce::File::userMusicDirectory);
    filesDir = juce::File(docsDir.getFullPathName() + "/DrumsDemixFilesToDrop");
    filesDir.createDirectory();
    DBG("the files you get as output are in: ");
    DBG(filesDir.getFullPathName());

    imagesDir = juce::File(juce::File::getSpecialLocation(juce::File::userDesktopDirectory).getFullPathName() + "/NeuralPluginUtils/NeuralPluginImages");
    DBG("the images are in: ");
    DBG(imagesDir.getFullPathName());



    areaFirstEffect.setFilesDir(filesDir);

    thumbnailCache = new juce::AudioThumbnailCache(5);
    thumbnail = new juce::AudioThumbnail(512, formatManager, *thumbnailCache);

    thumbnailCacheFirstEffectOut = new juce::AudioThumbnailCache(5);
    thumbnailFirstEffectOut = new juce::AudioThumbnail(512, formatManager, *thumbnailCacheFirstEffectOut);

    auto downloadIcon = juce::ImageCache::getFromFile( juce::File(imagesDir.getFullPathName() + "/download.png") );
    auto playIcon = juce::ImageCache::getFromFile( juce::File(imagesDir.getFullPathName() + "/play.png") );
    auto stopIcon = juce::ImageCache::getFromFile( juce::File(imagesDir.getFullPathName() + "/stop.png") );
    auto separate = juce::ImageCache::getFromFile( juce::File(imagesDir.getFullPathName() + "/SEPARATE.png") );

    addAndMakeVisible(testButton);
    testButton.setImages(false, true, true, separate, 1.0, juce::Colour(), separate, 0.5, juce::Colour(), separate, 0.8, juce::Colour(), 0);
    //testButton.setButtonText("SEPARATE");
    testButton.setEnabled(false);
    testButton.addListener(this);

    addAndMakeVisible(downloadFirstEffectButton);
    downloadFirstEffectButton.setImages(false,true,true,downloadIcon,1.0,juce::Colour(),downloadIcon,0.5,juce::Colour(),downloadIcon,0.8,juce::Colour(),0);
    downloadFirstEffectButton.setEnabled(true);
    downloadFirstEffectButton.addListener(this);

    addAndMakeVisible(playButton);
    //playButton.setButtonText("PLAY");
    //playButton.setEnabled(false);
    playButton.setImages(false, true, true, playIcon, 1.0, juce::Colour(), playIcon, 0.5, juce::Colour(), playIcon, 0.8, juce::Colour(), 0);
    //playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.addListener(this);

    addAndMakeVisible(stopButton);
    //stopButton.setButtonText("STOP");
    //stopButton.setEnabled(false);
    stopButton.setImages(false, true, true, stopIcon, 1.0, juce::Colour(), stopIcon, 0.5, juce::Colour(), stopIcon, 0.8, juce::Colour(), 0);
    //stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    stopButton.addListener(this);

    //FULL DRUMS
    addAndMakeVisible(areaFull);
    areaFull.addListener(this);
    areaFull.setAlpha(0);
    areaFull.setName("areaFull");
    

    //auto kitImage = juce::ImageCache::getFromFile(absolutePath.getChildFile("C:/Users/Riccardo/OneDrive - Politecnico di Milano/Documenti/GitHub/DrumsDemix/drums_demix/images/kit.png"));
    auto kitImage = juce::ImageCache::getFromFile( juce::File(imagesDir.getFullPathName() + "/input.jpeg") );
    imageKit.setImage(kitImage, juce::RectanglePlacement::stretchToFit);
    addAndMakeVisible(imageKit);
      
    //FirstEffect
    addAndMakeVisible(playFirstEffectButton);
    playFirstEffectButton.setImages(false, true, true, playIcon, 1.0, juce::Colour(), playIcon, 0.5, juce::Colour(), playIcon, 0.8, juce::Colour(), 0);
    playFirstEffectButton.addListener(this);

    addAndMakeVisible(stopFirstEffectButton);
    stopFirstEffectButton.setImages(false, true, true, stopIcon, 1.0, juce::Colour(), stopIcon, 0.5, juce::Colour(), stopIcon, 0.8, juce::Colour(), 0);
    stopFirstEffectButton.addListener(this);

    addAndMakeVisible(areaFirstEffect);
    areaFirstEffect.addListener(this);
    areaFirstEffect.setAlpha(0);
    areaFirstEffect.setName("areaFirstEffect");
    

    //auto FirstEffectImage = juce::ImageCache::getFromFile(absolutePath.getChildFile("C:/Users/Riccardo/OneDrive - Politecnico di Milano/Documenti/GitHub/DrumsDemix/drums_demix/images/FirstEffect.png"));
    auto FirstEffectImage = juce::ImageCache::getFromFile( juce::File(imagesDir.getFullPathName() + "/reverb.jpeg") );
    imageFirstEffect.setImage(FirstEffectImage, juce::RectanglePlacement::stretchToFit);
    addAndMakeVisible(imageFirstEffect);
    
    //-----------------------------------------------------
    //auto browseIcon = juce::ImageCache::getFromFile(absolutePath.getChildFile("C:/Users/Riccardo/OneDrive - Politecnico di Milano/Documenti/GitHub/DrumsDemix/drums_demix/images/browse.png"));
    auto browseIcon = juce::ImageCache::getFromFile( juce::File(imagesDir.getFullPathName() + "/browse.png") );

    addAndMakeVisible(openButton);
    openButton.setImages(false, true, true, browseIcon, 1.0, juce::Colour(), browseIcon, 0.5, juce::Colour(),browseIcon, 0.8, juce::Colour(), 0);
    openButton.addListener(this);

    formatManager.registerBasicFormats();
    audioProcessor.transportProcessorFirstEffect.addChangeListener(this);
    audioProcessor.transportProcessor.addChangeListener(this);
    
    //VISUALIZER
    thumbnail->addChangeListener(this);
    thumbnailFirstEffectOut->addChangeListener(this);

    
    
        

    startTimer(40);



}

NeuralPluginEditor::~NeuralPluginEditor()
{

    DBG("chiudo...");
    audioProcessor.transportProcessor.releaseResources();
    audioProcessor.transportProcessor.setSource(nullptr);
    delete thumbnail;
    delete thumbnailCache;

    audioProcessor.transportProcessorFirstEffect.releaseResources();
    audioProcessor.transportProcessorFirstEffect.setSource(nullptr);
    delete thumbnailFirstEffectOut;
    delete thumbnailCacheFirstEffectOut;
}

//==============================================================================
void NeuralPluginEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.setColour(juce::Colour::fromFloatRGBA(93, 90, 88, 255));
    g.fillAll(juce::Colour::fromRGB(93, 90, 88));

    //background = juce::ImageCache::getFromFile(juce::File(imagesDir.getFullPathName() + "/LETTERING.png"));
    //g.drawImageWithin(background, 5, 5, 380, 60, juce::RectanglePlacement::stretchToFit);
    
    //Background image
    //background = juce::ImageCache::getFromFile(absolutePath.getChildFile("C:/Users/Riccardo/OneDrive - Politecnico di Milano/Documenti/GitHub/DrumsDemix/drums_demix/images/DRUMS DEMIX.png"));
    //background = juce::ImageCache::getFromFile( juce::File(imagesDir.getFullPathName() + "/DRUMS DEMIX.png") );
    
    
    //VISUALIZER

    //N.B: getWidth() - 220 = 780

    int thumbnailHeight = (getHeight() - 200) / 5;
    int thumbnailStartPoint = (getHeight() / 9) + 10;
    int buttonHeight = (getHeight() - 200) / 5;
    juce::Rectangle<int> thumbnailBounds (10 + buttonHeight, (getHeight() / 9)+10, getWidth() - 220- buttonHeight, thumbnailHeight);
    
           if (thumbnail->getNumChannels() == 0)
               paintIfNoFileLoaded (g, thumbnailBounds, "Drop a file or load it");
           else 
           {
               paintIfFileLoaded(g, thumbnailBounds, *thumbnail, juce::Colour(200, 149, 127));
               paintCursorInput(g, thumbnailBounds, *thumbnail, juce::Colour(200, 149, 127));
           }

        
    juce::Rectangle<int> thumbnailBoundsFirstEffectOut (10 + buttonHeight,10+ thumbnailStartPoint + thumbnailHeight, getWidth() - 220 - buttonHeight, thumbnailHeight);
    
           if (thumbnailFirstEffectOut->getNumChannels() == 0)
               paintIfNoFileLoaded (g, thumbnailBoundsFirstEffectOut, "FirstEffect");
           else
           {
               paintIfFileLoaded(g, thumbnailBoundsFirstEffectOut, *thumbnailFirstEffectOut, juce::Colour(199, 128, 130));
               paintCursorFirstEffect(g, thumbnailBoundsFirstEffectOut, *thumbnailFirstEffectOut, juce::Colour(199, 128, 130));
           }
    
}

void NeuralPluginEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    float rowHeight = getHeight()/5; 
    int buttonHeight = (getHeight() - 200) / 5;
    int thumbnailWidth = getWidth() - 220;
    int thumbnailHeight = (getHeight() - 200) / 5;
    int thumbnailStartPoint = (getHeight() / 9) + 10;
    

    testButton.setBounds(getWidth()/2,5, getWidth()/2, getHeight()/9);
    openButton.setBounds(getWidth() - 220 + 10 + (getHeight() - 200) / 4 + 10 + buttonHeight, getHeight() / 9 + 10, buttonHeight, buttonHeight);

    
    playButton.setBounds(getWidth() - 220 +20, getHeight() / 9 +10, buttonHeight, buttonHeight);
    stopButton.setBounds(getWidth() - 220 + 10 + (getHeight() - 200) / 4, getHeight() / 9 +10, buttonHeight, buttonHeight);
    imageKit.setBounds(5 , getHeight() / 9 +10, buttonHeight, buttonHeight);
    

    playFirstEffectButton.setBounds(getWidth() - 220 +20, 10 + thumbnailStartPoint + thumbnailHeight, buttonHeight, buttonHeight);
    stopFirstEffectButton.setBounds(getWidth() - 220 + 10 + (getHeight() - 200) / 4, 10 + thumbnailStartPoint + thumbnailHeight, buttonHeight, buttonHeight);
    imageFirstEffect.setBounds(5, 10 + thumbnailStartPoint + thumbnailHeight, buttonHeight, buttonHeight);
    areaFirstEffect.setBounds(10,10+ thumbnailStartPoint + thumbnailHeight, getWidth() - 220, thumbnailHeight);

    

    areaFull.setBounds(10, (getHeight() / 9) + 10, getWidth() - 220, thumbnailHeight);


    downloadFirstEffectButton.setBounds(getWidth() - 220 + 10 + (getHeight() - 200) / 4 + buttonHeight + 10, 10 + thumbnailStartPoint + thumbnailHeight, buttonHeight, buttonHeight);
    
}


juce::AudioBuffer<float> NeuralPluginEditor::getAudioBufferFromFile(juce::File file)
{
    //juce::AudioFormatManager formatManager - declared in header...`;
    auto* reader = formatManager.createReaderFor(file);
    juce::AudioBuffer<float> audioBuffer;
    audioBuffer.setSize(reader->numChannels, reader->lengthInSamples);
    reader->read(&audioBuffer, 0, reader->lengthInSamples, 0, true, true);
    delete reader;
    return audioBuffer;

}

void NeuralPluginEditor::buttonClicked(juce::Button* btn)
{

    if (btn == &testButton) {


        //auto begin = std::chrono::high_resolution_clock::now();
        //***TAKE THE INPUT FROM THE MIXED DRUMS FILE***


        //-From Wav to AudiofileBuffer


        Utils utils = Utils();
        juce::AudioBuffer<float> fileAudiobuffer = getAudioBufferFromFile(myFile);

        DBG("number of samples, audiobuffer");
        DBG(fileAudiobuffer.getNumSamples());

        const float* readPointer1 = fileAudiobuffer.getReadPointer(0);
        //const float* readPointer2 = fileAudiobuffer.getReadPointer(1);


        auto options = torch::TensorOptions().dtype(torch::kFloat32);

        // MODEL rtNeural

        auto modelFilePath = "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/neural_plug_in/model.json";
        //assert(std::filesystem::exists(modelFilePath));

        std::cout << "Loading model from path: " << modelFilePath << std::endl;

        std::ifstream jsonStream(modelFilePath, std::ifstream::binary);
        RTNeural::ModelT<float, 1, 1, RTNeural::LSTMLayerT<float, 1, 8>, RTNeural::DenseT<float, 8, 1>> model;
        loadModel(jsonStream, model);
        model.reset();


        //-From a stereo AudioBuffer to a 2D Tensor
        torch::Tensor fileTensor1 = torch::from_blob((float*)readPointer1, { 1, fileAudiobuffer.getNumSamples() }, options);
        //torch::Tensor fileTensor2 = torch::from_blob((float*)readPointer2, { 1, fileAudiobuffer.getNumSamples() }, options);


        //from tensor to vector

        std::vector<float> inputs(fileTensor1.data_ptr<float>(), fileTensor1.data_ptr<float>() + fileTensor1.numel());
        std::vector<float> outputs{};
        outputs.resize(inputs.size(), {});

        for (size_t i = 0; i < inputs.size(); ++i)
        {
            outputs[i] = model.forward(&inputs[i]);
            std::cout << "in " << inputs[i] << " out: " << outputs[i] << std::endl;
        }

        int sizeOut = outputs.size();
        c10::ArrayRef<int64_t> sizes = { 1,sizeOut };


        tensorOut = torch::from_blob(outputs.data(),sizes);


        juce::File file = juce::File("/Users/alessandroorsatti/Desktop/ReverbTest.wav");
        DBG(file.getFullPathName());

        //-Create an array of 2 float pointers from the 2 std vectors 
        float* dataPtrs[2];
        dataPtrs[0] = { outputs.data() };
        dataPtrs[1] = { outputs.data() };


        //-Create the stereo AudioBuffer
        juce::AudioBuffer<float> bufferY = juce::AudioBuffer<float>(dataPtrs, 2, tensorOut.sizes()[1]); //need to change last argument to let it be dynamic!

        //-Print Wav
        juce::WavAudioFormat formatWav;
        std::unique_ptr<juce::AudioFormatWriter> writerY;

        DBG("OK");
        
        writerY.reset(formatWav.createWriterFor(new juce::FileOutputStream(file),
            44100.0,
            bufferY.getNumChannels(),
            16,
            {},
            0));
        if (writerY != nullptr)
            writerY->writeFromAudioSampleBuffer(bufferY, 0, bufferY.getNumSamples());

        thumbnailFirstEffectOut->setSource(new juce::FileInputSource(file));
        
        DBG("wav scritto!");

    }
    if (btn == &openButton) {

        juce::FileChooser chooser("Choose a Wav or Aiff File", juce::File::getSpecialLocation(juce::File::userDesktopDirectory), "*.wav;*.aiff;*.mp3");

        if (chooser.browseForFileToOpen())
        {
            //juce::File myFile;
            myFile = chooser.getResult();
            inputFileName = chooser.getResult().getFileName();

            areaFirstEffect.setInFile(inputFileName);



            juce::AudioFormatReader* reader = formatManager.createReaderFor(myFile);

            if (reader != nullptr)
            {

                std::unique_ptr<juce::AudioFormatReaderSource> tempSource(new juce::AudioFormatReaderSource(reader, true));

                audioProcessor.transportProcessor.setSource(tempSource.get());
                transportStateChanged(Stopped, "input");

                playSource.reset(tempSource.get());
                areaFull.setSrc(tempSource.release());


                DBG("IFopenbuttonclicked");

            }
            DBG("openbuttonclicked");
            testButton.setEnabled(true);

            auto docsDir = juce::File::getSpecialLocation(juce::File::userMusicDirectory);

            DBG(docsDir.getFullPathName());
            DBG(juce::File::getSpecialLocation(juce::File::currentExecutableFile).getFullPathName());
            DBG(juce::File::getSpecialLocation(juce::File::currentApplicationFile).getFullPathName());
            DBG(juce::File::getSpecialLocation(juce::File::invokedExecutableFile).getFullPathName());
            DBG(juce::File::getSpecialLocation(juce::File::hostApplicationPath).getFullPathName());
            DBG(juce::File::getSpecialLocation(juce::File::tempDirectory).getFullPathName());



        }
        //VISUALIZER
        thumbnail->setSource(new juce::FileInputSource(myFile));
    }


    if (btn == &downloadFirstEffectButton) {

        juce::FileChooser chooser("Choose a Folder to save the .wav File", juce::File::getSpecialLocation(juce::File::userDesktopDirectory));

        if (chooser.browseForDirectory())
        {
            DBG(chooser.getResult().getFullPathName());
            CreateWavQuick(yFirstEffect, chooser.getResult().getFullPathName(),inputFileName.dropLastCharacters(4)+"_FirstEffect.wav");


        }

    }

    if (btn == &playButton){

        audioProcessor.playInput = true;
        audioProcessor.playFirstEffect = false;


        transportStateChanged(Starting, "input");
        DBG("playbuttonclicked");
        

    }
    if (btn == &stopButton){
        transportStateChanged(Stopping, "input");
        DBG("stopbuttonclicked");

    }


    if (btn == &playFirstEffectButton) {
        audioProcessor.playInput = false;
        audioProcessor.playFirstEffect = true;
        transportStateChanged(Starting, "FirstEffect");
        DBG("playbuttonclicked");


    }
    if (btn == &stopFirstEffectButton) {
        transportStateChanged(Stopping, "FirstEffect");
        DBG("stopbuttonclicked");

    }


}

void NeuralPluginEditor::transportStateChanged(TransportState newState, juce::String id)
{
    if (id == "input")
    {
        if (newState != state)
        {
            state = newState;

            switch (state)
            {
            case Stopped:
                audioProcessor.transportProcessor.setPosition(0.0);
                //playButton.setEnabled(true);
                //stopButton.setEnabled(false);
                break;
            case Starting:
                //stopButton.setEnabled(true);
                //playButton.setEnabled(false);
                audioProcessor.transportProcessor.start();
                break;
            case Playing:
                //stopButton.setEnabled(true);
                break;
            case Stopping:
                //stopButton.setEnabled(false);
                //playButton.setEnabled(true);
                audioProcessor.transportProcessor.stop();
                break;
            }
        }
    }

    if (id == "FirstEffect")
    {
        if (newState != state)
        {
            state = newState;

            switch (state)
            {
            case Stopped:
                audioProcessor.transportProcessorFirstEffect.setPosition(0.0);
                //playFirstEffectButton.setEnabled(true);
                //stopFirstEffectButton.setEnabled(false);
                break;
            case Starting:
                //stopFirstEffectButton.setEnabled(true);
                //playFirstEffectButton.setEnabled(false);
                audioProcessor.transportProcessorFirstEffect.start();
                break;
            case Playing:
                //stopFirstEffectButton.setEnabled(true);
                break;
            case Stopping:
                //stopFirstEffectButton.setEnabled(false);
                //playFirstEffectButton.setEnabled(true);
                audioProcessor.transportProcessorFirstEffect.stop();
                break;
            }
        }
    }
}


void NeuralPluginEditor::displayOut(juce::AudioBuffer<float>& buffer, juce::AudioThumbnail& thumbnailOut)
{

    thumbnailOut.reset(buffer.getNumChannels(), 44100, buffer.getNumSamples());
    thumbnailOut.addBlock(0, buffer, 0, buffer.getNumSamples());
}


//VISUALIZER
void NeuralPluginEditor::changeListenerCallback (juce::ChangeBroadcaster* source)
  {
    if (source == thumbnail) { repaint(); }
    if (source == thumbnailFirstEffectOut) { repaint(); }
    if(source == &audioProcessor.transportProcessor)
    {

        if(audioProcessor.transportProcessor.isPlaying())
        {
        transportStateChanged(Playing, "input");
        }
        else
        {
            DBG("input reset");
        transportStateChanged(Stopped, "input");
        }
    }

    if (source == &audioProcessor.transportProcessorFirstEffect)
    {

        if (audioProcessor.transportProcessorFirstEffect.isPlaying())
        {
            transportStateChanged(Playing, "FirstEffect");
        }
        else
        {
            transportStateChanged(Stopped, "FirstEffect");
        }
    }
  }

void NeuralPluginEditor::paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds, at::string phrase)
  {
      g.setColour(juce::Colour(46, 45, 45));
      g.fillRect (thumbnailBounds);
      g.setColour (juce::Colours::white);
      g.drawFittedText (phrase, thumbnailBounds, juce::Justification::centred, 1);
  }

void NeuralPluginEditor::paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds, juce::AudioThumbnail& thumbnailWav, juce::Colour color)
 {
    float thumbnailHeight = (getHeight() - 200) / 5;
    float thumbnailStartPoint = (getHeight() / 9) + 10;
     g.setColour (juce::Colour(46, 45, 45));
     g.fillRect (thumbnailBounds);

     g.setColour (color);                               // [8]
     auto audioLength = (float)thumbnailWav.getTotalLength();

     thumbnailWav.drawChannels (g,                                      // [9]
                             thumbnailBounds,
                             0.0,                                    // start time
                             thumbnailWav.getTotalLength(),             // end time
                             1.0f);  // vertical zoom

     g.setColour(juce::Colours::lightgrey);

 }

void NeuralPluginEditor::paintCursorInput(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds, juce::AudioThumbnail& thumbnailWav, juce::Colour color) {
    auto audioLength = (float)thumbnailWav.getTotalLength();
    float thumbnailHeight = (getHeight() - 200) / 5;
    float thumbnailStartPoint = (getHeight() / 9) + 10;
    g.setColour(juce::Colours::lightgrey);
    auto audioPosition = (float)audioProcessor.transportProcessor.getCurrentPosition();
    auto drawPosition = (audioPosition / audioLength) * (float)thumbnailBounds.getWidth() + (float)thumbnailBounds.getX();
    g.drawLine(drawPosition, (float)(getHeight() / 9) + 10, drawPosition,
        (float)(getHeight() / 9) + 10 + thumbnailHeight, 1.0f);
}

void NeuralPluginEditor::paintCursorFirstEffect(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds, juce::AudioThumbnail& thumbnailWav, juce::Colour color) {
    auto audioLength = (float)thumbnailWav.getTotalLength();
    float thumbnailHeight = (getHeight() - 200) / 5;
    float thumbnailStartPoint = (getHeight() / 9) + 10;
    g.setColour(juce::Colours::lightgrey);
    auto audioPosition = (float)audioProcessor.transportProcessorFirstEffect.getCurrentPosition();
    auto drawPosition = (audioPosition / audioLength) * (float)thumbnailBounds.getWidth() + (float)thumbnailBounds.getX();
    g.drawLine(drawPosition, (float)10 + thumbnailStartPoint + thumbnailHeight, drawPosition,
        (float)10 + thumbnailStartPoint + thumbnailHeight + thumbnailHeight, 1.0f);
}






bool NeuralPluginEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aiff"))
        {
            return true;
        }
    }

    return false;
}

void NeuralPluginEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if ((juce::File(file).isAChildOf(filesDir.getFullPathName()))) { DBG("cercando di droppare un file dall'interno!"); };


        if (isInterestedInFileDrag(files) && !(juce::File(file).isAChildOf(filesDir.getFullPathName())))
        {
            loadFile(file);

        }
    }
    repaint();

}

void NeuralPluginEditor::loadFile(const juce::String& path)
{


    auto file = juce::File(path);
    inputFileName = file.getFileName();

    areaFirstEffect.setInFile(inputFileName);

    DBG(inputFileName);
    
    myFile = file;
    juce::AudioFormatReader* reader = formatManager.createReaderFor(file);
    if (reader != nullptr)
    {

        std::unique_ptr<juce::AudioFormatReaderSource> tempSource(new juce::AudioFormatReaderSource(reader, true));

        audioProcessor.transportProcessor.setSource(tempSource.get());
        transportStateChanged(Stopped, "input");

        playSource.reset(tempSource.get());
        areaFull.setSrc(tempSource.release());
        DBG("IFopenbuttonclicked");

    }
    DBG("openbuttonclicked");
    testButton.setEnabled(true);
    playButton.setEnabled(true);

    thumbnail->setSource(new juce::FileInputSource(file));

}


void NeuralPluginEditor::CreateWavQuick(torch::Tensor yDownloadTensor, juce::String path, juce::String name)
{


        DBG("y sizes: ");
        DBG(yDownloadTensor.sizes()[0]);
        DBG(yDownloadTensor.sizes()[1]);


        juce::File file = juce::File(path).getChildFile(name);
        DBG(file.getFullPathName());


         //-Split output tensor in Left & Right
        torch::autograd::variable_list ySplit = torch::split(yDownloadTensor, 1);
        at::Tensor yL = ySplit[0];
        at::Tensor yR = ySplit[1];



        //-Make a std vector for every channel (L & R)
        yL = yL.contiguous();
        std::vector<float> vectoryL(yL.data_ptr<float>(), yL.data_ptr<float>() + yL.numel());

        yR = yR.contiguous();
        std::vector<float> vectoryR(yR.data_ptr<float>(), yR.data_ptr<float>() + yR.numel());

        //-Create an array of 2 float pointers from the 2 std vectors 
        float* dataPtrs[2];
        dataPtrs[0] = { vectoryL.data() };
        dataPtrs[1] = { vectoryR.data() };



        //-Create the stereo AudioBuffer
        juce::AudioBuffer<float> bufferY = juce::AudioBuffer<float>(dataPtrs, 2, yDownloadTensor.sizes()[1]); //need to change last argument to let it be dynamic!
        //bufferOut = juce::AudioBuffer<float>(dataPtrsOut, 2, yFirstEffectTensor.sizes()[1]);

        //-Print Wav
        juce::WavAudioFormat formatWav;
        std::unique_ptr<juce::AudioFormatWriter> writerY;

        writerY.reset (formatWav.createWriterFor(new juce::FileOutputStream(file),
                                        44100.0,
                                        bufferY.getNumChannels(),
                                        16,
                                        {},
                                        0));
            if (writerY != nullptr)
                writerY->writeFromAudioSampleBuffer(bufferY, 0, bufferY.getNumSamples());

            //std::unique_ptr<juce::MemoryAudioSource> memSourcePtr(new juce::MemoryAudioSource(bufferY, true, false));


            //audioProcessor.transportProcessorFirstEffect.setSource(memSourcePtr.get());
            //transportStateChanged(Stopped, "FirstEffect");


            //playSourceFirstEffect.reset(memSourcePtr.get());
            //areaFirstEffect.setSrcInst(memSourcePtr.release());

            ////displayOut(juce::File("../wavs/testWavJuceFirstEffect.wav"), thumbnailFirstEffectOut);

            //displayOut(bufferY,*thumbnailFirstEffectOut);
       

        DBG("wav scritto!");
    
       

}

void NeuralPluginEditor::loadModel(std::ifstream& jsonStream, RTNeural::ModelT<float, 1, 1, RTNeural::LSTMLayerT<float, 1, 8>, RTNeural::DenseT<float, 8, 1>>& model)
{
    nlohmann::json modelJson;
    jsonStream >> modelJson;

    auto& lstm = model.get<0>();
    // note that the "lstm." is a prefix used to find the 
    // lstm data in the json file so your python
    // needs to name the lstm layer 'lstm' if you use lstm. as your prefix
    std::string prefix = "lstm.";
    // for LSTM layers, number of hidden  = number of params in a hidden weight set
    // divided by 4
    auto hidden_count = modelJson[prefix + ".weight_ih_l0"].size() / 4;
    // assert that the number of hidden units is the same as this count
    // to ensure the json file we are importing matches the model we defined.
    RTNeural::torch_helpers::loadLSTM<float>(modelJson, prefix, lstm);

    auto& dense = model.get<1>();
    // as per the lstm prefix, here the json needs a key prefixed with dense. 
    RTNeural::torch_helpers::loadDense<float>(modelJson, "dense.", dense);
}
