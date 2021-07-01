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
    /*m_WritePos(0),
    m_LastFeedback(0.f)*/
    //,delay_channel(48000 * 2, 20)
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

    //juce::dsp::ProcessSpec spec{ sampleRate, samplesPerBlock, 2 };

    //delay_channel.reset();
    //delay_channel.prepare(spec);

    //mixer.reset();
    //mixer.prepare(spec);

    //UpdateRandDelay();

    /*const int num_input_channels = getTotalNumInputChannels();
    const int delay_buffer_size = 2 * (sampleRate + samplesPerBlock);

    m_DelayBuffer.setSize(num_input_channels, delay_buffer_size);*/

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

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    const int buffer_length = buffer.getNumSamples();


     //This is the place where you'd normally do the guts of your plugin's
     //audio processing...
     //Make sure to reset the state if your inner loop is processing
     //the samples and the outer loop is handling the channels.
     //Alternatively, you can process the samples with the channels
     //interleaved by keeping the same state.
    //UpdateRandDelay();

    //float delay_time = GetParameterSettings(tree_state).time;
    //float delay_in_samples = delay_time * getSampleRate();

    //float feedback = GetParameterSettings(tree_state).feedback;
    //float adjusted_feedback = feedback / 5.0;

    //

    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto* channel_data = buffer.getWritePointer(channel);
    //    float delay_data;
    //    
    //    for (int i = 0; i < buffer_length; ++i) {
    //        delay_channel.pushSample(channel, channel_data[i]);
    //        delay_data = delay_channel.popSample(channel, delay_in_samples);

    //        delay_channel.pushSample(channel, delay_data * feedback);
    //        channel_data[i] = delay_channel.popSample(channel, delay_in_samples);

    //        
    //    }

    //    // ..do something to the data...
    //}

    

    //juce::dsp::AudioBlock<float> block(buffer);
    //for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    //    auto channel_block = block.getSingleChannelBlock(channel);
    //    mixer.pushDrySamples(block);

    //    juce::dsp::ProcessContextReplacing<float> context(channel_block);

    //    delay_channel.process(context);
    //    mixer.mixWetSamples(block);
    //}

    /*auto left_block = block.getSingleChannelBlock(0);
    auto right_block = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> left_context(left_block);
    juce::dsp::ProcessContextReplacing<float> right_context(right_block);

    delay_channel.process(left_context);
    delay_channel.process(right_context);*/

   /* auto params = GetParameterSettings(tree_state);
    float feedback_gain = juce::Decibels::decibelsToGain(params.feedback);

    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        WriteDelayBuffer(channel, buffer, params, 1.0f, 1.0f, true);
        ReadDelayBuffer(channel, buffer, params, 1.f, 1.f, getSampleRate());

        WriteDelayBuffer(channel, buffer, params, m_LastFeedback, feedback_gain, false);
    }

    m_LastFeedback = feedback_gain;

    m_WritePos += buffer.getNumSamples();
    m_WritePos %= m_DelayBuffer.getNumSamples();*/

    UpdateParams();

    juce::dsp::AudioBlock<float> block(buffer);

    m_Phaser.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//void GlitchBunnyAudioProcessor::WriteDelayBuffer(int channel, juce::AudioBuffer<float>& buffer, ParameterSettings& params, const float start_gain, const float end_gain, bool copying) 
//{
//    const int buffer_length = buffer.getNumSamples();
//    const int delay_buffer_length = m_DelayBuffer.getNumSamples();
//
//    const float* buffer_data = buffer.getReadPointer(channel);
//    const float* delay_buffer_data = m_DelayBuffer.getReadPointer(channel);
//
//    if (delay_buffer_length > buffer_length + m_WritePos) {
//        if (copying)
//            m_DelayBuffer.copyFromWithRamp(channel, m_WritePos, buffer_data, buffer_length, start_gain, end_gain);
//        else
//            m_DelayBuffer.addFromWithRamp(channel, m_WritePos, buffer_data, buffer_length, start_gain, end_gain);
//    }
//    else {
//        // fill end of buffer and carry over to beginning
//        const int remainder = delay_buffer_length - m_WritePos;
//        const double mid_gain = juce::jmap(float(remainder) / buffer_length, start_gain, end_gain);
//        if (copying) {
//            m_DelayBuffer.copyFromWithRamp(channel, m_WritePos, buffer_data, remainder, start_gain, mid_gain);
//            m_DelayBuffer.copyFromWithRamp(channel, 0, buffer_data + remainder, buffer_length - remainder, mid_gain, end_gain);
//        }
//        else {
//            m_DelayBuffer.addFromWithRamp(channel, m_WritePos, buffer_data, remainder, 1.0f, mid_gain);
//            m_DelayBuffer.addFromWithRamp(channel, 0, buffer_data + remainder, buffer_length - remainder, mid_gain, end_gain);
//        }
//    }
//}
//
//void GlitchBunnyAudioProcessor::ReadDelayBuffer(int channel, juce::AudioBuffer<float>& buffer, ParameterSettings& params, const float start_gain, const float end_gain, const double sample_rate)
//{
//    const float* buffer_data = buffer.getReadPointer(channel);
//    const float* delay_buffer_data = m_DelayBuffer.getReadPointer(channel);
//
//    const int buffer_length = buffer.getNumSamples();
//    const int delay_buffer_length = m_DelayBuffer.getNumSamples();
//
//    const int read_pos = static_cast<int>(delay_buffer_length + m_WritePos - (sample_rate * params.time/1000)) % delay_buffer_length;
//
//    if (delay_buffer_length > buffer_length + read_pos) {
//        buffer.addFrom(channel, 0, delay_buffer_data + read_pos, buffer_length);
//    }
//    else {
//        const int remainder = delay_buffer_length - read_pos;
//
//        buffer.addFrom(channel, 0, delay_buffer_data + read_pos, remainder);
//        buffer.addFrom(channel, remainder, delay_buffer_data, buffer_length - remainder);
//    }
//}

//==============================================================================
bool GlitchBunnyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GlitchBunnyAudioProcessor::createEditor()
{
    return new GlitchBunnyAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
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

//void GlitchBunnyAudioProcessor::UpdateRandDelay() 
//{
//    auto p_settings = GetParameterSettings(tree_state);
//
//    int randomness = p_settings.rand;
//    std::default_random_engine generator;
//    std::uniform_int_distribution<int> distribution(-randomness, randomness);
//    int rand = distribution(generator);
//
//    float rand_time = p_settings.time + (rand / 50.f);
//
//
//    mixer.setWetMixProportion(p_settings.mix / 100.0);
//    delay_channel.setDelay(rand_time * getSampleRate());
//    delay_channel.UpdateFeedback(p_settings.feedback);
//}

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