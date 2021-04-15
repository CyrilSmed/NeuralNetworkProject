#include <iostream>
#include <vector>
#include <random>
#include <fstream> // File Management
#include <string>
#include <sstream> // File Management
#include <Windows.h> // for CopyFile
#include "NeuralNetwork.h"

using namespace std;

//Network
NeuralNetwork::NeuralNetwork(int numHiddenLayers, int numInputNeurons, int numHiddenNeurons, int numOutputNeurons) 
{
	srand(time(0));

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
NeuralNetwork::NeuralNetwork(string initializeFileName)
{
	srand(time(0));

	vector<int> stactureInit;

	// Create an input filestream
	ifstream fileRead(initializeFileName);

	// Make sure the file is open
	if (!fileRead.is_open())
	{
		throw runtime_error("Could not open file");
	}

	// Read the column names
	if (fileRead.good())
	{
		// Helper variables
		string line;
		string curVariable;

		// Extract structure data
		getline(fileRead, line);
		stringstream stringStream(line);
		while (getline(stringStream, curVariable, ';'))
		{
			if (!curVariable.empty())
			{
				stactureInit.push_back(stoi(&curVariable.back()));
			}
		}

		//Apply Network Stracture

		// InputLayer
		inputLayer.resize(stactureInit[1]);

		// HiddenLayers
		neuralLayers.resize(stactureInit[0] + 1);

		for (int i = 0; i < neuralLayers.size() - 1; i++)
		{
			neuralLayers[i].resize(stactureInit[2]);
		}

		// OutputLayer
		m_outputLayerIndex = stactureInit[0];

		neuralLayers[m_outputLayerIndex].resize(stactureInit[3]);

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

		//Initialize weights and biases
		for (int layerIndex = 0; layerIndex < neuralLayers.size(); layerIndex++) // Слои
		{
			for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++) // Нейроны
			{
				getline(fileRead, line);
				stringstream stringStream(line);

				for (int weightIndex = 0; weightIndex < neuralLayers[layerIndex][neuronIndex].weights.size(); weightIndex++) //Веса
				{
					getline(stringStream, curVariable, ';');
					float tmp = stof(curVariable);
					neuralLayers[layerIndex][neuronIndex].weights[weightIndex] = tmp; // todo
				}
				getline(stringStream, curVariable, ';');
				neuralLayers[layerIndex][neuronIndex].bias = stof(curVariable);
			}
		}
	}

	fileRead.close();
}

void NeuralNetwork::weightInitialization()
{
	for (int layerIndex = 0; layerIndex < neuralLayers.size(); layerIndex++) // Слои
	{
		for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++) // Нейроны
		{
			for (int weightIndex = 0; weightIndex < neuralLayers[layerIndex][neuronIndex].weights.size(); weightIndex++) //Веса
			{
				neuralLayers[layerIndex][neuronIndex].weights[weightIndex] = ((long double)rand() / (RAND_MAX))*2 - 1; // от -2 до 2
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
			//printf("f(x) = ");

			float calculatedValue = 0;
			for (int connectionIndex = 0; connectionIndex < neuralLayers[layerIndex][neuronIndex].weights.size(); connectionIndex++) //Веса
			{
				float weight = neuralLayers[layerIndex][neuronIndex].weights[connectionIndex];

				float value = 0;
				if (layerIndex == 0) value = inputLayer[connectionIndex];
				else value = neuralLayers[layerIndex - 1][connectionIndex].value;

				calculatedValue += weight * value; // Считаем взвешенную сумму
				//printf("%0.2f * %0.2f + ", weight, value);
			}
			calculatedValue += neuralLayers[layerIndex][neuronIndex].bias; // Добавляем сдвиг
			//printf("%0.2f = %0.2f", neuralLayers[layerIndex][neuronIndex].bias, calculatedValue);

			neuralLayers[layerIndex][neuronIndex].value = 1 / (1 + exp(-calculatedValue)); // Применяем сигмоидную функцию
			//printf("\nsigmoif(f(x)) = %0.2f \n", neuralLayers[layerIndex][neuronIndex].value = 1 / (1 + exp(-calculatedValue)));

			//cout << neuralLayers[layerIndex][neuronIndex].value << endl;
		}
	}

}
/*
float NeuralNetwork::costFunction(const vector<float> solution) // Подсчет коэффициента ошибки (cost)
{
	if (solution.size() == neuralLayers[m_outputLayerIndex].size())
	{
		float cost = 0;

		for (int outputNeuronIndex = 0; outputNeuronIndex < neuralLayers[m_outputLayerIndex].size(); outputNeuronIndex++) // Нейроны выходного слоя и решения
		{
			float curValue = neuralLayers[m_outputLayerIndex][outputNeuronIndex].value;
			cost += (curValue - solution[outputNeuronIndex]) * (curValue - solution[outputNeuronIndex]);
		}
		return cost;
	}
	else return 0;
}
*/

