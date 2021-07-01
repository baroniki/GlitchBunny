/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GlitchBunnyAudioProcessor::GlitchBunnyAudioProcessor()
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
}

GlitchBunnyAudioProcessor::~GlitchBunnyAudioProcessor()
{
}

//==============================================================================
const juce::String GlitchBunnyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GlitchBunnyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GlitchBunnyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GlitchBunnyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GlitchBunnyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GlitchBunnyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GlitchBunnyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GlitchBunnyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GlitchBunnyAudioProcessor::getProgramName (int index)
{
    return {};
}

void GlitchBunnyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GlitchBunnyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec{ sampleRate, samplesPerBlock, 2 };

    m_Phaser.prepare(spec);
    m_Phaser.reset();
}

void GlitchBunnyAudioProcessor::UpdateParams() {
    auto params = GetParameterSettings(tree_state);

    m_Phaser.setMix(params.mix);
    m_Phaser.setRate(params.rate);
    m_Phaser.setDepth(params.depth);
    m_Phaser.setFeedback(params.feedback);
    m_Phaser.setCentreFrequency(params.center_freq);
}

void GlitchBunnyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GlitchBunnyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GlitchBunnyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    const int buffer_length = buffer.getNumSamples();

    UpdateParams();

    juce::dsp::AudioBlock<float> block(buffer);

    m_Phaser.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool GlitchBunnyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GlitchBunnyAudioProcessor::createEditor()
{
    return new GlitchBunnyAudioProcessorEditor (*this);
}

//==============================================================================
void GlitchBunnyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GlitchBunnyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GlitchBunnyAudioProcessor();
}

ParameterSettings GetParameterSettings(const juce::AudioProcessorValueTreeState& tree_state)
{
    ParameterSettings p_settings;

    p_settings.mix = tree_state.getRawParameterValue("Mix")->load();
    p_settings.rate = tree_state.getRawParameterValue("Rate")->load();
    p_settings.depth = tree_state.getRawParameterValue("Depth")->load();
    p_settings.feedback = tree_state.getRawParameterValue("Feedback")->load();
    p_settings.center_freq = tree_state.getRawParameterValue("Center Freq")->load();

    return p_settings;
}

juce::AudioProcessorValueTreeState::ParameterLayout GlitchBunnyAudioProcessor::CreateParameterLayout() 
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>( "Mix", "Mix", 
                                                            juce::NormalisableRange<float>(0.f, 1.f, 0.01f), 
                                                            0.5f ));

    layout.add(std::make_unique<juce::AudioParameterFloat>( "Rate", "Rate",
                                                            juce::NormalisableRange<float>(0.f, 99.f, 1.0f),
                                                            50.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>( "Depth", "Depth",
                                                            juce::NormalisableRange<float>(0.f, 1.f, 0.1f),
                                                            0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Feedback", "Feedback", juce::NormalisableRange<float>(-1.f, 1.f, 0.1f), 0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Center Freq", "Center Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.3f), 1000.f));

    return layout;
}