/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DSPDelayLineTutorial_02.h"

//==============================================================================
A3AudioProcessor::A3AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

A3AudioProcessor::~A3AudioProcessor()
{
}

//==============================================================================
const juce::String A3AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool A3AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool A3AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool A3AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double A3AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int A3AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int A3AudioProcessor::getCurrentProgram()
{
    return 0;
}

void A3AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String A3AudioProcessor::getProgramName (int index)
{
    return {};
}

void A3AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void A3AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();

    outputBuffer.setSize(2, samplesPerBlock);
    outputBuffer.clear();

    

   //DBG("SR: " << sampleRate);
    samplesPerSec = sampleRate;
    /*
    stateVariableFilter.reset();
    fxChain.reset();
    */
    reverb.reset();
    convolution.reset();
    delayChain.reset();
    constructedConvolution.reset();
    /*
    stateVariableFilter.prepare(spec);
    fxChain.prepare(spec);
    */

    reverb.prepare(spec);
    convolution.prepare(spec);
    constructedConvolution.prepare(spec);
    delayChain.prepare(spec);
    
    if(first)
    { 
    auto& delay1Processor = delayChain.template get<delay1Index>();
    delay1Processor.setMaxDelayTime(1);

    auto& delay2Processor = delayChain.template get<delay2Index>();
    delay2Processor.setMaxDelayTime(1);

    auto& delay3Processor = delayChain.template get<delay3Index>();
    delay3Processor.setMaxDelayTime(1);

    auto& delay4Processor = delayChain.template get<delay4Index>();
    delay4Processor.setMaxDelayTime(1);

    juce::File file = juce::File::getSpecialLocation(juce::File::userDesktopDirectory);
    file = file.getChildFile("Resources/SquareVictoriaDome.wav");


    convolution.loadImpulseResponse(file, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0, juce::dsp::Convolution::Normalise::yes);
    //====================================================================
    /*
    juce::AudioBuffer<float> constructedImpulseResponse;
    constructedImpulseResponse.setSize(2, 6 * sampleRate);
    constructedImpulseResponse.clear();
    for (int channel = 0; channel < 2; channel++)
    {
        constructedImpulseResponse.setSize(2, samplesPerSec);
        auto* data = constructedImpulseResponse.getWritePointer(channel);
        for (int i = 0; i < 2*samplesPerSec; i++)
        {
            float offset = (((float)i + 1.0) / ((float)samplesPerSec *2));
            data[i] = 1.0 - offset;
            //DBG("of: " << offset);
        }
    }
    constructedConvolution.loadImpulseResponse(std::move(constructedImpulseResponse), samplesPerSec, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, juce::dsp::Convolution::Normalise::no);
    */
    first = false;
    }

    

    updateFX();
}

