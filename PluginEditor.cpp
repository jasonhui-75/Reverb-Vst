/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DSPDelayLineTutorial_02.h"

//==============================================================================
A3AudioProcessorEditor::A3AudioProcessorEditor (A3AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (800, 650);
    

    impulseResponseMenu.setJustificationType(juce::Justification::centred);
    impulseResponseMenu.addItem("2) Recorded Impulse Response: Square Victoria Dome", 1);
    impulseResponseMenu.addItem("2) Recorded Impulse Response: Transit Center", 2);
    impulseResponseMenu.addItem("2) Recorded Impulse Response: Tunnel To Hell", 3);
    impulseResponseMenu.addItem("2) Recorded Impulse Response: Racquetball Court", 4);
    impulseResponseMenu.addItem("2) Recorded Impulse Response: Woodruff Lane (very short!)", 5);
    impulseResponseMenu.addItem("2) Load Your Own Impulse Response", 6);
   // impulseResponseMenu.addItem("3) Constructed Impulse Response: Short", 7);
    //impulseResponseMenu.addItem("3) Constructed Impulse Response, Long", 8);
    addAndMakeVisible(&impulseResponseMenu);
    //impulseResponseMenu.addListener(this);
    impulseResponseMenu.onChange =[this]{ comboBoxChanged(&impulseResponseMenu); };

    constructedIRMenu.setJustificationType(juce::Justification::centred);
    constructedIRMenu.addItem("3) Constructed Impulse Response: Short (2 seconds)", 1);
    constructedIRMenu.addItem("3) Constructed Impulse Response: Long (4 seconds)", 2);
    addAndMakeVisible(&constructedIRMenu);
    constructedIRMenu.onChange = [this] { comboBoxChanged(&constructedIRMenu); };

    reverbMenu.setJustificationType(juce::Justification::centred);
    reverbMenu.addItem("1) Reverb: DSP", 1);
    reverbMenu.addItem("2) Reverb: Recorded Impulse Response", 2);
    reverbMenu.addItem("3) Reverb: Constructed Impulse Response", 3);
    reverbMenu.addItem("4) Reverb: Delay", 4);
    reverbMenu.addItem("Reverb: Off", 5);
    addAndMakeVisible(&reverbMenu);
    reverbMenu.onChange = [this] { comboBoxChanged(&reverbMenu); };

    roomSizeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    roomSizeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    roomSizeSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&roomSizeSlider);
    roomSizeSlider.setTextValueSuffix(" %");

    dampingSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dampingSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    dampingSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&dampingSlider);

    wetLevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    wetLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    wetLevelSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&wetLevelSlider);
    wetLevelSlider.setTextValueSuffix(" %");

    dryLevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dryLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    dryLevelSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&dryLevelSlider);
    dryLevelSlider.setTextValueSuffix(" %");

    widthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    widthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    widthSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&widthSlider);
    widthSlider.setTextValueSuffix(" %");

    freezeModeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    freezeModeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    freezeModeSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&freezeModeSlider);
    // DELAY CHAIN --------
    delay1WetLevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delay1WetLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delay1WetLevelSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delay1WetLevelSlider);
    delay1WetLevelSlider.setTextValueSuffix(" %");

    delay1DelayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delay1DelayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 113 - 30, 20);
    delay1DelayTimeSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delay1DelayTimeSlider);
    delay1DelayTimeSlider.setTextValueSuffix(" ms");

    delay1FeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delay1FeedbackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delay1FeedbackSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delay1FeedbackSlider);
    delay1FeedbackSlider.setTextValueSuffix(" %");

    delay2WetLevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delay2WetLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delay2WetLevelSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delay2WetLevelSlider);
    delay2WetLevelSlider.setTextValueSuffix(" %");

    delay2DelayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delay2DelayTimeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delay2DelayTimeSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delay2DelayTimeSlider);
    delay2DelayTimeSlider.setTextValueSuffix(" ms");
    delay2DelayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 113 - 30, 20);

    delay2FeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delay2FeedbackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delay2FeedbackSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delay2FeedbackSlider);

    delay2FeedbackSlider.setTextValueSuffix(" %");
    delay3WetLevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delay3WetLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delay3WetLevelSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delay3WetLevelSlider);
    delay3WetLevelSlider.setTextValueSuffix(" %");

    delay3DelayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delay3DelayTimeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delay3DelayTimeSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delay3DelayTimeSlider);
    delay3DelayTimeSlider.setTextValueSuffix(" ms");
    delay3DelayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 113 - 30, 20);

    delay3FeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delay3FeedbackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delay3FeedbackSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delay3FeedbackSlider);
    delay3FeedbackSlider.setTextValueSuffix(" %");

    delay4WetLevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delay4WetLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delay4WetLevelSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delay4WetLevelSlider);
    delay4WetLevelSlider.setTextValueSuffix(" %");

    delay4DelayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delay4DelayTimeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delay4DelayTimeSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delay4DelayTimeSlider);
    delay4DelayTimeSlider.setTextValueSuffix(" ms");
    delay4DelayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 113 - 30, 20);

    delay4FeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delay4FeedbackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delay4FeedbackSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delay4FeedbackSlider);
    delay4FeedbackSlider.setTextValueSuffix(" %");

    cutoffSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 60, 20);
    cutoffSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&cutoffSlider);
    cutoffSlider.setTextValueSuffix(" Hz");
    //-------------------------------------------------------------------------------
    reverbLengthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    reverbLengthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    reverbLengthSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&reverbLengthSlider);
    reverbLengthSlider.addListener(this);
    reverbLengthSlider.setTextValueSuffix(" sec");
    reverbLengthSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 70, 20);
    //--------------
    impulseResponseMenuValue = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "IMPULSERESPONSEMENU", impulseResponseMenu);
    constructedIRMenuValue = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "CONSTRUCTEDIRMENU", constructedIRMenu);
    reverbMenuValue = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "REVERBMENU", reverbMenu);

    roomSizeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ROOMSIZE", roomSizeSlider);
    dampingValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DAMPING", dampingSlider);
    wetLevelValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "WETLEVEL", wetLevelSlider);
    dryLevelValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DRYLEVEL", dryLevelSlider);
    widthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "WIDTH", widthSlider);
    freezeModeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FREEZEMODE", freezeModeSlider);

    delay1WetLevelValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY1WETLEVEL", delay1WetLevelSlider);
    delay1DelayTimeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY1DELAYTIME", delay1DelayTimeSlider);
    delay1FeedbackValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY1FEEDBACK", delay1FeedbackSlider);

    delay2WetLevelValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY2WETLEVEL", delay2WetLevelSlider);
    delay2DelayTimeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY2DELAYTIME", delay2DelayTimeSlider);
    delay2FeedbackValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY2FEEDBACK", delay2FeedbackSlider);

    delay3WetLevelValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY3WETLEVEL", delay3WetLevelSlider);
    delay3DelayTimeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY3DELAYTIME", delay3DelayTimeSlider);
    delay3FeedbackValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY3FEEDBACK", delay3FeedbackSlider);

    delay4WetLevelValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY4WETLEVEL", delay4WetLevelSlider);
    delay4DelayTimeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY4DELAYTIME", delay4DelayTimeSlider);
    delay4FeedbackValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY4FEEDBACK", delay4FeedbackSlider);

    cutoffValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CUTOFF", cutoffSlider);

    reverbLengthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REVERBLENGTH", reverbLengthSlider);
    
}

