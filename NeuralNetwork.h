#pragma once
#include <vector>

using namespace std;

struct Neuron
{
	float value = 0;
	float bias = 0;
	float error = 0;
	vector<float> weights;
};

class NeuralNetwork
{
private:
	static const string m_colorLabels[13];
	int m_outputLayerIndex = 0;
	float m_learningRate = 0.2;
 	vector<float> inputLayer;
	vector<vector<Neuron>> neuralLayers;

	void initializeWeightsRandomly();

	void forwardPropagate();
	float calculateErrorSum(const vector<float> expected);
	void backwardPropagateErrors(const vector<float> expected);
	void updateWeights();

	float runTestingDataSet(const string testingDataSetFileName, const char delimChar = ',');

	void setInputs(const vector<float> inputs);
	int getResultIndex();
	void printStats();

public:

	NeuralNetwork(int numHiddenLayers, int numInputNeurons, int numHiddenNeurons, int numOutputNeurons);
	NeuralNetwork(string initFileName, char delimChar = ',');

	void trainNetwork(const string trainingDataSetFileName, const string testingDataSetFileName, 
		const int epochNum, const float learningRate, const char delimChar = ',');

	void saveNetwork(const string saveFileName, const char delimChar = ',');

	bool testInterface();
};

