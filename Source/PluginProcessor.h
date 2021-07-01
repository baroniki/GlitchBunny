/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <random>

#include "Logger.h"
//==============================================================================
/**
*/



struct ParameterSettings {
    float mix{ 0.f }, rate{ 0.f }, depth{ 0.f }, feedback{ 0.f }, center_freq{ 1000.f };
};

ParameterSettings GetParameterSettings(const juce::AudioProcessorValueTreeState& tree_state);

class GlitchBunnyAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    GlitchBunnyAudioProcessor();
    ~GlitchBunnyAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    static juce::AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout();
    juce::AudioProcessorValueTreeState tree_state { *this, nullptr, "Parameters", CreateParameterLayout() };

private:
    juce::dsp::Phaser<float> m_Phaser;

    void UpdateParams();

    Logger logger;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlitchBunnyAudioProcessor)
};