void A3AudioProcessor::updateFX()
{
   
    //impulseResponseChoice = *apvts.getRawParameterValue("IMPULSERESPONSEMENU");
   /*
    if (oldIR != impulseResponseChoice)
    {
        selected = true;
    }
    oldIR = impulseResponseChoice;
    */
    reverbChoice = *apvts.getRawParameterValue("REVERBMENU");

    int roomSize = *apvts.getRawParameterValue("ROOMSIZE");
    int damping = *apvts.getRawParameterValue("DAMPING");
    int wetLevel = *apvts.getRawParameterValue("WETLEVEL");
    int dryLevel = *apvts.getRawParameterValue("DRYLEVEL");
    int width = *apvts.getRawParameterValue("WIDTH");
    int freezeMode = *apvts.getRawParameterValue("FREEZEMODE");
    
    float delay1DelayTime = *apvts.getRawParameterValue("DELAY1DELAYTIME");
    float delay1Feedback = *apvts.getRawParameterValue("DELAY1FEEDBACK");
    float delay1WetLevel = *apvts.getRawParameterValue("DELAY1WETLEVEL");

    float delay2DelayTime = *apvts.getRawParameterValue("DELAY2DELAYTIME");
    float delay2Feedback = *apvts.getRawParameterValue("DELAY2FEEDBACK");
    float delay2WetLevel = *apvts.getRawParameterValue("DELAY2WETLEVEL");

    float delay3DelayTime = *apvts.getRawParameterValue("DELAY3DELAYTIME");
    float delay3Feedback = *apvts.getRawParameterValue("DELAY3FEEDBACK");
    float delay3WetLevel = *apvts.getRawParameterValue("DELAY3WETLEVEL");

    float delay4DelayTime = *apvts.getRawParameterValue("DELAY4DELAYTIME");
    float delay4Feedback = *apvts.getRawParameterValue("DELAY4FEEDBACK");
    float delay4WetLevel = *apvts.getRawParameterValue("DELAY4WETLEVEL");


    float cutoff = *apvts.getRawParameterValue("CUTOFF");
    
   

    if (reverbChoice == 1)
    {
        bypassReverb = false;

        bypassConvolution = true;
        bypassConstructedConvolution = true;
        bypassDelayChain = true;
    }
    if (reverbChoice == 2)
    {
        bypassConvolution = false;

        bypassReverb = true;
        bypassDelayChain = true;
        bypassConstructedConvolution = true;

    }
    if (reverbChoice == 3)
    {
        bypassConstructedConvolution = false;

        bypassReverb = true;
        bypassConvolution = true;
        bypassDelayChain = true;
        
    }
    if (reverbChoice == 4)
    {
        bypassDelayChain = false;

        bypassReverb = true;
        bypassConvolution = true;
        bypassConstructedConvolution = true;
        
    }
    if (reverbChoice == 5)
    {
        bypassConvolution = true;
        bypassConstructedConvolution = true;
        bypassReverb = true;
        bypassDelayChain = true;
    }

    //auto& gainProcessor = fxChain.template get<gainIndex>();
    //gainProcessor.setGainLinear (gain);
     
    //auto& phaserProcessor = fxChain.template get<phaserIndex>();
    //phaserProcessor.setRate (phaserRate);
    //phaserProcessor.setDepth (phaserDepth);

    float mRoomSize = (float)roomSize / 100.0;
    float mDamping = (float)damping / 100.0;
    float mWetLevel = (float)wetLevel / 100.0;
    float mDryLevel = (float)dryLevel / 100.0;
    float mWidth = (float)width / 100.0;
    float mFreeze = (float)freezeMode / 100.0;
    reverb.setParameters({mRoomSize,mDamping,mWetLevel, mDryLevel, mWidth, mFreeze});
    
    auto& delay1Processor = delayChain.template get<delay1Index>();
    delay1Processor.setDelayTime(0,((float)delay1DelayTime/1000));
    delay1Processor.setDelayTime(1, ((float)delay1DelayTime / 1000));
    delay1Processor.setWetLevel((float)delay1WetLevel/100 );
    delay1Processor.setFeedback((float)delay1Feedback/100);
 
    auto& delay2Processor = delayChain.template get<delay2Index>();
    delay2Processor.setDelayTime(0, ((float)delay2DelayTime / 1000) );
    delay2Processor.setDelayTime(1, ((float)delay2DelayTime / 1000));
    delay2Processor.setWetLevel((float)delay2WetLevel / 100);
    delay2Processor.setFeedback((float)delay2Feedback / 100);

    auto& delay3Processor = delayChain.template get<delay3Index>();
    delay3Processor.setDelayTime(0, ((float)delay3DelayTime / 1000));
    delay3Processor.setDelayTime(1, ((float)delay3DelayTime / 1000)) ;
    delay3Processor.setWetLevel((float)delay3WetLevel / 100);
    delay3Processor.setFeedback((float)delay3Feedback / 100);

    auto& delay4Processor = delayChain.template get<delay4Index>();
    delay4Processor.setDelayTime(0, ((float)delay4DelayTime / 1000)) ;
    delay4Processor.setDelayTime(1, ((float)delay4DelayTime / 1000) );
    delay4Processor.setWetLevel((float)delay4WetLevel / 100);
    delay4Processor.setFeedback((float)delay4Feedback / 100);

    auto& filterProcessor = delayChain.template get<filterIndex>();
    filterProcessor.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    filterProcessor.setCutoffFrequency(cutoff);
   
}

