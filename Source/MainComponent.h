#pragma once

#include <JuceHeader.h>
#include "MixingChannel.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public juce::Timer, public juce::Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;
    //juce::Timer ApplicationTimer;

    juce::AudioDeviceManager audioDeviceManager;
    std::unique_ptr<juce::ComboBox> audioDeviceSelector;

    juce::AudioBuffer<float> audioBuffer;

    juce::AudioBuffer<float>& getAudioBuffer()
    {
        return audioBuffer;
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;


    void CableMuteButton_Open();
    void CableMuteButton_Close();

    void buttonClicked(juce::Button* button) override;


private:
    //==============================================================================
    // Your private member variables go here...

    MixingChannel mixingChannel;
    std::unique_ptr<juce::TextButton> CableMuteButton;
    std::unique_ptr<juce::AudioDeviceSelectorComponent> audioSettings;
    AudioVisualizer AudioVisualizer_;


    enum class MuteState
    {
        open,
        close
    };


    MuteState mute_state{ MuteState::open };




    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


