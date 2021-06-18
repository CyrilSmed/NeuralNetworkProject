#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include "NeuralNetwork.h"

using namespace std;

int main()
{
	NeuralNetwork* testNetwork;
	int input = -1;
	printf("\n	Would you like intialize and train a new Network (1)\n	or open one from a file (2)\n>");
	if (scanf("%d", &input) == 0) printf("Read error\n");
	if (input == 1)
	{
		testNetwork = new NeuralNetwork(1, 3, 13, 13);
		testNetwork->trainNetwork("training_data_set.csv", "testing_data_set.csv", 50, 0.2);
	}
	else
	{
		testNetwork = new NeuralNetwork("network_data.csv");
	}

	while (testNetwork->testInterface());
	testNetwork->saveNetwork("network_data.csv");

	delete testNetwork;
}