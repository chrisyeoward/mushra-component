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
		
		/** Called when the submit button is clicked. */
		virtual void mushraFormSubmitted (MushraComponent*) = 0;
		
		/** Called when all permutations have been completed. */
		virtual void mushraFormCompleted (MushraComponent*) = 0;
		
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
	
	void setScaleLabels(String* labels);
	
	void setFilePrefix(String prefix);
	
	void setSceneDescription(String);
	
	void writeValuesToFile();
	
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
	
	Label sceneDescriptionLabel;
	
	std::vector<int> indexToStimulusMapping;
	
	TextButton submitButton;
	TextButton playPauseButton;
	
	String fileDirectory;
	File file;
	String filePrefix;
	
	Button* activeButton;
	
	void shuffleMapping();
	
	TextButton saveAndExitButton;

	
	int test = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MushraComponent)
};
