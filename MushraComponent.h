/*
  ==============================================================================

    MushraComponent.h
    Created: 29 Jun 2019 5:31:27pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MushraProcessor.h"
#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

//==============================================================================
/*
*/
class MushraComponent    : public Component, public Button::Listener, public Slider::Listener
{
public:
    MushraComponent(MushraProcessor &p, int count, String fileDirectory);
    ~MushraComponent();

    void paint (Graphics&) override;
    void resized() override;
	
	void buttonClicked(Button* button) override;
	void sliderValueChanged (Slider*) override;
	
	class Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener()  {}
		
		/** Called when the button is clicked. */
		virtual void mushraFormSubmitted (MushraComponent*) = 0;
		
		/** Called when the button's state changes. */
	};
	
	/** Registers a listener to receive events when this form is submitted.
	 If the listener is already registered, this will not register it again.
	 @see removeListener
	 */
	void addListener (Listener* newListener);
	
	/** Removes a previously-registered button listener
	 @see addListener
	 */
	void removeListener (Listener* listener);

	void resetForm();
	
	void setInfoText(String text);
	
private:
	
	MushraProcessor& processor;
	
	ListenerList<Listener> submitButtonListeners;
	
	const int stimulusCount;
	
	Label infoTextLabel;
	
	TextButton referenceButton;
	TextButton* stimulusButtons;
	Slider* stimulusRatingSliders;
	
	Label ExcellentLabel;
	Label GoodLabel;
	Label FairLabel;
	Label PoorLabel;
	Label BadLabel;
	
	Label statusLabel;
	
	
	std::vector<int> indexToStimulusMapping;
	
	TextButton submitButton;
	TextButton playPauseButton;
	
	void writeValuesToFile();
	
	String fileDirectory;
	File file;
	
	Button* activeButton;
	
	void shuffleMapping();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MushraComponent)
};