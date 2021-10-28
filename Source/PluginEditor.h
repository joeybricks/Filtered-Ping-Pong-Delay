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
class PingPongA2AudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener, public juce::Button::Listener
{
public:
    PingPongA2AudioProcessorEditor (PingPongA2AudioProcessor&);
    ~PingPongA2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    
    
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* Button) override;
    
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PingPongA2AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PingPongA2AudioProcessorEditor)
    
    // Initiate object variables
    
    juce::Slider mGainSlider;
    juce::Slider mDelayLengthSlider;
    juce::Slider mFeedbackSlider;
    juce::Slider mDistDriveSlider;
    juce::Slider mDistBlendSlider;
    juce::Slider mLPFSlider;
    juce::Slider mWetDrySlider;
    juce::Slider mDelayLSlider;
    
    juce::Label mGainLabel;
    juce::Label mDelayLengthLabel;
    juce::Label mFeedbackLabel;
    juce::Label mDistDriveLabel;
    juce::Label mDistBlendLabel;
    juce::Label mLPFLAbel;
    juce::Label mWetDryLabel;
    
    
    juce::ToggleButton mFxButton;
    juce::ToggleButton mParallelButton;
};


