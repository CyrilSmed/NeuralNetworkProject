#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>  // pause and cls
#include <fstream> // file Management
#include <sstream> // file Management
#include "NeuralNetwork.h"

using namespace std;

int main()
{
	// Инициализировать новую сеть
	NeuralNetwork testNetwork(1, 3, 6, 13);

	// Инициализировать старую сеть файлом
	//NeuralNetwork testNetwork("network_data.csv");

	testNetwork.trainNetwork("training_data_set.csv", "testing_data_set.csv", 50, 0.2);

	while (testNetwork.testInterface());
	testNetwork.saveNetwork("network_data.csv");

	int pause;
	cin >> pause;

}