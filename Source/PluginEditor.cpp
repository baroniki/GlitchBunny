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
    attachment_mix(audioProcessor.tree_state, "Mix", slider_mix),
    attachment_rate(audioProcessor.tree_state, "Rate", slider_rate),
    attachment_depth(audioProcessor.tree_state, "Depth", slider_depth),
    attachment_feedback(audioProcessor.tree_state, "Feedback", slider_feedback),
    attachment_center_freq(audioProcessor.tree_state, "Center Freq", slider_center_freq)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    InitLabels();
    InitSliders();

    for (auto* comp : GetGUIComponents()) {
        addAndMakeVisible(comp);
    }
    setSize (800, 500);
}

void GlitchBunnyAudioProcessorEditor::InitLabels() {
    auto sliders = GetSliders();

    label_mix.setText("Mix", juce::NotificationType::dontSendNotification);
    label_mix.attachToComponent(sliders[0], false);

    label_rate.setText("Rate", juce::NotificationType::dontSendNotification);
    label_rate.attachToComponent(sliders[1], false);
    
    label_depth.setText("Depth", juce::NotificationType::dontSendNotification);
    label_depth.attachToComponent(sliders[2], false);

    label_feedback.setText("Feedback", juce::NotificationType::dontSendNotification);
    label_feedback.attachToComponent(sliders[3], false);

    label_center_freq.setText("Center Frequency", juce::NotificationType::dontSendNotification);
    label_center_freq.attachToComponent(sliders[4], false);
}

void GlitchBunnyAudioProcessorEditor::InitSliders() {
    for (auto* slider : GetSliders()) {
        slider->setLookAndFeel(&lf);
    }
}

GlitchBunnyAudioProcessorEditor::~GlitchBunnyAudioProcessorEditor()
{
}

std::vector<juce::Component*> GlitchBunnyAudioProcessorEditor::GetGUIComponents()
{
    return {
        &slider_mix, &slider_rate, &slider_depth, &slider_feedback, &slider_center_freq,
        &label_mix, &label_rate, &label_depth, &label_feedback, &label_center_freq
    };
}

std::vector<CustomSlider*> GlitchBunnyAudioProcessorEditor::GetSliders()
{
    return {
        &slider_mix, & slider_rate, &slider_depth, &slider_feedback, &slider_center_freq
    };
}

//==============================================================================
void GlitchBunnyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);

    auto bounds = getLocalBounds();

    auto bunny_area = bounds.removeFromTop(bounds.getHeight() * 0.8);

    g.setColour(juce::Colours::white);
    g.drawHorizontalLine(bunny_area.getHeight(), 0.f, bunny_area.getWidth());
    
    juce::ColourGradient knobs_background{  juce::Colours::white.withAlpha(0.5f), { 0.f, 0.f }, 
                                            juce::Colours::white.withAlpha(0.2f), { (float) bounds.getWidth(), (float) bounds.getHeight() }, 
                                            false   };
    g.setGradientFill(knobs_background);
    g.fillRect(bounds);
}

void GlitchBunnyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();

    auto bunny_area = bounds.removeFromTop(bounds.getHeight() * 0.8);

    auto comps = GetSliders();
    double comp_ctr = comps.size();


    for ( int i = 0; i < comps.size(); i++ ) {
        // remove 1/4 of bounds, then 1/3, 1/2 until last component
        if (i == comps.size() - 1)
            comps[i]->setBounds(bounds);
        else
            comps[i]->setBounds(bounds.removeFromLeft(bounds.getWidth() * (1 / comp_ctr--)));
    }
}
