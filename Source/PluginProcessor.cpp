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
                       ),
    delay_channel(48000 * 2)
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    juce::dsp::ProcessSpec spec{ sampleRate, samplesPerBlock, 2 };

    delay_channel.reset();
    delay_channel.prepare(spec);

    UpdateRandDelay();
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

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    const int buffer_length = buffer.getNumSamples();


    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    UpdateRandDelay();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* data = buffer.getWritePointer(channel);
        
        for (int i = 0; i < buffer_length; ++i) {
            delay_channel.pushSample(channel, data[i]);
            data[i] = delay_channel.popSample(channel, GetParameterSettings(tree_state).time * getSampleRate());
        }

        // ..do something to the data...
    }

    

 /*   juce::dsp::AudioBlock<float> block(buffer);

    auto left_block = block.getSingleChannelBlock(0);
    auto right_block = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> left_context(left_block);
    juce::dsp::ProcessContextReplacing<float> right_context(right_block);

    delay_channel.process(left_context);
    delay_channel.process(right_context);*/
}

//==============================================================================
bool GlitchBunnyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GlitchBunnyAudioProcessor::createEditor()
{
    //return new GlitchBunnyAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
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

void GlitchBunnyAudioProcessor::UpdateRandDelay() 
{
    auto p_settings = GetParameterSettings(tree_state);

    int randomness = p_settings.rand;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(-randomness, randomness);
    int rand = distribution(generator);

    float rand_time = p_settings.time + (rand / 50.f);

    delay_channel.setDelay(rand_time * getSampleRate());
}

ParameterSettings GetParameterSettings(const juce::AudioProcessorValueTreeState& tree_state)
{
    ParameterSettings p_settings;

    p_settings.mix = tree_state.getRawParameterValue("Mix")->load();
    p_settings.time = tree_state.getRawParameterValue("Time")->load();
    p_settings.feedback = tree_state.getRawParameterValue("Mix")->load();
    p_settings.rand = tree_state.getRawParameterValue("Randomness")->load();

    return p_settings;
}

juce::AudioProcessorValueTreeState::ParameterLayout GlitchBunnyAudioProcessor::CreateParameterLayout() 
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterInt>( "Mix", "Mix", 0, 100, 50 ));

    layout.add(std::make_unique<juce::AudioParameterFloat>( "Time", "Time", 
                                                            juce::NormalisableRange<float>(0.f, MAX_DELAY, 0.001f, 1.f), 
                                                            1.f ));

    layout.add(std::make_unique<juce::AudioParameterInt>("Feedback", "Feedback", 0, 100, 20));

    layout.add(std::make_unique<juce::AudioParameterInt>("Randomness", "Randomness", 0, 100, 0));

    return layout;
}