A3AudioProcessorEditor::~A3AudioProcessorEditor()
{
}

//==============================================================================
void A3AudioProcessorEditor::paint (juce::Graphics& g)
{
    getLookAndFeel().setColour (juce::Slider::thumbColourId, juce::Colours::greenyellow);
    juce::Rectangle<int> area = getLocalBounds().reduced(5);
    juce::Rectangle<int> titleArea = area.removeFromTop (20);
    //int width = area.getWidth();
    g.fillAll (juce::Colours::grey);
    g.setColour(juce::Colours::white);
    g.drawText("The number before each parameter signifies which reverb it's adjusting and all delay parameters adjust the fourth reverb", titleArea, juce::Justification::centredTop);
    
    juce::Rectangle <int> innerArea = getLocalBounds().reduced(25);//(25, 25, 350, 150);
    g.setColour(juce::Colours::yellowgreen);
    g.drawRect(innerArea, 1.0f);
    
    int innerWidth = innerArea.getWidth();
    g.drawText("1) Room Size", innerArea.getX(), 60, innerWidth / 4, 25, juce::Justification::centred);
    g.drawText("1) Wet Level", innerArea.getX() + innerWidth / 4, 60, innerWidth / 4, 25, juce::Justification::centred);
    g.drawText("1) Dry Level", innerArea.getX() + innerWidth / 2, 60, innerWidth / 4, 25, juce::Justification::centred);
    g.drawText("1) Width", innerArea.getX() + 3 * innerWidth / 4, 60, innerWidth / 4, 25, juce::Justification::centred);
    
    g.drawText("3) Reverb length (Adjusting this while audio is playing might create artifact, so stop the audio first)", 0, 240 , innerWidth , 25, juce::Justification::centred);

    g.drawText("Delay1 Wet Level", 20, 250 + 40, innerWidth / 6, 25, juce::Justification::centred);
    g.drawText("Delay1 Delay Time", 20 + innerWidth / 6, 250 + 40, innerWidth / 6, 25, juce::Justification::centred);
    g.drawText("Delay1 Feedback", 20 +  2*innerWidth / 6, 250 + 40, innerWidth / 6, 25, juce::Justification::centred);
    g.drawText("Delay2 Wet Level", 20 + 3 * innerWidth / 6, 250 + 40, innerWidth / 6, 25, juce::Justification::centred);
    g.drawText("Delay2 Delay Time", 20 + 4 * innerWidth / 6, 250 + 40, innerWidth / 6, 25, juce::Justification::centred);
    g.drawText("Delay2 Feedback", 20 + 5 * innerWidth / 6, 250 + 40, innerWidth / 6, 25, juce::Justification::centred);

    g.drawText("4) Highpass Filter: Cutoff", 20 + 15, 370 + 60,2* innerWidth / 6, 25, juce::Justification::centred);

    g.drawText("Delay3 Wet Level", 20, 420 + 60, innerWidth / 6, 25, juce::Justification::centred);
    g.drawText("Delay3 Delay Time", 20 + innerWidth / 6, 420 + 60, innerWidth / 6, 25, juce::Justification::centred);
    g.drawText("Delay3 Feedback", 20 + 2 * innerWidth / 6, 420 + 60, innerWidth / 6, 25, juce::Justification::centred);
    g.drawText("Delay4 Wet Level", 20 + 3 * innerWidth / 6, 420 + 60, innerWidth / 6, 25, juce::Justification::centred);
    g.drawText("Delay4 Delay Time", 20 + 4 * innerWidth / 6, 420 + 60, innerWidth / 6, 25, juce::Justification::centred);
    g.drawText("Delay4 Feedback", 20 + 5 * innerWidth / 6, 420 + 60, innerWidth / 6, 25, juce::Justification::centred);
    
}

