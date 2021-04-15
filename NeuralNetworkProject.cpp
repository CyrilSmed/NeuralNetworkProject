#include <stdio.h>  // for pause and cls
#include <iostream>
#include <vector>
#include <fstream> // File Management
#include <string>
#include <sstream> // File Management
#include <Windows.h> // for CopyFile
#include "NeuralNetwork.h"


using namespace std;

int main()
{
	/*
	NeuralNetwork curNetwork(1, 3, 3, 13);
	curNetwork.weightInitialization();


	int R = 0;
	int G = 0;
	int B = 0;
	cout << endl << "Input RGB values:" << endl;
	cin >> R >> G >> B;

	curNetwork.testSetInputColor(R, G, B);
	curNetwork.testPrintValues();
	curNetwork.testPrintResult();
	*/

	NeuralNetwork testNetwork("network_data.csv");
	testNetwork.testSetInput(1, 1);
	testNetwork.forwardPropagation();
	vector<float> expected = { 0, 1 };
	testNetwork.calculateErrors(expected);

	testNetwork.saveNetwork("network_data.csv");

	int pause;
	cin >> pause;

}