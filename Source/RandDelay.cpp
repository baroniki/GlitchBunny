/*
  ==============================================================================

    RandDelay.cpp
    Created: 29 Jun 2021 11:56:51pm
    Author:  Alex

  ==============================================================================
*/

#include "RandDelay.h"

RandDelay::RandDelay(double sample_rate) : juce::dsp::DelayLine<float>(sample_rate) {
}