void A3AudioProcessorEditor::resized()
{
    juce::Rectangle<int> area = getLocalBounds().reduced(40);
    juce::Rectangle<int> menus = area.removeFromTop(20);
    int width = menus.getWidth();
    reverbMenu.setBounds(35, 40, 355,20);
    impulseResponseMenu.setBounds(35 + 370, 40, 355, 20);
    //reverbMenu.setBounds(40, 300, 350, 20);
   
    //DBG("width:" << width / 4 - 10);
    //DBG("height:" << area.getHeight() - 10);
   
    roomSizeSlider.setBounds(35, 80, 170, 170);
    wetLevelSlider.setBounds(35 + 185, 80, 170, 170);
    dryLevelSlider.setBounds(35 + 2 * (185), 80, 170, 170);
    widthSlider.setBounds(35 + 3 * (185), 80, 170, 170);
    
    //constructedIRMenu.setBounds(35, 250, 355, 20);

    delay1WetLevelSlider.setBounds(35, 270 + 40, 113, 113);
    delay1DelayTimeSlider.setBounds(35 + 123, 270 + 40, 113, 113);
    delay1FeedbackSlider.setBounds(35 + 2 * (123), 270 + 40, 113, 113);
    delay2WetLevelSlider.setBounds(35 + 3 * (123), 270 + 40, 113, 113);
    delay2DelayTimeSlider.setBounds(35 + 4 * (123), 270 + 40, 113, 113);
    delay2FeedbackSlider.setBounds(35 + 5 * (123), 270 + 40, 113, 113);

    delay3WetLevelSlider.setBounds(35, 500, 113, 113);
    delay3DelayTimeSlider.setBounds(35 + 123, 500, 113, 113);
    delay3FeedbackSlider.setBounds(35 + 2 * (123), 500, 113, 113);
    delay4WetLevelSlider.setBounds(35 + 3 * (123), 500, 113, 113);
    delay4DelayTimeSlider.setBounds(35 + 4 * (123), 500, 113, 113);
    delay4FeedbackSlider.setBounds(35 + 5 * (123), 500, 113, 113);


    cutoffSlider.setBounds(75 , 390 + 60, 113*6-40, 25);
   
    reverbLengthSlider.setBounds(75 , 260 , 113*6-40, 25);
}

