#include <iostream>
#include <vector>
#include "NeuralNetwork.h"

using namespace std;

//Neuron
Neuron::Neuron() {}
void Neuron::setValue(const float value) { m_value = value; } //todo from 0 to 1
void Neuron::setBias(const float bias) { m_bias = bias; }
void Neuron::setWeightAt(int at, float value) { m_weights[at] = value; }

float Neuron::getValue() { return m_value; }
float Neuron::getBias() { return m_bias; }
float Neuron::getWeightAt(int at) { return m_weights[at]; }

//NeuronLayer
Layer::Layer() {};
Layer::Layer(int neuronCount)
{
	resizeLayer(neuronCount);
}
void Layer::resizeLayer(int neuronCount)
{
	m_neurons.resize(neuronCount);
}

//Network
Network::Network(int numHiddenLayers, int numInputNeurons, int numHiddenNeurons, int numOutputNeurons) // todo weights initialization
{
	hiddenLayers.resize(numHiddenLayers);

	inputLayer.resizeLayer(numInputNeurons);
	
	for (Layer layer : hiddenLayers)
	{
		layer.resizeLayer(numHiddenNeurons);
	}

	outputLayer.resizeLayer(numOutputNeurons);
}
