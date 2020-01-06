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
	MushraProcessor(const int stimulusCount, int numberOfScenes) :
	stimulusCount(stimulusCount),
	numberOfScenes(numberOfScenes) {

		scores.resize(numberOfScenes, std::vector<int>(stimulusCount, 0));
				
		for(int scene = 0; scene < numberOfScenes; scene++)
		{
			sceneOrder.push_back(scene);
		}

		shuffleSceneOrder();

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
		return numberOfScenes;
	}

	int getCurrentPermutation() {
		return currentScene;
	}

	int getCurrentStage() {
		return sceneCount;
	}

	void goToNextStage() {
		if(!isFinished()) {
			sceneCount++;

			currentScene = sceneOrder[sceneCount];

			changeScene(currentScene);
		}
		isPlaying = false;
	}

	bool isFinished() {
		return sceneCount >= numberOfScenes - 1;
	}

	void setScoresForScene(int* updatedScores, int scene) {

		for(int stimulus = 0; stimulus < stimulusCount; stimulus++ ) {
			scores[scene][stimulus] = updatedScores[stimulus];
		}
	}

	void resetEvaluation() {
		sceneCount = 0;
		for(int i = 0; i < numberOfScenes; i++){
			std::fill(scores[i].begin(), scores[i].end(), 0);
		}

		isPlaying = false;
		shuffleSceneOrder();
	}

	void shuffleSceneOrder() {
		unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();

		shuffle (sceneOrder.begin(), sceneOrder.end(), std::default_random_engine(seed));

		currentScene = sceneOrder[0];

		std::cout << "permutation order:";
		for (int& x: sceneOrder) std::cout << ' ' << x;
		std::cout << '\n';
	}

	int getScoreFor(int scene, int stimulus) {
		return scores[scene][stimulus];
	}

	void startEvaluation()
	{
		for(int i = 0; i < numberOfScenes; i++){
			std::fill(scores[i].begin(), scores[i].end(), 0);
		}

		currentScene = sceneOrder[0];
		changeScene(currentScene);
	}

protected:
	int stimulusCount;
	int selectedStimulus = 0;
	bool isPlaying = false;
	int numberOfScenes;
	int currentScene;

	int sceneCount = 0;


	std::vector<int> sceneOrder;

	virtual void changeScene(int) = 0;

	std::vector<std::vector<int>> scores;

};
