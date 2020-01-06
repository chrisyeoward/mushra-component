/*
  ==============================================================================

    MushraComponent.cpp
    Created: 29 Jun 2019 5:31:27pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MushraComponent.h"

//==============================================================================
MushraComponent::MushraComponent(MushraProcessor &p, int count, String destFileDirectory) :
	processor(p),
infoTextLabel("", ""),
	stimulusCount(count),
ExcellentLabel("", "Excellent"),
GoodLabel("", "Good"),
FairLabel("", "Fair"),
PoorLabel("", "Poor"),
BadLabel("", "Bad"),
statusLabel("", "Stage 1"),
sceneDescriptionLabel("", ""),
fileDirectory(destFileDirectory)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	
	
	getLookAndFeel().setColour(TextButton::ColourIds::buttonOnColourId, Colours::white);
	getLookAndFeel().setColour(TextButton::ColourIds::textColourOnId, Colours::black);
	
	addAndMakeVisible(infoTextLabel);
	infoTextLabel.setJustificationType(Justification::centred);
	
	
	addAndMakeVisible(ExcellentLabel);
	addAndMakeVisible(GoodLabel);
	addAndMakeVisible(FairLabel);
	addAndMakeVisible(PoorLabel);
	addAndMakeVisible(BadLabel);
	
	ExcellentLabel.setJustificationType(Justification::centred);
	GoodLabel.setJustificationType(Justification::centred);
	FairLabel.setJustificationType(Justification::centred);
	PoorLabel.setJustificationType(Justification::centred);
	BadLabel.setJustificationType(Justification::centred);
	
//	addAndMakeVisible (referenceButton);
	referenceButton.setRadioGroupId(1);
	referenceButton.setButtonText ("R");
	referenceButton.addListener(this);
	
	stimulusButtons = new TextButton[stimulusCount];
	stimulusRatingSliders = new Slider[stimulusCount];
	
	for(int stimulusIndex = 0; stimulusIndex < stimulusCount; stimulusIndex++)
	{
		addAndMakeVisible(stimulusButtons[stimulusIndex]);
		stimulusButtons[stimulusIndex].setButtonText(String(stimulusIndex + 1));
		stimulusButtons[stimulusIndex].addListener(this);
		stimulusButtons[stimulusIndex].setRadioGroupId(1);
		
		addAndMakeVisible(stimulusRatingSliders[stimulusIndex]);
		stimulusRatingSliders[stimulusIndex].addListener(this);
		stimulusRatingSliders[stimulusIndex].setRange (0.0, 100.0, 0.1);
		stimulusRatingSliders[stimulusIndex].setSliderStyle(Slider::LinearVertical);
		stimulusRatingSliders[stimulusIndex].hideTextBox(true);
		
		indexToStimulusMapping.push_back(stimulusIndex);
	}
	
	resetForm();
	processor.resetEvaluation();

	
	addAndMakeVisible(playPauseButton);
	playPauseButton.setButtonText("Play");
	playPauseButton.addListener(this);
	
	addAndMakeVisible(submitButton);
	submitButton.setButtonText("Next");
	submitButton.addListener(this);
	
	addAndMakeVisible(statusLabel);
	statusLabel.setJustificationType(Justification::centred);
	
	addAndMakeVisible(sceneDescriptionLabel);
	sceneDescriptionLabel.setJustificationType(Justification::centred);
	
	
	addAndMakeVisible(saveAndExitButton);
	saveAndExitButton.addListener(this);
//	saveAndExitButton.setButtonText("Save and exit");
}

MushraComponent::~MushraComponent()
{
	delete[] stimulusButtons;
	delete[] stimulusRatingSliders;
}

void MushraComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);

	
	
	g.setColour (Colours::white);
	g.setFont (14.0f);
	
	float buttonSpacing = 50;
	float sectionWidth = (getWidth() / (stimulusCount + 2));
	float sectionHeight = getHeight()/7;
	
	float buttonWidth = sectionWidth - buttonSpacing;
	float buttonHeight = sectionHeight/2;
	float sliderHeight = (5 * sectionHeight) + 20;
	
	infoTextLabel.setBounds(0, 0, getWidth(), sectionHeight);
	
	
	g.setColour (Colours::grey);
	float lineWidth = sectionWidth * (stimulusCount + 1) - buttonWidth;
	for(int line = 0; line < 4; line++) {
		g.drawLine(0, (line + 3) * sectionHeight, lineWidth, (line + 3) * sectionHeight);
	}
	
	referenceButton.setBounds(0, buttonHeight/2, buttonWidth, buttonHeight);
	referenceButton.setToggleState(false, NotificationType::dontSendNotification);
	for(int stimulusIndex = 0; stimulusIndex < stimulusCount; stimulusIndex++)
	{
		float x = (buttonWidth + buttonSpacing) * (stimulusIndex + 1);
		stimulusButtons[stimulusIndex].setBounds(x, buttonHeight/2 + sectionHeight, buttonWidth, buttonHeight);
		
		stimulusRatingSliders[stimulusIndex].setBounds(x, 2*sectionHeight - 10, buttonWidth, sliderHeight);
		stimulusRatingSliders[stimulusIndex].setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
		
		stimulusButtons[stimulusIndex].setToggleState(false, NotificationType::dontSendNotification);
	}
	
	ExcellentLabel.setBounds(0, 2*sectionHeight, buttonWidth, sectionHeight);
	GoodLabel.setBounds(0, 3*sectionHeight, buttonWidth, sectionHeight);
	FairLabel.setBounds(0, 4*sectionHeight, buttonWidth, sectionHeight);
	PoorLabel.setBounds(0, 5*sectionHeight, buttonWidth, sectionHeight);
	BadLabel.setBounds(0, 6*sectionHeight, buttonWidth, sectionHeight);
	
	submitButton.setBounds((stimulusCount + 1) * sectionWidth, getHeight()/2, sectionWidth, buttonHeight);
	
	String buttonText = processor.getIsPlaying() ? "Pause" : "Play";
	playPauseButton.setButtonText(buttonText);
	playPauseButton.setBounds((stimulusCount + 1) * sectionWidth, (getHeight()/2) - sectionHeight, sectionWidth, buttonHeight);
	playPauseButton.setToggleState(!processor.getIsPlaying(), NotificationType::dontSendNotification);
	
	String status = "Scene " + String(processor.getCurrentStage() + 1) + " of " + String(processor.getNumberOfPermutations());
	statusLabel.setText(status, NotificationType::dontSendNotification);
	statusLabel.setBounds((stimulusCount + 1) * sectionWidth, (getHeight()/2) + sectionHeight, sectionWidth, buttonHeight);
	
	sceneDescriptionLabel.setBounds((stimulusCount + 1) * sectionWidth, (getHeight()/2) + 2 * sectionHeight, sectionWidth, buttonHeight);
	
	if(activeButton != NULL)
	{
		activeButton->setToggleState(true, NotificationType::dontSendNotification);
	}
	
	saveAndExitButton.setBounds(getWidth() - buttonHeight, getHeight() - buttonHeight, buttonHeight, buttonHeight);
	
}

void MushraComponent::setScaleLabels(String* labels){
	ExcellentLabel.setText(labels[4], dontSendNotification);
	GoodLabel.setText(labels[3], dontSendNotification);
	FairLabel.setText(labels[2], dontSendNotification);
	PoorLabel.setText(labels[1], dontSendNotification);
	BadLabel.setText(labels[0], dontSendNotification);
}

void MushraComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void MushraComponent::setInfoText(String text) {
	infoTextLabel.setText(text, NotificationType::dontSendNotification);
}

void MushraComponent::buttonClicked (Button* button)
{
	if(button == &submitButton) {
		int scores[stimulusCount];
		
		for(int stimulusIndex = 0; stimulusIndex<stimulusCount; stimulusIndex++)
		{
			scores[indexToStimulusMapping.at(stimulusIndex)] = stimulusRatingSliders[stimulusIndex].getValue();
		}
		
		processor.setScoresForScene(scores, processor.getCurrentPermutation());
		
		Component::BailOutChecker checker (this);
		
		if(processor.isFinished()) {
			writeValuesToFile();
			submitButtonListeners.callChecked (checker, [this] (Listener& l) { l.mushraFormCompleted (this); });
			processor.resetEvaluation();
			resetForm();
		} else {
			resetForm();
			processor.goToNextStage();
			submitButtonListeners.callChecked (checker, [this] (Listener& l) { l.mushraFormSubmitted (this); });
		}
	} else if (button == &playPauseButton) {
		if(processor.getIsPlaying()) {
			activeButton = nullptr;
			processor.setIsPlaying(false);
		} else {
			processor.setActiveStimulus(indexToStimulusMapping[0]);
			activeButton = &stimulusButtons[0];
		}
		
	} else if (button == &referenceButton)
	{
		processor.setActiveStimulus(0);
		activeButton = button;
	} else if (button == &saveAndExitButton){
		int scores[stimulusCount];
		
		for(int stimulusIndex = 0; stimulusIndex<stimulusCount; stimulusIndex++)
		{
			scores[indexToStimulusMapping.at(stimulusIndex)] = stimulusRatingSliders[stimulusIndex].getValue();
		}
		
		processor.setScoresForScene(scores, processor.getCurrentPermutation());
		
		Component::BailOutChecker checker (this);
		
		writeValuesToFile();
		submitButtonListeners.callChecked (checker, [this] (Listener& l) { l.mushraFormCompleted (this); });
		processor.resetEvaluation();
		resetForm();
		
	} else {
		for(int buttonIndex = 0; buttonIndex < stimulusCount; buttonIndex++)
		{
			if(button == &(stimulusButtons[buttonIndex])) {
				activeButton = button;
				processor.setActiveStimulus(indexToStimulusMapping[buttonIndex]);
			}
		}
	}
}

void MushraComponent::sliderValueChanged(Slider* slider)
{
//	DBG(slider->getValue());
	
}

void MushraComponent::writeValuesToFile()
{
	std::cout << "write to file!";
	
	String filename = filePrefix != "" ? filePrefix : "evaluation";
	filename += "_" + Time::getCurrentTime().toISO8601(true);
	filename += ".csv";
	String filepath = fileDirectory + filename;
	File file(filepath);
	
	TemporaryFile tempFile (file);
	
	FileOutputStream output (tempFile.getFile());
	
	if (! output.openedOk())
	{
		DBG ("FileOutputStream didn't open correctly ...");
		// ... some other error handling
	}
	
	int outputs[stimulusCount];
	
	for(int stimulusIndex = 0; stimulusIndex<stimulusCount; stimulusIndex++)
	{
		outputs[indexToStimulusMapping.at(stimulusIndex)] = stimulusRatingSliders[stimulusIndex].getValue();
	}
	
	String line1 = "scene\\method," + String(0);
	for(int stimulusIndex = 1; stimulusIndex < stimulusCount; stimulusIndex++)
	{
		line1 += ",";
		line1 += String(stimulusIndex);
	}
	line1 += "\n";
	
	output.setNewLineString("\n");
	
	output.writeString(line1);
	
	for(int scene = 0; scene < processor.getNumberOfPermutations(); scene++)
	{
		String line = String(scene);
		
		for(int method = 0; method < stimulusCount; method ++) {
			line += ",";
			line += String(processor.getScoreFor(scene, method));
		}
		
		line += "\n";
		output.writeString(line);
	}
	
//	output.writeString(line2);
	
	output.flush(); // (called explicitly to force an fsync on posix)
	
	if (output.getStatus().failed())
	{
		DBG ("An error occurred in the FileOutputStream");
		// ... some other error handling
	}
	
	tempFile.overwriteTargetFileWithTemporary();
}

void MushraComponent::addListener (Listener* l)      { submitButtonListeners.add (l); }
void MushraComponent::removeListener (Listener* l)   { submitButtonListeners.remove (l); }

void MushraComponent::resetForm() {
	activeButton = nullptr;
//	processor.setActiveStimulus(-1);
	shuffleMapping();
	for(int stimulusIndex = 0; stimulusIndex < stimulusCount; stimulusIndex++)
	{
		stimulusRatingSliders[stimulusIndex].setValue(0.0);
	}
}

void MushraComponent::setFilePrefix(String prefix)
{
	filePrefix = prefix;
}

void MushraComponent::setSceneDescription(String description)
{
	sceneDescriptionLabel.setText(description, dontSendNotification);
}

void MushraComponent::shuffleMapping() {
	unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
	
	shuffle (indexToStimulusMapping.begin(), indexToStimulusMapping.end(), std::default_random_engine(seed));
	
	std::cout << "shuffled elements:";
	for (int& x: indexToStimulusMapping) std::cout << ' ' << x;
	std::cout << '\n';
}
