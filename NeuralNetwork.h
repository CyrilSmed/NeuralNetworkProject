#pragma once
#include <vector>

using namespace std;

class Neuron
{
private:
	float m_value = 0;
	float m_bias = 0;
	vector<float> m_weights;
public:
	Neuron();
	void setValue(const float value);
	void setBias(const float bias);
	void setWeightAt(int at, float value);

	float getValue();
	float getBias();
	float getWeightAt(int at);
};

class Layer
{
private:
public:
	vector<Neuron> m_neurons; //todo make it private

	Layer();
	Layer(int neuronCount);
	void resizeLayer(int neuronCount);
};

class Network
{
private:

public:
	Layer inputLayer;//todo make it private?
	vector<Layer> hiddenLayers;//todo make it private?
	Layer outputLayer;//todo make it private?

	Network(int numHiddenLayers, int numInputNeurons, int numHiddenNeurons, int numOutputNeurons);
};