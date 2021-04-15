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
	int m_outputLayerIndex = 0;
	const float learningrate = 0.2;

	void weightsUpdate();
	void backPropagation();

public:
	void calculateErrors(const vector<float> expected); // todo make private
 	vector<float> inputLayer;
	vector<vector<Neuron>> neuralLayers;

	NeuralNetwork(int numHiddenLayers, int numInputNeurons, int numHiddenNeurons, int numOutputNeurons);
	NeuralNetwork(string initializeFileName);

	void weightInitialization();

	void forwardPropagation();

	void saveNetwork(string saveFileName);
	void fileInitialize(string initializeFileName);

	void testSetInput(float inputOne, float inputTwo);
	void testSetInputColor(float R, float G, float B);
	void testPrintValues();
	void testPrintResult();
};



