#pragma once

#include <JuceHeader.h>
#include "AudioVisualizer.h"

class MixingChannel : public juce::Component
{
public:


    MixingChannel() 
    {

        //channelLabel.setText("Channel X", juce::dontSendNotification);
        //channelLabel.setFont(juce::Font(16.0f, juce::Font::bold));

        channel_Volume = std::make_unique<juce::Slider>();
        PluginChain_Box = std::make_unique<juce::ListBox>();

        channelLabel = std::make_unique<juce::Label>();
        channelLabel->setText("Channel X", juce::dontSendNotification);
        channelLabel->setFont(juce::Font(16.0f, juce::Font::bold));
        addAndMakeVisible(channelLabel.get());
        setSize(800, 600);


;    }

    ~MixingChannel()
    {
        audioBuffer_Redraw.clear();
    }

	MixingChannel(juce::String channelLabelTitle)
	{

        //channelLabel.setText(channelLabelTitle, juce::dontSendNotification);
        //channelLabel.setFont(juce::Font(16.0f, juce::Font::bold));
        //addAndMakeVisible(channelLabel);


        channel_Volume = std::make_unique<juce::Slider>();

        channelLabel = std::make_unique<juce::Label>();
        channelLabel->setText(channelLabelTitle, juce::dontSendNotification);
        channelLabel->setFont(juce::Font(16.0f, juce::Font::bold));
        addAndMakeVisible(channelLabel.get());

        AudioVisualizer self_AudioVisualizer;
        
	}

    void resized() override {
        channelLabel->setBounds(10, 10, getWidth() - 20, 30);
    }


	void paint(juce::Graphics& g) override
	{
        g.setColour(juce::Colours::white);
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));


        //g.setColour(juce::Colours::white);
        //juce::Rectangle<int> RectangleShape_expanded = getLocalBounds().expanded(10);

        g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 3.0f);


	}


    void SetChannelVolume_parameters()
    {

        channel_Volume->setSliderStyle(juce::Slider::LinearBarVertical);
        channel_Volume->setRange(0.0, 127.0, 1.0);
        channel_Volume-> setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
        channel_Volume->setPopupDisplayEnabled(true, false, this);
        channel_Volume->setTextValueSuffix(" Decibals (Volume)");
        channel_Volume->setValue(1.0);
    }

    void SetChannelVolume_bounds()
    {
        channel_Volume->setBounds(480,160, getWidth() - 260, getHeight() - 125);
    }

    juce::Slider& getChannelVolumeMeter()
    {
        return *channel_Volume;
    }
  

private:
	juce::AudioBuffer<float> audioBuffer_Redraw;
    std::unique_ptr<juce::Label> channelLabel;
    std::unique_ptr<juce::Slider> channel_Volume;
    std::unique_ptr<juce::ListBox> PluginChain_Box;
};