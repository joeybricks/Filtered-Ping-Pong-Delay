/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class PingPongA2AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    PingPongA2AudioProcessor();
    ~PingPongA2AudioProcessor() override;

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
    
    // Delay Line Params
    int mDL, mrwInx;
    float mFb, mDry, mLevel;
    
    // Filter Params
    int mFs;
    float mFc;
    float mTheta, mOhm, mKappa, mDel;
    
    // Distortion Params
    float mDistBlend, mDrive;
    
    
    // Delay Line Pointers
    float* mDDLL = nullptr;
    float* mDDLR = nullptr;
    
    // Buttons
    bool mFxOn;
    bool mParallelOn;
    
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PingPongA2AudioProcessor)
};
