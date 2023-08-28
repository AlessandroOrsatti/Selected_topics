/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DISTNNAudioProcessor::DISTNNAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    MemoryInputStream jsonStream (BinaryData::modelParametricDIST16_json, BinaryData::modelParametricDIST16_jsonSize, false);
    loadModel(jsonStream,model);

}

DISTNNAudioProcessor::~DISTNNAudioProcessor()
{
}

//==============================================================================
const juce::String DISTNNAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DISTNNAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DISTNNAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DISTNNAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DISTNNAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DISTNNAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DISTNNAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DISTNNAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DISTNNAudioProcessor::getProgramName (int index)
{
    return {};
}

void DISTNNAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DISTNNAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    model.reset();
}

void DISTNNAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DISTNNAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DISTNNAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (func == false){
        
        juce::ScopedNoDenormals noDenormals;
        auto totalNumInputChannels  = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();
        
        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
               buffer.clear (i, 0, buffer.getNumSamples());
        
        
	    float* channelDataL = buffer.getWritePointer(0);
	    float* channelDataR = buffer.getWritePointer(1);
        for (int n = 0; n < buffer.getNumSamples(); ++n)
            {
                float inputL[] = {channelDataL[n], effect};
                float inputR[] = {channelDataR[n], effect};
                channelDataL[n] = model.forward(inputL);
                channelDataR[n] = model.forward(inputR);
            }
    
    }
}

//==============================================================================
bool DISTNNAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DISTNNAudioProcessor::createEditor()
{
    return new DISTNNAudioProcessorEditor (*this);
}

//==============================================================================
void DISTNNAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DISTNNAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DISTNNAudioProcessor();
}

void DISTNNAudioProcessor::loadModel(MemoryInputStream& jsonStream, RTNeural::ModelT<float, 2, 1, RTNeural::LSTMLayerT<float, 2, 16>, RTNeural::DenseT<float, 16, 1>>& model)
{
    auto modelJson = nlohmann::json::parse (jsonStream.readEntireStreamAsString().toStdString());

    auto& lstm = model.get<0>();
    std::string prefix = "lstm.";
    auto hidden_count = modelJson[prefix + ".weight_ih_l0"].size() / 4;
    RTNeural::torch_helpers::loadLSTM<float>(modelJson, prefix, lstm);

    auto& dense = model.get<1>();
    RTNeural::torch_helpers::loadDense<float>(modelJson, "dense.", dense);
}

