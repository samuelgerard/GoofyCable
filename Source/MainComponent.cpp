#include "MainComponent.h"



//==============================================================================
MainComponent::MainComponent() : mixingChannel("Channel 1"),  AudioVisualizer_()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(800, 600);

    startTimer(25);


    //addAndMakeVisible(SoftwareTitle);


    addAndMakeVisible(mixingChannel);
    addAndMakeVisible(AudioVisualizer_);


    mixingChannel.SetChannelVolume_parameters();

    addAndMakeVisible(mixingChannel.getChannelVolumeMeter());
    //    addAndMakeVisible(mixingChannel_2);
    //    addAndMakeVisible(mixingChannel_3);
    //    addAndMakeVisible(mixingChannel_4);
    //    addAndMakeVisible(mixingChannel_5);






        //mixingChannel_2.setBounds(180, 80, 100, 400);
        //mixingChannel_3.setBounds(330, 80, 100, 400);
        //mixingChannel_4.setBounds(480, 80, 100, 400);
        //mixingChannel_5.setBounds(630, 80, 100, 400);







        //====== Setting up Audio Input Selector ========
        // TODO: 

    //AudioIO_Selector_input_label = std::make_unique<juce::Label>();
    //AudioIO_Selector_input_label->setText("Select your Input Device:", juce::NotificationType::dontSendNotification);

    //addAndMakeVisible(AudioIO_Selector_input_label.get());



    //AudioIO_Selector_output_label = std::make_unique<juce::Label>();
    //AudioIO_Selector_output_label->setText("Select your Output Device:", juce::NotificationType::dontSendNotification);

    //addAndMakeVisible(AudioIO_Selector_output_label.get());

    //audioDeviceSelector_input = std::make_unique<juce::ComboBox>("Audio Input");

    //addAndMakeVisible(audioDeviceSelector_input.get());

    //audioDeviceSelector_output = std::make_unique<juce::ComboBox>("Audio Output");

    //addAndMakeVisible(audioDeviceSelector_output.get());





    //======= Setting up Audio Channels and Device selection ========



    audioDeviceManager.initialise(2, 2, nullptr, true);
    auto& availableDevices = audioDeviceManager.getAvailableDeviceTypes();
    auto* currentDevice = audioDeviceManager.getCurrentAudioDevice();
    audioSettings.reset(new juce::AudioDeviceSelectorComponent(audioDeviceManager, 0, 2, 0, 2, true, true, true, true));
    addAndMakeVisible(audioSettings.get());


    juce::String CurrentDeviceName;

    if (currentDevice)
    {
        CurrentDeviceName = currentDevice->getName();
    }

    juce::StringArray deviceNames;

    for (auto* deviceType : availableDevices)
    {
        juce::StringArray typeDevices = deviceType->getDeviceNames();
        deviceNames.addArray(typeDevices);
        break;
    }



    //audioDeviceSelector_input->addItemList(deviceNames, 1);
    //audioDeviceSelector_output->addItemList(deviceNames, 1);

    //audioDeviceSelector_input->setSelectedItemIndex(0, juce::NotificationType::dontSendNotification);
    //audioDeviceSelector_output->setSelectedItemIndex(1, juce::NotificationType::dontSendNotification);

    //audioDeviceManager.getAudioDeviceSetup(CurrentAudioSetup);

    //CurrentAudioSetup.inputDeviceName = currentDevice->getName();
    //CurrentAudioSetup.inputDeviceName = audioDeviceSelector_output->getText();

    //juce::String error = audioDeviceManager.setAudioDeviceSetup(CurrentAudioSetup, true);

    //if (!error.isEmpty())
    //{
    //    CurrentAudioSetup.outputDeviceName = currentDevice->getName();
    //    audioDeviceManager.setAudioDeviceSetup(CurrentAudioSetup, true);
    //}

    mixingChannel.setBounds(250, 80, 300, 400);
    mixingChannel.SetChannelVolume_bounds();
    AudioVisualizer_.setBounds(265, 155, 190, 100);
    //AudioIO_Selector_input_label->setBounds(10, 10, 200, 30);
    //AudioIO_Selector_output_label->setBounds(10, 525, 200, 30);
    //audioDeviceSelector_input->setBounds(175, 10, 200, 30);
    ///audioDeviceSelector_output->setBounds(185, 525, 200, 30);
    audioSettings->setBounds(10, 10, 200, 30);


    CableMuteButton.reset(new juce::TextButton());
    CableMuteButton->addListener(this);
    CableMuteButton->setBounds(395, 490, 155, 30);
    CableMuteButton->setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);
    CableMuteButton->setColour(juce::TextButton::buttonOnColourId, juce::Colours::aquamarine);
    CableMuteButton->setColour(juce::TextButton::textColourOffId, juce::Colours::mediumaquamarine);
    CableMuteButton->setColour(juce::TextButton::textColourOnId, juce::Colours::darkslategrey);
    CableMuteButton->setButtonText("Mute Channel");
    
    addAndMakeVisible(CableMuteButton.get());


    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 0); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 6);
    }

}

