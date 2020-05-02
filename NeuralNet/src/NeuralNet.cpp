#define _OCR_
#ifdef _OCR_
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

// Target-Data
// [offset] [type]          [value]          [description]
//0000     32 bit integer  0x00000801(2049) magic number (MSB first)
//0004     32 bit integer  60000            number of items
//0008     unsigned byte   ??               label
//0009     unsigned byte   ??               label
//........
//xxxx     unsigned byte   ??               label

// Training-Data
//[offset] [type]          [value]          [description]
//0000     32 bit integer  0x00000803(2051) magic number
//0004     32 bit integer  60000            number of images
//0008     32 bit integer  28               number of rows
//0012     32 bit integer  28               number of columns
//0016     unsigned byte   ??               pixel
//0017     unsigned byte   ??               pixel
//........
//xxxx     unsigned byte   ??               pixel

std::string TargetVectorToString(const std::vector<double> & inVector);

int main(int argc, char **argv)
{
//	std::ofstream TEST;
//	TEST.open("/home/simon/eclipse-workspace/mnist/../output.txt", std::ios::out);
//	TEST << "Testxxx\n";
//	double testDouble=123.456;
//	TEST << std::to_string(testDouble);
//	TEST.close();

	std::string folder="/home/simon/eclipse-workspace/mnist";
	std::string set1_training="/train-images-idx3-ubyte";
	std::string set1_target="/train-labels-idx1-ubyte";
	std::string set2_training="/t10k-images-idx3-ubyte";
	std::string set2_target="/t10k-labels-idx1-ubyte";
	std::string output_file="/../output.txt";

	const int offsetToDataTraining=16;
	const int offsetToDataTargets=8;
	const int entrySizeTraining=784;//28*28=784
	const int entrySizeTargets=1;

	char * tmpTrainEntry=new char[entrySizeTraining];
	char * tmpTargetEntry=new char[entrySizeTargets];

	std::vector<size_t> topology;
	topology.push_back(entrySizeTraining);//28*28=784
	topology.push_back(700);
	topology.push_back(700);
	topology.push_back(10);
	Net myTestNet(topology,-20.0,20.0);
	myTestNet.SetLearnRate(0.05);

	std::vector<double> layersBiases;
	layersBiases.push_back(1.0);
	layersBiases.push_back(1.0);
	layersBiases.push_back(1.0);
	layersBiases.push_back(1.0);
	myTestNet.SetLayerBiases(layersBiases);

	int iterationCounter=0;

	std::ifstream training1_stream;
	std::ifstream targets1_stream;

	std::ofstream analysis_stream;
	analysis_stream.open(folder+output_file, std::ios::out);

	for( int passes=0 ; passes<10 ; passes++ )
	{
		training1_stream.open(folder+set1_training, std::ifstream::binary);
		targets1_stream.open(folder+set1_target, std::ifstream::binary);
		training1_stream.seekg(offsetToDataTraining ,training1_stream.beg);
		targets1_stream.seekg(offsetToDataTargets ,targets1_stream.beg);

		while(!training1_stream.eof())
		{
			iterationCounter++;
			training1_stream.read(tmpTrainEntry, entrySizeTraining);
			targets1_stream.read(tmpTargetEntry, entrySizeTargets);
			std::vector<double> netInput;
			for(int i=0; i<(28*28) ; i++)
			{
				char tmp=tmpTrainEntry[i];
				unsigned char tmpU=static_cast<unsigned char>(tmp);
				double tmpD=((double)(tmpU))/255;
				netInput.push_back(tmpD);
			}

			std::vector<double> netTarget;
			for(int i=0; i<10 ; i++)
			{
				if(tmpTargetEntry[0]==i)
				{
					netTarget.push_back(1.0);
				}
				else
				{
					netTarget.push_back(0.0);
				}
			}

			myTestNet.FeedForward(netInput);
			myTestNet.BackPropagation(netTarget);


			std::string weightsAsString=myTestNet.PrintWeights();
			std::string outAsString=myTestNet.PrintFinalOutput();
			std::string targetAsString=TargetVectorToString(netTarget);

//			analysis_stream << "Weights:\n";
//			analysis_stream << weightsAsString;
//			analysis_stream << "\n";
			analysis_stream << "Output:\n";
			analysis_stream << outAsString;
			analysis_stream << "\n";
			analysis_stream << "Target:\n";
			analysis_stream << targetAsString;
			analysis_stream << "\n";
			analysis_stream << "\n";
		}

		training1_stream.close();
		targets1_stream.close();
	}
	analysis_stream.close();
	return 0;
}


std::string TargetVectorToString(const std::vector<double> & inVector)
{
	std::string ret="";

	for(size_t i=0; i<inVector.size(); i++)
	{
		ret+=std::to_string(inVector[i])+"\n";
	}

	return ret;
}
#endif
