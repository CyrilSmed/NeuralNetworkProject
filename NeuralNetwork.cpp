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
				// todo Выбрать метод инициализации весов:
				//neuralLayers[layerIndex][neuronIndex].weights[weightIndex] = (long double)rand() / (RAND_MAX); // от 0 до 1
				neuralLayers[layerIndex][neuronIndex].weights[weightIndex] = ((long double)rand() / (RAND_MAX))*2 - 1; // от -1 до 1
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
			float calculatedValue = 0;
			for (int connectionIndex = 0; connectionIndex < neuralLayers[layerIndex][neuronIndex].weights.size(); connectionIndex++) //Веса
			{
				float weight = neuralLayers[layerIndex][neuronIndex].weights[connectionIndex];

				float value = 0;
				if (layerIndex == 0) value = inputLayer[connectionIndex];
				else value = neuralLayers[layerIndex - 1][connectionIndex].value;

				calculatedValue += weight * value; // Считаем взвешенную сумму
			}
			calculatedValue += neuralLayers[layerIndex][neuronIndex].bias; // Добавляем сдвиг

			neuralLayers[layerIndex][neuronIndex].value = 1 / (1 + exp(-calculatedValue)); // Применяем сигмоидную функцию

			//cout << neuralLayers[layerIndex][neuronIndex].value << endl;
		}
	}

}

// Test Functions
void NeuralNetwork::testSetInputColor(float R, float G, float B)
{
	inputLayer[0] = R / 255;
	inputLayer[1] = G / 255;
	inputLayer[2] = B / 255;
}
void NeuralNetwork::testPrintValues()
{
	for (int neuronIndex = 0; neuronIndex < inputLayer.size(); neuronIndex++) // Входные нейроны
	{
		printf("%d) %0.2f ", neuronIndex + 1, inputLayer[neuronIndex]);
	}

	printf("\n");

	for (int layerIndex = 0; layerIndex < neuralLayers.size(); layerIndex++) // Слои
	{
		for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++) // Нейроны
		{
			//cout << neuronIndex << ") " << neuralLayers[layerIndex][neuronIndex].value << " ";
			printf("%d) %0.2f ", neuronIndex+1, neuralLayers[layerIndex][neuronIndex].value);
		}
		printf("\n");
	}
}