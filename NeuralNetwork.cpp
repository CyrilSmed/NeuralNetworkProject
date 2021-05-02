#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>  // pause and cls
#include <fstream> // file Management
#include <sstream> // file Management
#include "NeuralNetwork.h"
#include "CommaSeparatedFile.h"

using namespace std;

const string NeuralNetwork::m_colorLabels[13] = 
{ "Red", "Pink", "Orange", "Brown", "Yellow", 
"Green", "Sky Blue", "Blue", "Violet", "White",
"Grey", "Black", "Beige"};


NeuralNetwork::NeuralNetwork(int numHiddenLayers, int numInputNeurons, int numHiddenNeurons, int numOutputNeurons) // Инициализация сети с нуля
{
	srand(time(0));

	// Входной слой
	inputLayer.resize(numInputNeurons);

	// Массив скрытых слоев 
	neuralLayers.resize(numHiddenLayers + 1); // Кол-во скрытых слоев + 1 выходной слой
	// Нейроны скрытых слоев
	for (int i = 0; i < neuralLayers.size() - 1; i++) neuralLayers[i].resize(numHiddenNeurons);
	// Скрытые слои и выходной слой хранятся в одном массиве

	// Выходной слой
	m_outputLayerIndex = numHiddenLayers;

	neuralLayers[m_outputLayerIndex].resize(numOutputNeurons);

	// Изменение размерности массивов с весами 
	for (int layerIndex = 0; layerIndex < neuralLayers.size(); layerIndex++) // Слои
	{
		for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++) // Нейроны
		{
			if (layerIndex == 0) // Первый скрытый слой соединен с входным слоем 
				neuralLayers[layerIndex][neuronIndex].weights.resize(inputLayer.size());
			else // Последующие скрытые слои и выходной слой соединины с предыдущими 
				neuralLayers[layerIndex][neuronIndex].weights.resize(neuralLayers[layerIndex - 1].size());
		}
	}
	initializeWeightsRandomly();
}
NeuralNetwork::NeuralNetwork(string initFileName, char delimChar) // Инициализвация сохраненной в файле нейронной сетью
{
	string fileValue;

	CommaSeparatedDataFileRead networkFile(initFileName, delimChar);

	// Массив скрытых слоев
	networkFile.getNextValue(&fileValue);
	int intFileValue = stoi(fileValue);
	m_outputLayerIndex = intFileValue; // Индекс выходного слоя = Количеству скрытых слоев
	neuralLayers.resize(intFileValue + 1); // Кол-во скрытых слоев + 1 выходной слой
	// Скрытые слои и выходной слой хранятся в одном массиве

	// Входной слой
	networkFile.getNextValue(&fileValue);
	inputLayer.resize(stoi(fileValue));

	// Нейроны скрытых слоев
	networkFile.getNextValue(&fileValue);
	intFileValue = stoi(fileValue);
	for (int i = 0; i < neuralLayers.size() - 1; i++) neuralLayers[i].resize(intFileValue);

	// Выходной слой
	networkFile.getNextValue(&fileValue);
	neuralLayers[m_outputLayerIndex].resize(stoi(fileValue));

	// Изменение размерности массивов с весами
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

	// Назначение весов и сдвигов по файлу
	for (int layerIndex = 0; layerIndex < neuralLayers.size(); layerIndex++) // Слои
	{
		for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++) // Нейроны
		{
			for (int weightIndex = 0; weightIndex < neuralLayers[layerIndex][neuronIndex].weights.size(); weightIndex++) // Веса
			{
				networkFile.getNextValue(&fileValue);
				neuralLayers[layerIndex][neuronIndex].weights[weightIndex] = stof(fileValue);
			}
			networkFile.getNextValue(&fileValue);
			neuralLayers[layerIndex][neuronIndex].bias = stof(fileValue);
		}
	}

}

