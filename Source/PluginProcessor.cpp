/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PingPongA2AudioProcessor::PingPongA2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

PingPongA2AudioProcessor::~PingPongA2AudioProcessor()
{
    if (mDDLL != NULL){
        delete [] mDDLL;
    }
    if (mDDLR != NULL){
        delete [] mDDLR;
    }
}

//==============================================================================
const juce::String PingPongA2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PingPongA2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PingPongA2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PingPongA2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PingPongA2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PingPongA2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PingPongA2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void PingPongA2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PingPongA2AudioProcessor::getProgramName (int index)
{
    return {};
}

void PingPongA2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PingPongA2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Sample rate aquisirion
    int Fs = getSampleRate();
    mFs = Fs;
    // clear delay lines
    if (mDDLL != NULL){
        delete[] mDDLL;
    }
    if (mDDLR != NULL){
        delete[] mDDLR;
    }
    
    // Assign delay maximum memory size of 1 second
    mDDLL = new float[Fs];
    mDDLR = new float[Fs];
    
    // Flush delay line with 0's
    for (int n=0; n<Fs; n++){
        mDDLL[n] = 0.0f;
        mDDLR[n] = 0.0f;
    }
    
    // Initiate read/write index
    mrwInx = 0;

   
   

 
    
}

void PingPongA2AudioProcessor::releaseResources()
{
   
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PingPongA2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PingPongA2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Aquire number of samples
    int numSamples = buffer.getNumSamples();
    if (numSamples == 0){
        return;
    }
    
    // Aquire number of input and output channels
    int numChannels = buffer.getNumSamples();
    int outputChannels = getTotalNumOutputChannels();
    
    // Assign writing pointers for each channel
    auto channelDataL = buffer.getWritePointer(0);
    auto channelDataR = buffer.getWritePointer(1);
    
    if (numChannels == 2){
        channelDataR = buffer.getWritePointer(1);
    }
    
    

    // Input variable initiation
    float inputL = 0.0f;
    float inputR = 0.0f;
 
    // Previous values initiation
    float zX1L = 0;
    float zX2L = 0;
    float zY1L = 0;
    float zY2L = 0;
   
    float zX1R = 0;
    float zX2R = 0;
    float zY1R = 0;
    float zY2R = 0;

    // Main Time loop
    for (int n = 0; n < numSamples; n++){
        float blend = 1-mDistBlend;
        float wet = 1-mDry;
        
        inputL = buffer.getSample(0, n);
        inputR = buffer.getSample(1, n);
        
        
        // Implement gain
        channelDataL[n] = inputL * mLevel;
        channelDataR[n] = inputR * mLevel;
        
        // Read from delay line
        channelDataL[n] = ((mDry*channelDataL[n]) + (wet*mDDLL[mrwInx])) * mLevel;
        channelDataR[n] = ((mDry*channelDataR[n]) + (wet*mDDLR[mrwInx])) * mLevel;
        
        
        // If Fx is on do this ...
        if (mFxOn == true){
        // Temp variables to add distortion to feedback
        auto DistL = (mDistBlend*mDDLL[mrwInx]) + blend*tanhf(mDrive*mDDLL[mrwInx]);
        auto DistR = (mDistBlend*mDDLR[mrwInx]) + blend*tanhf(mDrive*mDDLR[mrwInx]);

        // Filter equation variables
        mTheta = M_PI*mFc/mFs;
        mOhm = M_PI*mFc;
        mKappa = mOhm/tanf(mTheta);
        mDel = pow(mKappa,2) + pow(mOhm,2) + 2*mKappa*mOhm;
        
      // Declare coefficients
        float b0 = (pow(mOhm,2))/mDel;
        float b1 = 2*b0;
        float b2 = b0;
        float a1 = (-2*pow(mKappa,2) + 2*pow(mOhm,2))/mDel;
        float a2 = (-2*mKappa*mOhm + pow(mKappa,2)+ pow(mOhm,2))/mDel;
        
        // If Parallel is on do this...
            if (mParallelOn == false){
        // Filter Difference equations
        auto LPFL = b0*DistL+ b1*zX1L + b2*zX2L - a1*zY1L - a2*zY2L;

        zX2L = zX1L;
        zX1L = DistL;
        zY2L = zY1L;
        zY1L = LPFL;

        auto LPFR = b0*DistR + b1*zX1R + b2*zX2R - a1*zY1R - a2*zY2R;
        
        zX2R = zX1R;
        zX1R = DistR;
        zY2R = zY1R;
        zY1R = LPFR;
        
        // Write to delay line
          
        mDDLL[mrwInx] = inputR + mFb * LPFR;
        mDDLR[mrwInx] = inputL + mFb * LPFL;
            }
            if (mParallelOn == true){
                auto LPFL = b0*mDDLL[mrwInx] + b1*zX1L + b2*zX2L - a1*zY1L - a2*zY2L;

                zX2L = zX1L;
                zX1L = mDDLL[mrwInx];
                zY2L = zY1L;
                zY1L = LPFL;

                auto LPFR = b0*mDDLR[mrwInx] + b1*zX1R + b2*zX2R - a1*zY1R - a2*zY2R;
                
                zX2R = zX1R;
                zX1R = mDDLR[mrwInx];
                zY2R = zY1R;
                zY1R = LPFR;
                
                // Write to delay line
                  
                mDDLL[mrwInx] = inputR + mFb * (0.5*LPFR + 0.5*DistR);
                mDDLR[mrwInx] = inputL + mFb * (0.5*LPFL + 0.5*DistL);
                
            }
        }
        // If Fx is off do this...
       if (mFxOn == false){
 
            auto tempDDL = mDDLL[mrwInx];
        mDDLL[mrwInx] = inputR + mFb * mDDLR[mrwInx];
        mDDLR[mrwInx] = inputL + mFb * tempDDL;
        }
        
        // Increment read/write index
        mrwInx++;
        
        // Implement circular buffer, wrap round delay length
        if(mrwInx > mDL-1){
            mrwInx = 0;
        }
    }
    
    
}

//==============================================================================
bool PingPongA2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PingPongA2AudioProcessor::createEditor()
{
    return new PingPongA2AudioProcessorEditor (*this);
}

//==============================================================================
void PingPongA2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PingPongA2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PingPongA2AudioProcessor();
}
