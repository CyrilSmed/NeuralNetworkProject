#pragma once
#include <vector>

using namespace std;

struct Neuron
{
	float value = 0;
	float bias = 0;
	vector<float> weights;
};

class NeuralNetwork
{
private:
	int m_outputLayerIndex;
public:
	vector<float> inputLayer;
	vector<vector<Neuron>> neuralLayers;

	//neuralLayers[m_outputLayerIndex]

	NeuralNetwork(int numHiddenLayers, int numInputNeurons, int numHiddenNeurons, int numOutputNeurons);

	void weightInitialization();

	void calculateNeuronValues();
};