void NeuralNetwork::initializeWeightsRandomly() // Случайная инициализация весов для создания необученной сети
{
	for (int layerIndex = 0; layerIndex < neuralLayers.size(); layerIndex++) // Слои
	{
		for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++) // Нейроны
		{
			for (int weightIndex = 0; weightIndex < neuralLayers[layerIndex][neuronIndex].weights.size(); weightIndex++) //Веса
			{
				neuralLayers[layerIndex][neuronIndex].weights[weightIndex] = ((long double)rand() / (RAND_MAX))*2 - 1; // от -1 до 1
			}
		}
	}
}

void NeuralNetwork::forwardPropagate()
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

				if (layerIndex == 0) value = inputLayer[connectionIndex]; // Первый скрытый слой соединен с входным слоем 
				else value = neuralLayers[layerIndex - 1][connectionIndex].value; // Последующие скрытые слои и выходной слой соединины с предыдущими 

				calculatedValue += weight * value; // Считаем взвешенную сумму
			}
			calculatedValue += neuralLayers[layerIndex][neuronIndex].bias; // Добавляем сдвиг

			neuralLayers[layerIndex][neuronIndex].value = 1 / (1 + exp(-calculatedValue)); // Применяем сигмоидную функцию
		}
	}

}
float NeuralNetwork::calculateErrorSum(const vector<float> expected) // Функция ошибки, которую мы минимизируем
{
	// Нейронные сети используют разные функции ошибки. Самая распространенная: (value - expectedValue)^2
	// То есть разница между фактическим значением и ожидаемым (правильным) в квадрате
	float errorSum = 0;
	for (int neuronIndex = 0; neuronIndex < neuralLayers[m_outputLayerIndex].size(); neuronIndex++)
	{
		float valueDifference = neuralLayers[m_outputLayerIndex][neuronIndex].value - expected[neuronIndex];
		errorSum += valueDifference * valueDifference;
	}
	return errorSum;
}
void NeuralNetwork::backwardPropagateErrors(const vector<float> expected)
{
	if (expected.size() == neuralLayers[m_outputLayerIndex].size()) // Если массив expected имеет правильную размерность
	{
		for (int layerIndex = m_outputLayerIndex; layerIndex >= 0; layerIndex--) // Идем от выходного слоя к первому скрытому
		{
			if (layerIndex == m_outputLayerIndex) // Значения выходных нейронов сравниваются с ожидаемыми (правильными)
			{
				for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++)
				{
					float curValue = neuralLayers[layerIndex][neuronIndex].value;
					neuralLayers[layerIndex][neuronIndex].error = (curValue - expected[neuronIndex]); 
				}
			}
			else // Значения остальных нейронов сравниваются со взвешенными нейронами, с ними связанными
			{    // Так определяется влияние каждого нейрона на вывод неправильного ответа (их ошибка)
				for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++)
				{
					float errorSum = 0; 
					// Все нейроны (кроме выходных) влияют на ошибку по средством изменения значений нейронов, стоящих перед ними
					// Так нейроны (кроме выходных) влияют на ошибку несколькими связями. Ошибка каждой связи взвешивается силой/весом связи и суммируется
			
					for (int nextLayerNeuronIndex = 0; nextLayerNeuronIndex < neuralLayers[layerIndex + 1].size(); nextLayerNeuronIndex++)
					{
						float error = neuralLayers[layerIndex + 1][nextLayerNeuronIndex].error;
						float weight = neuralLayers[layerIndex + 1][nextLayerNeuronIndex].weights[neuronIndex];

						errorSum += error * weight; 
					}
					neuralLayers[layerIndex][neuronIndex].error = errorSum;
				}
			}
			for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++)
			{
				float curValue = neuralLayers[layerIndex][neuronIndex].value;
				neuralLayers[layerIndex][neuronIndex].error *= curValue * (1 - curValue); 
				// Фактически мы получаем частичную производную функции ошибки по отношению к значению каждого нейрона
				// То есть то, как это нейрон изменяет ошибку (увеличивает или уменьшает и насколько)
			}
		}
	}
}
void NeuralNetwork::updateWeights()
{
	// После подсчета ошибок (частичной производной функции ошибки по отношению к значению каждого нейрона)
	// мы можем соответственно изменить значения весов и сдвигов для минимизации ошибки
	// Для этого мы должны знать, как веса и сдвиги связей, изменяют значения нейронов
	// Фактически мы считаем частичную производную функции ошибки по отношению к весам и сдвигам
	// То есть то, как вес или сдвиг изменяет ошибку (увеличивает или уменьшает и насколько)

	for (int layerIndex = 0; layerIndex < neuralLayers.size(); layerIndex++)
	{
		for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++)
		{
			float error = neuralLayers[layerIndex][neuronIndex].error;

			for (int connectionIndex = 0; connectionIndex < neuralLayers[layerIndex][neuronIndex].weights.size(); connectionIndex++)
			{
				float connectedNeuronValue = 0; // Влияние веса связи зависит от нейрона этой связи

				if (layerIndex == 0) connectedNeuronValue = inputLayer[connectionIndex]; // Первый скрытый слой соединен с входным слоем  
				else connectedNeuronValue = neuralLayers[layerIndex - 1][connectionIndex].value; // Последующие скрытые слои соединины с предыдущими  

				neuralLayers[layerIndex][neuronIndex].weights[connectionIndex] -= m_learningRate * error * connectedNeuronValue;
				// Мы можем определить то, как данный вес изменяет значение ошибки, зная, как ошибку изменяет нейрон с ним связанный 
				// Это мы посчитали в backwardPropagateErrors() и сейчас сохранили в переменной "error"
				// Он делает это посредством предыдущего нейрона, связанного с текущим связью этого веса "connectedNeuronValue"
				// Так мы вычитаем из нейрона значение пропорциональное тому, как он изменяет ошибку. Этим мы её минимизируя 
				// (m_learningRate - коэффициент скорости обучения, устанавливаемый пользователем)

			}

			neuralLayers[layerIndex][neuronIndex].bias -= m_learningRate * error;
			// В отличие от веса, сдвиг изменяет вес нейрона напрямую, а значит напрямую изменяет и значение ошибки
			// Значит, то как изменяет ошибку нейрон (значение, хранящееся в переменной "error") и то, как изменяет значение ошибки вес - равнозначно
		}
	}
}
float NeuralNetwork::runTestingDataSet(const string testingDataSetFileName, const char delimChar) 
{
	// Для обучения и тестирования сети лучше использовать разные наборы данных
	// Так можно определить, что сеть обучается слишком долго и уже не обобщает свои ответы, 
	// а подстраивает их конкретно под данный ей для обучения дата сет

	CommaSeparatedDataFileRead testingDataSet(testingDataSetFileName);
	string fileValue;

	int correctResultCount = 0;
	int dataExampleCount = 0;

	vector<float> inputs;
	inputs.resize(inputLayer.size());

	while (testingDataSet.getNextValue(&fileValue))
	{
		for (int i = 0; i < inputLayer.size(); i++) // Получаем значения ввода
		{
			inputLayer[i] = stof(fileValue);
			testingDataSet.getNextValue(&fileValue);
		}
		forwardPropagate();

		int correctResultIndex = stoi(fileValue);
		if (getResultIndex() == correctResultIndex) correctResultCount++;

		dataExampleCount++;
	}
	if (dataExampleCount != 0) return (float)correctResultCount / dataExampleCount;
	else return 0;
}
void NeuralNetwork::trainNetwork(const string trainingDataSetFileName, const string testingDataSetFileName, 
	const int epochNum, const float learningRate, const char delimChar)
{
	m_learningRate = learningRate; // Коэффициент скорости обучения

	vector<float> expected;
	expected.resize(neuralLayers[m_outputLayerIndex].size());

	CommaSeparatedDataFileRead trainingDataSet(trainingDataSetFileName);

	int dataExampleCount = 0;

	for (int epoch = 1; epoch <= epochNum; epoch++) // Количество полных проходов по обучающему набору данных
	{
		float sumError = 0;
		int dataExampleCount = 0;
		string fileValue;

		while (trainingDataSet.getNextValue(&fileValue))
		{
			for (int i = 0; i < expected.size(); i++) expected[i] = 0;

			for (int i = 0; i < inputLayer.size(); i++) // Получаем значения ввода
			{
				inputLayer[i] = stof(fileValue);
				trainingDataSet.getNextValue(&fileValue);
			}

			int expectedOutputIndex = stoi(fileValue); // Получаем ожидаемые значения выхода (правильные ответы)
			if (!fileValue.empty() && expected.size() >= expectedOutputIndex) expected[expectedOutputIndex] = 1;
			else throw runtime_error("Incorrect file format or network structure");

			forwardPropagate();
			sumError += calculateErrorSum(expected);
			backwardPropagateErrors(expected);
			updateWeights();

			dataExampleCount++;
		}

		float errorAverage = 0;
		if (dataExampleCount != 0) errorAverage = sumError / dataExampleCount;

		float testingRunSuccessRate = runTestingDataSet(testingDataSetFileName, delimChar) * 100;

		printf("Epoch %d:	Error Sum = %f;	Testing Run Success Rate = %.1f%%\n", epoch, errorAverage, testingRunSuccessRate);
	}
}