MainComponent::~MainComponent()
{

    // This shuts down the audio device and clears the audio source.
    shutdownAudio();

}

void MainComponent::buttonClicked(juce::Button* button)
{
    if (button == CableMuteButton.get())
    {
        if (mute_state == MuteState::open)
        {
            CableMuteButton->onClick = [this]() { CableMuteButton_Close(); };
        }
        else if (mute_state == MuteState::close)
        {
            CableMuteButton->onClick = [this]() { CableMuteButton_Open(); };
        }
    }
}


void MainComponent::CableMuteButton_Open()
{

    CableMuteButton->setToggleState(false, juce::NotificationType::dontSendNotification);
    mute_state = MuteState::open;
}

void MainComponent::CableMuteButton_Close()
{
    CableMuteButton->setToggleState(true, juce::NotificationType::dontSendNotification);
    mute_state = MuteState::close;
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
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




    if (mute_state == MuteState::open)
    {

        for (int i = 0; i < bufferToFill.numSamples; ++i)
        {
            outBufferLeft[i] = inBuffer[i];
            outBufferRight[i] = inBuffer[i];
        }
    }
    else
    {
        juce::FloatVectorOperations::clear(outBufferLeft, bufferToFill.numSamples);
        juce::FloatVectorOperations::clear(outBufferRight, bufferToFill.numSamples);
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
// void MainComponent::paint (juce::Graphics& g)
// {
//     // (Our component is opaque, so we must completely fill the background with a solid colour)
//     g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));


//     g.setColour(juce::Colours::white);

//     const float* data = audioBuffer.getReadPointer(0);

//     if (data)
//     {
//         int numSamples = audioBuffer.getNumSamples();

//         for (int i = 1; i < numSamples; ++i)
//         {
//             int x1 = getWidth() * (i - 1) / numSamples;
//             int x2 = getWidth() * i / numSamples;
//             int y1 = getHeight() / 2 * (1.0f - data[i - 1]);
//             int y2 = getHeight() / 2 * (1.0f - data[i]);

//             g.drawLine(x1, y1, x2, y2, 2.0f);
//         }
//     }


// }


void MainComponent::paint(juce::Graphics& g)
{
    int padding = 50;
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(20.0f, juce::Font::bold));
    g.drawFittedText("Goofy Cable!", getWidth() - 200 - padding, 15, 200, 30, juce::Justification::topRight, 1);
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    //mixingChannel.setBounds(10, 10, getWidth() + 20, 30);        //channelLabel->setBounds();


}



void MainComponent::timerCallback()
{

    //mixingChannel.Visualizer_timerCallback(getAudioBuffer());
    AudioVisualizer_.SetAndDraw_AudioVisualizer(getAudioBuffer());

}

