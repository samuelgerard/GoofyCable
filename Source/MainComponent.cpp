#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    startTimer(25);
    audioDeviceManager.initialise(1, 0, nullptr, true);
    //auto availableDevices = audioDeviceManager.getAvailableDeviceTypes();

    auto* currentDevice = audioDeviceManager.getCurrentAudioDevice();
    audioDeviceSelector = std::make_unique<juce::ComboBox>("Audio Input");
    audioDeviceSelector->setBounds(10, 10, 200, 30);
    addAndMakeVisible(audioDeviceSelector.get());

    juce::String CurrentDeviceName;

    if (currentDevice)
    {
         CurrentDeviceName = currentDevice->getName();
    }


    audioDeviceSelector->addItem(CurrentDeviceName, audioDeviceSelector->getNumItems() + 1);
    audioDeviceSelector->setSelectedItemIndex(0, juce::NotificationType::dontSendNotification);


    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (1, 6);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{

    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)




    auto* inBuffer = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

    if (inBuffer == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    auto* outBufferLeft = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* outBufferRight = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);



    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        outBufferLeft[i] = inBuffer[i];
        outBufferRight[i] = inBuffer[i];
    }

    audioBuffer.makeCopyOf(*bufferToFill.buffer);

}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));


    g.setColour(juce::Colours::white);

    const float* data = audioBuffer.getReadPointer(0);

    if (data)
    {
        int numSamples = audioBuffer.getNumSamples();

        for (int i = 1; i < numSamples; ++i)
        {
            int x1 = getWidth() * (i - 1) / numSamples;
            int x2 = getWidth() * i / numSamples;
            int y1 = getHeight() / 2 * (1.0f - data[i - 1]);
            int y2 = getHeight() / 2 * (1.0f - data[i]);

            g.drawLine(x1, y1, x2, y2, 2.0f);
        }
    }

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::timerCallback() 
{
    repaint();
}