void NeuralNetwork::saveNetwork(const string saveFileName, const char delimChar)
{
	CommaSeparatedDataFileWrite networkFile(saveFileName, delimChar);
	
	// Сохраняем структуру сети
	networkFile.saveNextValue(to_string(neuralLayers.size() - 1));
	networkFile.saveNextValue(to_string(inputLayer.size()));
	networkFile.saveNextValue(to_string(neuralLayers[0].size()));
	networkFile.saveNextValue(to_string(neuralLayers[m_outputLayerIndex].size()));

	// Сохраняем значения весов и сдвигов
	for (int layerIndex = 0; layerIndex < neuralLayers.size(); layerIndex++) // Слои
	{
		for (int neuronIndex = 0; neuronIndex < neuralLayers[layerIndex].size(); neuronIndex++) // Нейроны
		{
			networkFile.nextLine();
			for (int weightIndex = 0; weightIndex < neuralLayers[layerIndex][neuronIndex].weights.size(); weightIndex++) //Веса
			{
				networkFile.saveNextValue(to_string(neuralLayers[layerIndex][neuronIndex].weights[weightIndex]));
			}
			networkFile.saveNextValue(to_string(neuralLayers[layerIndex][neuronIndex].bias)); // Сдвиги
		}
	}
}

// Функции для тестирования
void NeuralNetwork::setInputs(const vector<float> inputs)
{
	if (inputs.size() == inputLayer.size())
	{
		for (int i = 0; i < inputs.size(); i++)
		{
			inputLayer[i] = inputs[i];
		}
	}
	else throw runtime_error("Incorrect input format");
}
int NeuralNetwork::getResultIndex()
{
	int maxElIndex = -1;
	float maxEl = -1;
	for (int i = 0; i < neuralLayers[m_outputLayerIndex].size(); i++)
	{
		if (neuralLayers[m_outputLayerIndex][i].value > maxEl)
		{
			maxEl = neuralLayers[m_outputLayerIndex][i].value;
			maxElIndex = i;
		}
	}
	return maxElIndex;
}
bool NeuralNetwork::testInterface()
{
	int R, G, B;
	printf("Type in RGB. To exit type in a negative value\n");
	if (scanf("%d", &R) == 0) printf("Read error\n");
	if (scanf("%d", &G) == 0) printf("Read error\n");
	if (scanf("%d", &B) == 0) printf("Read error\n");

	if (R < 0 || G < 0 || B < 0) return false;
	setInputs({ (float)R / 255, (float)G / 255, (float)B / 255 });

	forwardPropagate();

	cout << "The color you have entered is: " << m_colorLabels[getResultIndex()] << endl;
	return true;
}

