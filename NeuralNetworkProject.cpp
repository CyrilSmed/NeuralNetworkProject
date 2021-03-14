#include <stdio.h>  // for pause and cls
#include <iostream>
#include <vector>
#include "NeuralNetwork.h"
=======
>>>>>>> parent of 929a822 (sdfg)

using namespace std;
int main()
{
	Network curNetwork(1, 3, 3, 10);

	curNetwork.inputLayer.m_neurons[0].setValue(0.8);

	cout << curNetwork.inputLayer.m_neurons[0].getValue() << endl;

	system("pause");
	//system("cls");
}