void NeuralNetwork::calculateErrors(const vector<float> expected)
{
	if (expected.size() == neuralLayers[m_outputLayerIndex].size())
	{
		for (int layerIndex = m_outputLayerIndex; layerIndex >= 0; layerIndex--)
		{
			if (layerIndex == m_outputLayerIndex)
			{
				for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++)
				{
					float curValue = neuralLayers[layerIndex][neuronIndex].value;
					neuralLayers[layerIndex][neuronIndex].error = (curValue - expected[neuronIndex])*(curValue - expected[neuronIndex]); // *curValue* (1 - curValue);
				}
			}
			else
			{
				for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++)
				{
					float errorSum = 0;

					for (int nextLayerNeuronIndex = 0; nextLayerNeuronIndex < neuralLayers[layerIndex + 1].size(); nextLayerNeuronIndex++)
					{
						float error = neuralLayers[layerIndex + 1][nextLayerNeuronIndex].error;
						float weight = neuralLayers[layerIndex + 1][nextLayerNeuronIndex].weights[neuronIndex];

						errorSum += error * weight; // weighter error
					}
					float curValue = neuralLayers[layerIndex][neuronIndex].value;
					// neuralLayers[layerIndex][neuronIndex].error = curValue * (1 - curValue) * errorSum; // Why do that?!
				}
			}
		}
	}
}

void NeuralNetwork::weightsUpdate()
{
	for (int layerIndex = 0; layerIndex < neuralLayers.size(); layerIndex++)
	{
		for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++)
		{
			for (int weightIndex = 0; weightIndex < neuralLayers[layerIndex][neuronIndex].weights.size(); weightIndex++)
			{
				// neuralLayers[layerIndex][neuronIndex].weights[weightIndex] =
				// = neuralLayers[layerIndex][neuronIndex].weights[weightIndex] + 
				// + learningrate * errors[index - 1] * 
				// * neuralLayers[layerIndex][neuronIndex].value;

				// float j = learningrate * errors[index - 1] * neuralLayers[layerIndex][neuronIndex].value;

				// index = index - 1;
			}
		}
	}
}

void NeuralNetwork::backPropagation()
{
	for (int epoch = 0; epoch < 2; epoch++)
	{
		//calculateErrors();
		weightsUpdate();
	}
}

void NeuralNetwork::saveNetwork(string saveFileName) //todo
{
	// CopyFile("c:/dog.txt", "previous_project_network.csv", FALSE); // Duplicate previous network


	// file pointer 
	fstream fileWrite;

	// opens an existing csv file or creates a new file.
	fileWrite.open(saveFileName, ios::out | ios::trunc);
	if (fileWrite.good())
	{
		// Save Structure

		// numHiddenLayers
		fileWrite << neuralLayers.size()-1 << ';';

		// numInputNeurons
		fileWrite << inputLayer.size() << ';';

		// numHiddenNeurons
		fileWrite << neuralLayers[0].size() << ';';

		// numOutputNeurons
		fileWrite << neuralLayers[m_outputLayerIndex].size() << '\n';

		// Save Neuron Data
		for (int layerIndex = 0; layerIndex < neuralLayers.size(); layerIndex++) // Слои
		{
			for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++) // Нейроны
			{
				for (int weightIndex = 0; weightIndex < neuralLayers[layerIndex][neuronIndex].weights.size(); weightIndex++) //Веса
				{
					//weights
					fileWrite << neuralLayers[layerIndex][neuronIndex].weights[weightIndex] << ';';
				}
				//bias
				fileWrite << neuralLayers[layerIndex][neuronIndex].bias << '\n';
			}
		}

	}

	fileWrite.close();
}

// Test Functions
void NeuralNetwork::testSetInput(float inputOne, float inputTwo)
{
	inputLayer[0] = inputOne;
	inputLayer[1] = inputTwo;
}
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
void NeuralNetwork::testPrintResult()
{
	float maxValue = 0;
	int maxValueIndex = -1;
	for (int neuronIndex = 0; neuronIndex < neuralLayers[m_outputLayerIndex].size(); neuronIndex++)
	{
		if (maxValue < neuralLayers[m_outputLayerIndex][neuronIndex].value)
		{
			maxValue = neuralLayers[m_outputLayerIndex][neuronIndex].value;
			maxValueIndex = neuronIndex;
		}
	}
	switch (maxValueIndex)
	{
	case 0:
		cout << "Red" << endl;
		break;
	case 1:
		cout << "Pink" << endl;
		break;
	case 2:
		cout << "Orange" << endl;
		break;
	case 3:
		cout << "Brown" << endl;
		break;
	case 4:
		cout << "Yellow" << endl;
		break;
	case 5:
		cout << "Green" << endl;
		break;
	case 6:
		cout << "Sky Blue" << endl;
		break;
	case 7:
		cout << "Blue" << endl;
		break;
	case 8:
		cout << "Violet" << endl;
		break;
	case 9:
		cout << "White" << endl;
		break;
	case 10:
		cout << "Grey" << endl;
		break;
	case 11:
		cout << "Black" << endl;
		break;
	case 12:
		cout << "Beige" << endl;
		break;
	default:
		break;
	}
}