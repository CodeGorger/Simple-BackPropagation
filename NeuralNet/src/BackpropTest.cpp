//#define _BACKPROPAGATION_TEST_
#ifdef _BACKPROPAGATION_TEST_
//============================================================================
// Name        : NeuralNet.cpp
// Author      : Simon Poschenrieder
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip>
#include <fstream>
#include "Net/Net.h"

int main(int argc, char **argv)
{

	Net myTestNet(std::string("xxx"));
	myTestNet.SetLearnRate(0.5);

	std::vector<double> layersBiases;
	layersBiases.push_back(0.0);
	layersBiases.push_back(0.35);
	layersBiases.push_back(0.6);
	myTestNet.SetLayerBiases(layersBiases);

	std::vector<double> in;
	in.push_back(0.05);
	in.push_back(0.1);
	myTestNet.FeedForward(in);

	std::vector<double> target;
	target.push_back(0.01);
	target.push_back(0.99);
	myTestNet.BackPropagation(target);


	return 0;
}
#endif
