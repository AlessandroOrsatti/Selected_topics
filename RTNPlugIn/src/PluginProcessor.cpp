/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RTNPlugInAudioProcessor::RTNPlugInAudioProcessor()
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

    auto modelFilePath = "C:/Users/Riccardo/OneDrive - Politecnico di Milano/Documenti/GitHub/Selected_topics/neural_plug_in/data/ht1_good_run_026.json";
    std::ifstream jsonStream(modelFilePath, std::ifstream::binary);
    //RTNeural::ModelT<float, 1, 1, RTNeural::LSTMLayerT<float, 1, 32>, RTNeural::DenseT<float, 32, 1>> model;
    //loadModel(jsonStream, model);
    //loadModelRun(jsonStream,modelRun[2]);
    nlohmann::json modelJson;
    jsonStream >> modelJson;

    DBG("ok");

    auto& lstm = modelRun[0]->get<0>();
    auto& lstm1 = modelRun[1]->get<0>();
    // note that the "lstm." is a prefix used to find the
    // lstm data in the json file so your python
    // needs to name the lstm layer 'lstm' if you use lstm. as your prefix
    std::string prefix = "lstm.";
    // for LSTM layers, number of hidden  = number of params in a hidden weight set
    // divided by 4
    auto hidden_count = modelJson[prefix + ".weight_ih_l0"].size() / 4;
    DBG("ok2");
    // assert that the number of hidden units is the same as this count
    // to ensure the json file we are importing matches the model we defined.
    RTNeural::torch_helpers::loadLSTM<float>(modelJson, prefix, lstm);
    RTNeural::torch_helpers::loadLSTM<float>(modelJson, prefix, lstm1);

    DBG("ok3");

    auto& dense = modelRun[0]->get<1>();
    auto& dense1 = modelRun[1]->get<1>();
    // as per the lstm prefix, here the json needs a key prefixed with dense.
    RTNeural::torch_helpers::loadDense<float>(modelJson, "dense.", dense);
    RTNeural::torch_helpers::loadDense<float>(modelJson, "dense.", dense1);
}

RTNPlugInAudioProcessor::~RTNPlugInAudioProcessor()
{
}

//==============================================================================
const juce::String RTNPlugInAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RTNPlugInAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RTNPlugInAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RTNPlugInAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RTNPlugInAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RTNPlugInAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RTNPlugInAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RTNPlugInAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RTNPlugInAudioProcessor::getProgramName (int index)
{
    return {};
}

void RTNPlugInAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void RTNPlugInAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    model.reset();

    modelRun->reset();
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void RTNPlugInAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RTNPlugInAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void RTNPlugInAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    //for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    //{
    //    auto* x = buffer.getWritePointer(ch);
    //    for (int n = 0; n < buffer.getNumSamples(); ++n)
    //    {
    //        float input[] = { x[n] };
    //        x[n] = model.forward(input);
    //    }
    //}
}

//==============================================================================
bool RTNPlugInAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RTNPlugInAudioProcessor::createEditor()
{
    return new RTNPlugInAudioProcessorEditor (*this);
}

//==============================================================================
void RTNPlugInAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RTNPlugInAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RTNPlugInAudioProcessor();
}

void RTNPlugInAudioProcessor::loadModel(std::ifstream& jsonStream, RTNeural::ModelT<float, 1, 1, RTNeural::LSTMLayerT<float, 1, 32>, RTNeural::DenseT<float, 32, 1>>& model)
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

void RTNPlugInAudioProcessor::loadModelRun(std::ifstream& jsonStream, std::unique_ptr<RTNeural::ModelT<float, 1, 1, RTNeural::LSTMLayerT<float, 1, 32>, RTNeural::DenseT<float, 32, 1>>> modelRun[2])
{
    nlohmann::json modelJson;
    jsonStream >> modelJson;

    DBG("ok");

    auto& lstm = modelRun[0]->get<0>();
    // note that the "lstm." is a prefix used to find the
    // lstm data in the json file so your python
    // needs to name the lstm layer 'lstm' if you use lstm. as your prefix
    std::string prefix = "lstm.";
    // for LSTM layers, number of hidden  = number of params in a hidden weight set
    // divided by 4
    auto hidden_count = modelJson[prefix + ".weight_ih_l0"].size() / 4;
    DBG("ok2");
    // assert that the number of hidden units is the same as this count
    // to ensure the json file we are importing matches the model we defined.
    RTNeural::torch_helpers::loadLSTM<float>(modelJson, prefix, lstm);

    DBG("ok3");

    auto& dense = modelRun[0]->get<1>();
    // as per the lstm prefix, here the json needs a key prefixed with dense.
    RTNeural::torch_helpers::loadDense<float>(modelJson, "dense.", dense);

}