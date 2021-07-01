/*
  ==============================================================================

    RandDelay.h
    Created: 29 Jun 2021 11:56:51pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

const float MAX_DELAY = 2.f;

class RandDelay : public juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> {
public:
    RandDelay(double sample_rate);
private:
};
