/*
  ==============================================================================

    MushraProcessor.h
    Created: 30 Jun 2019 11:32:45am
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

class MushraProcessor
{
public:
	MushraProcessor(const int stimulusCount, int numberOfPermutations) :
	stimulusCount(stimulusCount),
	numberOfPermutations(numberOfPermutations) {
		
		scores.resize(numberOfPermutations, std::vector<int>(stimulusCount, 0.0));
		
		for(int permutation = 0; permutation < numberOfPermutations; permutation++)
		{
			permutationOrder.push_back(permutation);
		}
		
		unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
		
		shuffle (permutationOrder.begin(), permutationOrder.end(), std::default_random_engine(seed));
		
		permutation = permutationOrder[0];
		
		std::cout << "permutation order:";
		for (int& x: permutationOrder) std::cout << ' ' << x;
		std::cout << '\n';

	};
	
	virtual ~MushraProcessor() {};
	
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
	
	int getCurrentStage() {
		return stageNumber;
	}
	
	void goToNextStage() {
		if(!isFinished()) {
			stageNumber++;
			
			permutation = permutationOrder[stageNumber];
			//		permutation++;
			//		permutation %= numberOfPermutations;
			
			DBG("permutation");
			DBG(permutation);
			changePermutation(permutation);
		}
		isPlaying = false;
	}
	
	bool isFinished() {
		return stageNumber >= numberOfPermutations - 1;
	}
	
	void setScoresForPermutation(int* updatedScores, int permutation) {
		
		for(int stimulus = 0; stimulus < stimulusCount; stimulus++ ) {
			scores[permutation][stimulus] = updatedScores[stimulus];
		}
	}
	
	void reset() {
		stageNumber = 0;
		scores.resize(numberOfPermutations, std::vector<int>(stimulusCount, 0.0));
		isPlaying = false;
	}
	
	
protected:
	int stimulusCount;
	int selectedStimulus = 0;
	bool isPlaying = false;
	int numberOfPermutations;
	int permutation;
	
	int stageNumber = 0;
	

	std::vector<int> permutationOrder;
	
	virtual void changePermutation(int newPermutationIndex) = 0;

	std::vector<std::vector<int>> scores;

};
