/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
struct RotarySlider : juce::Slider
{
    RotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow)
    {

    }
};

class GlitchBunnyAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GlitchBunnyAudioProcessorEditor (GlitchBunnyAudioProcessor&);
    ~GlitchBunnyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GlitchBunnyAudioProcessor& audioProcessor;

    RotarySlider slider_mix, slider_time, slider_feedback, slider_rand;
    juce::Label label_mix, label_time, label_feedback, label_rand;

    std::vector<juce::Component*> GetGUIComponents();
    std::vector<RotarySlider*> GetSliders();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlitchBunnyAudioProcessorEditor)
};
