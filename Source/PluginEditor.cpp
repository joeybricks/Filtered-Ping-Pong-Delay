/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PingPongA2AudioProcessorEditor::PingPongA2AudioProcessorEditor (PingPongA2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
      mGainSlider.setSliderStyle(juce::Slider::LinearVertical);
      mGainSlider.setRange(0.0f,1.0f,0.01f);
      mGainSlider.setValue(0.5f);
      mGainSlider.addListener(this);
      addAndMakeVisible(mGainSlider);
      mGainSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, mGainSlider.getTextBoxHeight());
      mGainLabel.setText("Gain", juce::dontSendNotification);
      addAndMakeVisible(mGainLabel);
      mGainLabel.attachToComponent(&mGainSlider, false);
////
    mDelayLSlider.setSliderStyle(juce::Slider::LinearVertical);
    mDelayLSlider.setRange(5000.0f,48000,1.0f);
    mDelayLSlider.setValue(15000.0f);
    mDelayLSlider.addListener(this);
    addAndMakeVisible(mDelayLSlider);
    mDelayLSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, mDelayLSlider.getTextBoxHeight());
    mDelayLengthLabel.setText("Delay Length", juce::dontSendNotification);
    addAndMakeVisible(mDelayLengthLabel);
    mDelayLengthLabel.attachToComponent(&mDelayLSlider, false);

    mFeedbackSlider.setSliderStyle(juce::Slider::LinearVertical);
    mFeedbackSlider.setRange(0.0f,0.99f,0.01f);
    mFeedbackSlider.setValue(0.5f);
    mFeedbackSlider.addListener(this);
    addAndMakeVisible(mFeedbackSlider);
    mFeedbackSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, mFeedbackSlider.getTextBoxHeight());
    mFeedbackLabel.setText("Feedback", juce::dontSendNotification);
    addAndMakeVisible(mFeedbackLabel);
    mFeedbackLabel.attachToComponent(&mFeedbackSlider, false);

    mDistDriveSlider.setSliderStyle(juce::Slider::LinearVertical);
    mDistDriveSlider.setRange(1.0f,2.7f,0.01f);
    mDistDriveSlider.setValue(0.5f);
    mDistDriveSlider.addListener(this);
    addAndMakeVisible(mDistDriveSlider);
    mDistDriveSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, mDistDriveSlider.getTextBoxHeight());
    mDistDriveLabel.setText("Drive", juce::dontSendNotification);
    addAndMakeVisible(mDistDriveLabel);
    mDistDriveLabel.attachToComponent(&mDistDriveSlider, false);

    mDistBlendSlider.setSliderStyle(juce::Slider::LinearVertical);
    mDistBlendSlider.setRange(0.7f,1.0f,0.01f);
    mDistBlendSlider.setValue(1.0f);
    mDistBlendSlider.addListener(this);
    addAndMakeVisible(mDistBlendSlider);
    mDistBlendSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, mDistBlendSlider.getTextBoxHeight());
    mDistBlendLabel.setText("Blend", juce::dontSendNotification);
    addAndMakeVisible(mDistBlendLabel);
    mDistBlendLabel.attachToComponent(&mDistBlendSlider, false);

    mLPFSlider.setSliderStyle(juce::Slider::LinearVertical);
    mLPFSlider.setRange(1.0f,22000,1.0f); 
    mLPFSlider.setValue(1000.0f);
    mLPFSlider.addListener(this);
    addAndMakeVisible(mLPFSlider);
    mLPFSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, mLPFSlider.getTextBoxHeight());
    mLPFLAbel.setText("LPF", juce::dontSendNotification);
    addAndMakeVisible(mLPFLAbel);
    mLPFLAbel.attachToComponent(&mLPFSlider, false);
    
    mWetDrySlider.setSliderStyle(juce::Slider::LinearVertical);
    mWetDrySlider.setRange(0.0f,1.0f,0.01f);
    mWetDrySlider.setValue(0.5f);
    mWetDrySlider.addListener(this);
    addAndMakeVisible(mWetDrySlider);
    mWetDrySlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, mWetDrySlider.getTextBoxHeight());
    mWetDryLabel.setText("Dry/Wet", juce::dontSendNotification);
    addAndMakeVisible(mWetDryLabel);
    mWetDryLabel.attachToComponent(&mWetDrySlider, false);
    

    
    mFxButton.setButtonText("Fx");
       mFxButton.addListener(this);
       addAndMakeVisible(mFxButton);
    
    mParallelButton.setButtonText("Parallel On");
       mParallelButton.addListener(this);
       addAndMakeVisible(mParallelButton);
    
    setSize (600, 600);
}

PingPongA2AudioProcessorEditor::~PingPongA2AudioProcessorEditor()
{
}

//==============================================================================
void PingPongA2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Ping Pong Delay", getLocalBounds(), juce::Justification::centred, 1);
}

void PingPongA2AudioProcessorEditor::resized()
{
    mGainSlider.setBounds(40, 50, 60, 200);
    mDelayLSlider.setBounds(120, 50, 60, 200);
    mFeedbackSlider.setBounds(200, 50, 60, 200);
    mDistDriveSlider.setBounds(280, 50, 60, 200);
    mDistBlendSlider.setBounds(360, 50, 60, 200);
    mLPFSlider.setBounds(440, 50, 60, 200);
    mWetDrySlider.setBounds(520, 50, 60, 200);
    
    mFxButton.setBounds(40, 300, 60, 60);
    mParallelButton.setBounds(200, 300, 60, 60);

    
}
// Conecting sliders to variables in the processor
void PingPongA2AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &mGainSlider){
        audioProcessor.mLevel = mGainSlider.getValue();
    }

    if (slider == &mDelayLSlider){
        audioProcessor.mDL = mDelayLSlider.getValue();
    }

    if (slider == &mFeedbackSlider){
        audioProcessor.mFb = mFeedbackSlider.getValue();
    }

    if (slider == &mDistDriveSlider){
        audioProcessor.mDrive = mDistDriveSlider.getValue();
    }

    if (slider == &mDistBlendSlider){
        audioProcessor.mDistBlend = mDistBlendSlider.getValue();
    }
    if (slider == &mLPFSlider){
        audioProcessor.mFc = mLPFSlider.getValue();
   }
    if (slider == &mWetDrySlider){
        audioProcessor.mDry = mWetDrySlider.getValue();
    }

}
// Conecting sliders to variables in the processor
void PingPongA2AudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &mFxButton){
        audioProcessor.mFxOn = mFxButton.getToggleState();
    }
    if (button == &mParallelButton){
        audioProcessor.mParallelOn = mParallelButton.getToggleState();
    }
}
