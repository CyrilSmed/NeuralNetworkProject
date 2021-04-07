#include <stdio.h>  // for pause and cls
#include <iostream>
#include <vector>
#include "NeuralNetwork.h"

using namespace std;

int main()
{
	NeuralNetwork curNetwork(2, 3, 5, 12);
	curNetwork.weightInitialization();

	curNetwork.testSetInputColor(111, 250, 5);

	curNetwork.forwardPropagation();

	curNetwork.testPrintValues();

	vector<float> testSolution;
	testSolution.resize(12);
	testSolution[6] = 1;

	printf("\n\nCost = %0.3f\n", curNetwork.costFunction(testSolution));
	
	int pause;
	cin >> pause;
}