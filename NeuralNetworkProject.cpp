#include <stdio.h>  // for pause and cls
#include <iostream>
#include <vector>
#include "NeuralNetwork.h"

using namespace std;

int main()
{
	NeuralNetwork curNetwork(1, 3, 3, 10);
	curNetwork.weightInitialization();

	curNetwork.testSetInputColor(111, 250, 5);

	curNetwork.forwardPropagation();

	curNetwork.testPrintValues();
	
	int pause;
	cin >> pause;
}