void A3AudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox)
{

    juce::File file;
    
    if (comboBox->getSelectedId() == 1 &&comboBox == &impulseResponseMenu)
    {
        file = juce::File::getSpecialLocation(juce::File::userDesktopDirectory);
        //file = file.getChildFile("Desktop");
        file = file.getChildFile("Resources/SquareVictoriaDome.wav");

        
        audioProcessor.convolution.loadImpulseResponse(file, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0, juce::dsp::Convolution::Normalise::yes);
    }
    if (comboBox->getSelectedId() == 2 && comboBox == &impulseResponseMenu)
    {
        file = juce::File::getSpecialLocation(juce::File::userDesktopDirectory);
        file = file.getChildFile("Resources/TransitCenter.wav");
      
        //DBG("ok2");
        audioProcessor.convolution.loadImpulseResponse(file, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0, juce::dsp::Convolution::Normalise::yes);
    }
    if (comboBox->getSelectedId() == 3 && comboBox == &impulseResponseMenu)
    {
        file = juce::File::getSpecialLocation(juce::File::userDesktopDirectory);
        file = file.getChildFile("Resources/TunnelToHell.wav");
        audioProcessor.convolution.loadImpulseResponse(file, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0, juce::dsp::Convolution::Normalise::yes);
    }
    if (comboBox->getSelectedId() == 4 && comboBox == &impulseResponseMenu)
    {
        file = juce::File::getSpecialLocation(juce::File::userDesktopDirectory);
        file = file.getChildFile("Resources/RacquetballCourt.wav");
        audioProcessor.convolution.loadImpulseResponse(file, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0, juce::dsp::Convolution::Normalise::yes);
    }
    if (comboBox->getSelectedId() == 5 && comboBox == &impulseResponseMenu)
    {
        file = juce::File::getSpecialLocation(juce::File::userDesktopDirectory);
        file = file.getChildFile("Resources/WoodruffLane.wav");
        audioProcessor.convolution.loadImpulseResponse(file, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0, juce::dsp::Convolution::Normalise::yes);
    }
    if (comboBox->getSelectedId() == 6 && comboBox == &impulseResponseMenu)
    {
        juce::FileChooser fileChooser("Please select the impulse response you want to load",
            juce::File::getSpecialLocation(juce::File::userDesktopDirectory),
            "*.wav");

        if (fileChooser.browseForFileToOpen())
        {
            file = fileChooser.getResult();
        }
        audioProcessor.convolution.loadImpulseResponse(file, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0, juce::dsp::Convolution::Normalise::yes);
    }
    //-------------------------------------------------
    
    if (comboBox->getSelectedId() == 3  && comboBox == &reverbMenu)
    {
        double SR = 44100;
        int length = reverbLengthSlider.getValue() *  (float)SR;
        audioProcessor.constructedImpulseResponse.setSize(2, length);
        audioProcessor.constructedImpulseResponse.clear();

        auto* leftChannel = audioProcessor.constructedImpulseResponse.getWritePointer(0);
        auto* rightChannel = audioProcessor.constructedImpulseResponse.getWritePointer(1);
            
            for (int i = 0; i < length; i++)
            {
                auto ran = (rand() % 10000) * 0.0001f;
                float offset = (i * (-1.0 / (length))); // -1/6*SR
                leftChannel[i] = (1.0 + offset) * ran;
                rightChannel[i] = (1.0 + offset) * ran;
            }
            audioProcessor.constructedConvolution.loadImpulseResponse(std::move(audioProcessor.constructedImpulseResponse), SR, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, juce::dsp::Convolution::Normalise::yes);

      
    }
   
    
    //audioProcessor.selected = false;
}
void A3AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    double SR = 44100;
    int length = reverbLengthSlider.getValue() * (float)SR;
    audioProcessor.constructedImpulseResponse.setSize(2, length);
    audioProcessor.constructedImpulseResponse.clear();

    auto* leftChannel = audioProcessor.constructedImpulseResponse.getWritePointer(0);
    auto* rightChannel = audioProcessor.constructedImpulseResponse.getWritePointer(1);

    for (int i = 0; i < length; i++)
    {
        auto ran = (rand() % 10000) * 0.0001f;
        float offset = (i * (-1.0 / (length))); // -1/6*SR
        leftChannel[i] = (1.0 + offset) * ran;
        rightChannel[i] = (1.0 + offset) * ran;
    }
    audioProcessor.constructedConvolution.loadImpulseResponse(std::move(audioProcessor.constructedImpulseResponse), SR, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, juce::dsp::Convolution::Normalise::yes);


}

