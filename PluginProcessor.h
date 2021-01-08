/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSPDelayLineTutorial_02.h"

//==============================================================================
/**
*/
class A3AudioProcessor  : public juce::AudioProcessor
{
public:
    

    bool selected = true;
    int  reverbChoice;
    double samplesPerSec;
    juce::dsp::Convolution constructedConvolution;
    juce::dsp::Convolution convolution;
    juce::AudioBuffer<float> constructedImpulseResponse;
    //==============================================================================
    A3AudioProcessor();
    ~A3AudioProcessor() override;

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
    
    void updateFX();
    void updateParameters ();
    juce::AudioProcessorValueTreeState apvts;

    
private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    /*
    juce::dsp::StateVariableTPTFilter<float> stateVariableFilter;
    bool bypassFilter = false;
    
    enum { phaserIndex, gainIndex };
    juce::dsp::ProcessorChain<juce::dsp::Phaser<float>,juce::dsp::Gain<float>> fxChain;
    bool bypassPhaser = false;
    */
    juce::dsp::Reverb reverb;
    bool bypassReverb = true;

    
    bool bypassConvolution= true;
    
    juce::AudioBuffer<float>  outputBuffer;
    
    
    bool bypassConstructedConvolution = true;
    

    enum{delay1Index, delay2Index, delay3Index, delay4Index, filterIndex};
    juce::dsp::ProcessorChain <Delay<float> ,
        Delay<float>,
        Delay<float>,
        Delay<float>,
        juce::dsp::StateVariableTPTFilter <float>> delayChain;
    bool bypassDelayChain = true;

    int oldIR;
    bool first = true;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (A3AudioProcessor)
};
