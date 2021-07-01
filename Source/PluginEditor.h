/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "BunnyComponent.h"

//==============================================================================
/**
*/

struct KnobLF : juce::LookAndFeel_V4 {
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
    {
        const float radius = juce::jmin(width / 2, height / 2) - 4.0f;
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f;
        const float rx = centreX - radius;
        const float ry = centreY - radius;
        const float rw = radius * 2.0f;
        const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // fill
        juce::ColourGradient knob_fill { juce::Colours::green.withAlpha(0.8f), { 0.f, 0.f },
                                            juce::Colours::green.withAlpha(0.4f), { (float) width, (float) height },
                                            false };
        g.setGradientFill(knob_fill);
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(juce::Colours::green);
        g.drawEllipse(rx, ry, rw, rw, 1.0f);

        juce::Path p;
        const float pointerLength = radius * 0.33f;
        const float pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        // pointer
        g.setColour(juce::Colours::white);
        g.fillPath(p);
    }
};

struct CustomSlider : juce::Slider {
    CustomSlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow) {}
};

struct CustomLabel : juce::Label {
    CustomLabel() : juce::Label("", "") {
        this->setJustificationType(juce::Justification::centred);
        this->setFont(juce::Font(18.0f, juce::Font::italic));
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
    GlitchBunnyAudioProcessor& audioProcessor;

    KnobLF lf;

    CustomSlider slider_mix, slider_rate, slider_depth, slider_feedback, slider_center_freq;
    CustomLabel label_mix, label_rate, label_depth, label_feedback, label_center_freq;

    // BunnyComponent bunny;
    juce::ImageComponent bunny_image;
    

    std::vector<juce::Component*> GetGUIComponents();
    std::vector<CustomSlider*> GetSliders();
    std::vector<juce::Label*> GetLabels();
    void InitLabels();
    void InitSliders();

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    Attachment attachment_mix, attachment_rate, attachment_depth, attachment_feedback, attachment_center_freq;

    juce::Matrix3D<float> getProjectionMatrix() const;
    juce::Matrix3D<float> getViewMatrix() const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlitchBunnyAudioProcessorEditor)
};