void A3AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool A3AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void A3AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    updateFX();
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::AudioBlock<float> outputBlock(outputBuffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    juce::dsp::ProcessContextNonReplacing<float> context2(block,outputBlock);
    //auto& input = context2.getInputBlock();
    //auto& output = context2.getOutputBlock();

    if (!bypassReverb) reverb.process(context);
    if (!bypassConvolution)
    {
        
            convolution.process(context2);
            buffer.addFrom(0, 0, outputBuffer, 0, 0, outputBuffer.getNumSamples(), .8);
            buffer.addFrom(1, 0, outputBuffer, 1, 0, outputBuffer.getNumSamples(), .8);
    }
    if (!bypassConstructedConvolution)
    {
        constructedConvolution.process(context2);
        //block += outputBlock;
        buffer.addFrom(0, 0, outputBuffer, 0, 0, outputBuffer.getNumSamples(), 1);
        buffer.addFrom(1, 0, outputBuffer, 1, 0, outputBuffer.getNumSamples(), 1);
    }
    if (!bypassDelayChain)
    {
        delayChain.process(context2);
        
        block += outputBlock;
        //buffer.addFrom(0, 0, outputBuffer,0, 0,outputBuffer.getNumSamples(),1);
        //buffer.addFrom(1, 0, outputBuffer, 1, 0, outputBuffer.getNumSamples(), 1);
    }
    
}

//==============================================================================
bool A3AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* A3AudioProcessor::createEditor()
{
    return new A3AudioProcessorEditor (*this);
}

//==============================================================================
void A3AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void A3AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new A3AudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout A3AudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
 
   
    layout.add (std::make_unique<juce::AudioParameterInt> ("IMPULSERESPONSEMENU", "Impulse Response Menu", 1, 6 ,1));
    layout.add(std::make_unique<juce::AudioParameterInt>("CONSTRUCTEDIRMENU", "Constructed IR Menu", 1, 2, 1));

    layout.add (std::make_unique<juce::AudioParameterInt> ("REVERBMENU", "Reverb Menu", 1, 5, 5));
    layout.add(std::make_unique<juce::AudioParameterInt>("ROOMSIZE", "Room Size", 0, 100, 50));
    layout.add(std::make_unique<juce::AudioParameterInt>("DAMPING", "Damping", 0, 100, 50));
    layout.add(std::make_unique<juce::AudioParameterInt>("WETLEVEL", "Wet Level", 0, 100, 33));
    layout.add(std::make_unique<juce::AudioParameterInt>("DRYLEVEL", "Dry Level", 0, 100, 40));
    layout.add(std::make_unique<juce::AudioParameterInt>("WIDTH", "Width", 0, 100, 100));
    layout.add(std::make_unique<juce::AudioParameterInt>("FREEZEMODE", "Freeze Mode", 0, 100, 0));

    layout.add(std::make_unique<juce::AudioParameterInt>("DELAY1WETLEVEL", "Delay 1 Wet Level", 0, 100, 12));
    layout.add(std::make_unique<juce::AudioParameterInt>("DELAY1FEEDBACK", "Delay 1 Feedback", 0, 100, 70));
    layout.add(std::make_unique<juce::AudioParameterInt>("DELAY1DELAYTIME", "Delay 1 Delay Time", 0, 1000, 12));

    layout.add(std::make_unique<juce::AudioParameterInt>("DELAY2WETLEVEL", "Delay 2 Wet Level", 0, 100, 7));
    layout.add(std::make_unique<juce::AudioParameterInt>("DELAY2FEEDBACK", "Delay 2 Feedback", 0, 100, 42));
    layout.add(std::make_unique<juce::AudioParameterInt>("DELAY2DELAYTIME", "Delay 2 Delay Time", 0, 1000, 33));

    layout.add(std::make_unique<juce::AudioParameterInt>("DELAY3WETLEVEL", "Delay 3 Wet Level", 0, 100, 5));
    layout.add(std::make_unique<juce::AudioParameterInt>("DELAY3FEEDBACK", "Delay 3 Feedback", 0, 100, 31));
    layout.add(std::make_unique<juce::AudioParameterInt>("DELAY3DELAYTIME", "Delay 3 Delay Time", 0, 1000, 62));

    layout.add(std::make_unique<juce::AudioParameterInt>("DELAY4WETLEVEL", "Delay 4 Wet Level", 0, 100, 3));
    layout.add(std::make_unique<juce::AudioParameterInt>("DELAY4FEEDBACK", "Delay 4 Feedback", 0, 100, 30));
    layout.add(std::make_unique<juce::AudioParameterInt>("DELAY4DELAYTIME", "Delay 4 Delay Time", 0, 1000, 93));
   
    layout.add(std::make_unique<juce::AudioParameterInt>("CUTOFF", "Cutoff", 20, 20000, 600));

    layout.add(std::make_unique<juce::AudioParameterFloat>("REVERBLENGTH", "Reverb Length", 0.0f, 6.0f, 2.0f));
    return layout;
}
