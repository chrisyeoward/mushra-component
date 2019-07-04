/*
  ==============================================================================

    MushraProcessor.h
    Created: 30 Jun 2019 11:32:45am
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class MushraProcessor
{
public:
	MushraProcessor(int stimulusCount, int numberOfPermutations) :
	stimulusCount(stimulusCount),
	numberOfPermutations(numberOfPermutations) {};
	
	~MushraProcessor() {};
	
	void setActiveStimulus(int s) {
		selectedStimulus = s;
		isPlaying = true;
	}
	
	bool getIsPlaying() {
		return isPlaying;
	}
	
	void setIsPlaying(bool value) {
		isPlaying = value;
	}
	
	int getNumberOfPermutations() {
		return numberOfPermutations;
	}
	
	int getCurrentPermutation() {
		return permutation;
	}
	
protected:
	int stimulusCount;
	int selectedStimulus = 0;
	bool isPlaying = false;
	int numberOfPermutations;
	int permutation = 0;

};
