/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "DSPDelayLineTutorial_02.h"

//==============================================================================
/**
*/
class A3AudioProcessorEditor : public juce::AudioProcessorEditor, private juce::ComboBox::Listener, private juce::Slider::Listener

{
public:
    A3AudioProcessorEditor (A3AudioProcessor&);
    ~A3AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    A3AudioProcessor& audioProcessor;
    void A3AudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox);
    void A3AudioProcessorEditor::sliderValueChanged(juce::Slider* slider);
   
    juce::ComboBox impulseResponseMenu;
    juce::ComboBox constructedIRMenu;
    juce::ComboBox reverbMenu;

    juce::Slider roomSizeSlider;
    juce::Slider dampingSlider;
    juce::Slider wetLevelSlider;
    juce::Slider dryLevelSlider;
    juce::Slider widthSlider;
    juce::Slider freezeModeSlider;

    juce::Slider delay1WetLevelSlider;
    juce::Slider delay1DelayTimeSlider;
    juce::Slider delay1FeedbackSlider;

    juce::Slider delay2WetLevelSlider;
    juce::Slider delay2DelayTimeSlider;
    juce::Slider delay2FeedbackSlider;

    juce::Slider delay3WetLevelSlider;
    juce::Slider delay3DelayTimeSlider;
    juce::Slider delay3FeedbackSlider;

    juce::Slider delay4WetLevelSlider;
    juce::Slider delay4DelayTimeSlider;
    juce::Slider delay4FeedbackSlider;

    juce::Slider cutoffSlider;

    juce::Slider reverbLengthSlider;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> impulseResponseMenuValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> constructedIRMenuValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> reverbMenuValue;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roomSizeValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampingValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetLevelValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryLevelValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freezeModeValue;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delay1WetLevelValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delay1DelayTimeValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delay1FeedbackValue;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delay2WetLevelValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delay2DelayTimeValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delay2FeedbackValue;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delay3WetLevelValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delay3DelayTimeValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delay3FeedbackValue;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delay4WetLevelValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delay4DelayTimeValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delay4FeedbackValue;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffValue;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbLengthValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (A3AudioProcessorEditor)
};
