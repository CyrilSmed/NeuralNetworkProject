#include <iostream>
#include <vector>
#include <random>
#include "NeuralNetwork.h"

using namespace std;

//Network
NeuralNetwork::NeuralNetwork(int numHiddenLayers, int numInputNeurons, int numHiddenNeurons, int numOutputNeurons) 
{
	// InputLayer
	inputLayer.resize(numInputNeurons);
	
	// HiddenLayers
	neuralLayers.resize(numHiddenLayers + 1);

	for (int i = 0; i < neuralLayers.size() - 1; i++)
	{
		neuralLayers[i].resize(numHiddenNeurons);
	}

	// OutpuLayer
	m_outputLayerIndex = numHiddenLayers;

	neuralLayers[m_outputLayerIndex].resize(numOutputNeurons);

	// Weights 
	for (int layerIndex = 0; layerIndex < neuralLayers.size(); layerIndex++) // Слои
	{
		for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++) // Нейроны
		{
			if (layerIndex == 0)
				neuralLayers[layerIndex][neuronIndex].weights.resize(inputLayer.size());
			else
				neuralLayers[layerIndex][neuronIndex].weights.resize(neuralLayers[layerIndex - 1].size());
		}
	}
}

void NeuralNetwork::weightInitialization()
{
	for (int layerIndex = 0; layerIndex < neuralLayers.size(); layerIndex++) // Слои
	{
		for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++) // Нейроны
		{
			for (int weightIndex = 0; weightIndex < neuralLayers[layerIndex][neuronIndex].weights.size(); weightIndex++) //Веса
			{
				neuralLayers[layerIndex][neuronIndex].weights[weightIndex] = (long double)rand() / (RAND_MAX); //todo -1 до 1?
			}
		}
	}
}

void NeuralNetwork::forwardPropagation()
{
	for (int layerIndex = 0; layerIndex < neuralLayers.size(); layerIndex++) // Слои
	{
		for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++) // Нейроны
		{
			float weightedSum = 0;
			for (int connectionIndex = 0; connectionIndex < neuralLayers[layerIndex][neuronIndex].weights.size(); connectionIndex++) //Веса
			{
				float weight = neuralLayers[layerIndex][neuronIndex].weights[connectionIndex];

				float value = 0;
				if (layerIndex == 0) value = inputLayer[connectionIndex];
				else value = neuralLayers[layerIndex - 1][connectionIndex].value;

				weightedSum += weight * value;
			}
			weightedSum += neuralLayers[layerIndex][neuronIndex].bias;
			neuralLayers[layerIndex][neuronIndex].value = 1 / (1 + exp(-weightedSum));
			cout << neuralLayers[layerIndex][neuronIndex].value << endl;
		}
	}

}

void NeuralNetwork::testSetInput(int R, int G, int B)
{
	inputLayer[0] = R / 255;
	inputLayer[1] = G / 255;
	inputLayer[2] = B / 255;

}