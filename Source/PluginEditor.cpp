/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GlitchBunnyAudioProcessorEditor::GlitchBunnyAudioProcessorEditor (GlitchBunnyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), 
    label_mix("Mix Label", "Mix"), label_time("Time Label", "Time"), label_feedback("Feedback Label", "Feedback"), label_rand("Random Label", "Randomness")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    for (auto* comp : GetGUIComponents()) {
        addAndMakeVisible(comp);
    }
    setSize (800, 500);
}

GlitchBunnyAudioProcessorEditor::~GlitchBunnyAudioProcessorEditor()
{
}

std::vector<juce::Component*> GlitchBunnyAudioProcessorEditor::GetGUIComponents()
{
    return {
        &slider_mix, &slider_time, &slider_feedback, &slider_rand,
        &label_mix, &label_time, &label_feedback, &label_rand
    };
}

std::vector<RotarySlider*> GlitchBunnyAudioProcessorEditor::GetSliders()
{
    return {
        &slider_mix, &slider_time, &slider_feedback, &slider_rand
    };
}

//==============================================================================
void GlitchBunnyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);

    auto bounds = getLocalBounds();

    auto bunny_area = bounds.removeFromTop(bounds.getHeight() * 0.75);

    g.setColour(juce::Colours::green);
    g.drawHorizontalLine(bunny_area.getHeight(), 0.f, bunny_area.getWidth());
    
    juce::ColourGradient knobs_background{  juce::Colours::green.withAlpha(0.5f), { 0.f, 0.f }, 
                                            juce::Colours::green.withAlpha(0.2f), { (float) bounds.getWidth(), (float) bounds.getHeight() }, 
                                            false   };
    g.setGradientFill(knobs_background);
    g.fillRect(bounds);
}

void GlitchBunnyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();

    auto bunny_area = bounds.removeFromTop(bounds.getHeight() * 0.75);

    auto comps = GetGUIComponents();
    double comp_ctr, num_knobs = comps.size()/2;

    for ( int i = 0; i < num_knobs; i++ ) {
        // remove 1/5 of bounds, then 1/4, 1/3 etc... until last component
        if (i == comps.size() - 1) {
            comps[i + num_knobs]->setBounds(bounds.removeFromTop(bounds.getHeight() * 0.2));
            comps[i]->setBounds(bounds);
        } 
        else {
            auto knob_bounds = bounds.removeFromLeft(bounds.getWidth() * (1 / comp_ctr--));
            comps[i + num_knobs]->setBounds(knob_bounds.removeFromTop(knob_bounds.getHeight() * 0.2));
            comps[i]->setBounds(knob_bounds);

        }
    }
}
