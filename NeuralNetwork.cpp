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
	//todo
}