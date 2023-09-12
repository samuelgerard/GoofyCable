#pragma once


#include <JuceHeader.h>


class AudioVisualizer : public juce::Component
{
public:
    AudioVisualizer()
    {
    
    }

    ~AudioVisualizer()
    {
    }

    AudioVisualizer(juce::String channelLabelTitle)
    {

    }

    void resized() override {

    }


    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::white);
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));


        //g.setColour(juce::Colours::white);
        //juce::Rectangle<int> RectangleShape_expanded = getLocalBounds().expanded(10);

        g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 3.0f);


        const float* data = Visualizer_Redraw.getReadPointer(0);

        if (data)
        {
            int numSamples = Visualizer_Redraw.getNumSamples();

            for (int i = 1; i < numSamples; ++i)
            {
                int x1 = getWidth() * (i - 1) / numSamples;
                int x2 = getWidth() * i / numSamples;
                int y1 = getHeight() / 2 * (1.0f - data[i - 1]);
                int y2 = getHeight() / 2 * (1.0f - data[i]);

                g.drawLine(x1, y1 + 2, x2, y2, 1.5f);
                //g.drawVerticalLine(x1, 2.0f, 3.0f);
            }
        }
    }

    //void Visualizer_timerCallback(juce::AudioBuffer<float>& audioBuffer);

    void SetAndDraw_AudioVisualizer(juce::AudioBuffer<float>& audioBuffer)
    {
        Visualizer_Redraw.makeCopyOf(audioBuffer);
        repaint();
    }

private:
    juce::AudioBuffer<float> Visualizer_Redraw;
};