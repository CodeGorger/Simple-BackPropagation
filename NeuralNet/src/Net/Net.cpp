/*
 * Net.cpp
 *
 *  Created on: 12.04.2020
 *      Author: simon
 */

#include "Net.h"
#include <cstdlib>
#include <ctime>

//TODO(Simon): iostream not needed here
#include <iostream>

//Net::Net() {
//	// TODO Auto-generated constructor stub
//
//}

double fRand( double fMin, double fMax )
{
	double f =(double)rand() / RAND_MAX;
	return fMin + f*(fMax - fMin);
}


Net::Net(std::vector<size_t> inTopology, double inMinRandForInit, double inMaxRandForInit)
{
	_learnRate=0.5;
	_layers = inTopology;
	// for each edges betweens layers
	for(size_t i=0;i<_layers.size()-1;i++)
	{
		size_t rows=_layers[i+1];
		size_t cols=_layers[i];
		Matrix<double> tmpM(rows,cols);

		//Fill Matrix with values between min rand and max rand (default -20 and 20)
		for(size_t ii=0;ii<rows;ii++)
		{
			for(size_t jj=0;jj<cols;jj++)
			{
				tmpM.Set(ii,jj,fRand(inMinRandForInit,inMaxRandForInit)/cols);
			}
		}
		_weights.push_back(tmpM);
	}

	// init temporary layer outputs
	_layersOutputs=std::vector<std::vector<double>>(_layers.size());
	_layersInputs=std::vector<std::vector<double>>(_layers.size());
	_bias=std::vector<std::vector<double>>(_layers.size());
}

Net::Net(std::string specialInit)
{
	_learnRate=0.5;
	_layers = std::vector<size_t>();
	_layers.push_back(2);
	_layers.push_back(2);
	_layers.push_back(2);

	// for each edges betweens layers
	size_t rows=_layers[1];
	size_t cols=_layers[0];
	Matrix<double> tmpM1(rows,cols);
	tmpM1.Set(0,0,0.15);
	tmpM1.Set(0,1,0.2);
	tmpM1.Set(1,0,0.25);
	tmpM1.Set(1,1,0.3);
	_weights.push_back(tmpM1);
	rows=_layers[2];
	cols=_layers[1];
	Matrix<double> tmpM2(rows,cols);
	tmpM2.Set(0,0,0.4);
	tmpM2.Set(0,1,0.45);
	tmpM2.Set(1,0,0.5);
	tmpM2.Set(1,1,0.55);
	_weights.push_back(tmpM2);

	_layersOutputs=std::vector<std::vector<double>>(_layers.size());
	_layersInputs=std::vector<std::vector<double>>(_layers.size());
	_bias=std::vector<std::vector<double>>(_layers.size());
}


Net::~Net()
{
}


std::string Net::PrintWeights()
{
	std::string ret;

	for(size_t i=0;i<_weights.size();i++)
	{
		ret+=_weights[i].Print();
		ret+="\n";
	}

	return ret;
}


std::string Net::PrintFinalOutput()
{
	std::string ret="";
	for( size_t i=0; i<_layersOutputs.back().size() ; i++ )
	{
		ret+=std::to_string(_layersOutputs.back()[i])+"\n";
	}
	return ret;
}


void Net::SetLayerBiases(const std::vector<double> & inBiases)
{
	if(inBiases.size() != _bias.size())
	{
		//TODO(Simon): Complain
	}
	for( size_t i=0 ; i<_bias.size() ; i++)
	{
		_bias[i]=std::vector<double>(_layers[i],inBiases[i]);
	}
}

std::vector<double> ElementWiseMultiplication(
		 const std::vector<double> & lhs
		,const std::vector<double> & rhs)
{
	if(lhs.size() != rhs.size())
	{
		//TODO(Simon): Complain!
	}
	std::vector<double> ret;

	for(size_t i=0 ; i< lhs.size(); i++)
	{
		ret.push_back( lhs[i]*rhs[i]);
	}

	return ret;
}

std::vector<double> CalcDerivedSigmoid(const std::vector<double> & inputValues)
{
	std::vector<double> ret;

	for(size_t i=0 ; i< inputValues.size(); i++)
	{
		ret.push_back( indianSigmoidDerivation<double>( inputValues[i] ) );
	}

	return ret;
}

std::vector<double> CalcSigmoid(const std::vector<double> & inputValues)
{
	std::vector<double> ret;

	for(size_t i=0 ; i< inputValues.size(); i++)
	{
		ret.push_back( realSigmoid<double>( inputValues[i] ) );
	}

	return ret;
}

void Net::FeedForward(const std::vector<double> & inputValues) const
{

	if(inputValues.size()!=_layers[0])
	{
		//TODO(Simon): Error here ...
		return;
	}

	_layersInputs[0]=inputValues;
	_layersOutputs[0]=inputValues;
	//_layersOutputs[0]=CalcSigmoid(_layersInputs[0]);
	for(size_t i=0 ; i<_weights.size() ; i++)
	{

		_layersInputs[i+1]=(_weights[i]*_layersOutputs[i])+_bias[i+1];
		_layersOutputs[i+1]=CalcSigmoid(_layersInputs[i+1]);
	}
}


// So this can work, a feed forward must have happend.
// inTarget ... is the value it should have had
void Net::BackPropagation(
	const std::vector<double> & inTarget)
{
	if(_layersOutputs.back().size()!=inTarget.size())
	{
		std::cerr << "Net output vector: " << _layersOutputs.back().size() << std::endl;
		std::cerr << "Net target output vector: " << inTarget.size() << std::endl;
		std::cerr << "Sizes not matching!" << std::endl;
		//TODO(Simon): Error here ...
		return;
	}
	double n=_learnRate;


	std::vector<std::vector<double>>::reverse_iterator lo_rit =
		_layersOutputs.rbegin();

	std::vector<std::vector<double>>::reverse_iterator li_rit =
		_layersInputs.rbegin();

	std::vector<Matrix<double>>::reverse_iterator w_rit =
		_weights.rbegin();

	std::vector<Matrix<double>> allWeightDeltas;

	//Step 1a: Calc output neurons delta: out - target
	std::vector<double> delta;

	delta=ElementWiseMultiplication
			( CalcDerivedSigmoid(*lo_rit), ((*lo_rit)-inTarget) );
	++li_rit;
	++lo_rit;
	std::vector<double> prev_delta=delta;


	while(true)
	{
		//Step 2: Calc weight delta
		std::vector<double> tmpLayerOutputs=(*lo_rit);
		Matrix<double> tmpDeltaWeights=(delta*tmpLayerOutputs);
		tmpDeltaWeights.Scale(-n);
		allWeightDeltas.push_back(tmpDeltaWeights);

		if((w_rit+1)==_weights.rend())
		{
			break;
		}

		//Step 1b: Calc hidden neurons delta:...
		std::vector<double> tmpSigIn = CalcDerivedSigmoid(*lo_rit);
		std::vector<double> tmpV = w_rit->Transpose()*prev_delta;
		delta=ElementWiseMultiplication(
				tmpSigIn,
				tmpV);
		w_rit++;
		li_rit++;
		lo_rit++;
		prev_delta=delta;
	}

	std::reverse(allWeightDeltas.begin(),allWeightDeltas.end());

	for( size_t i=0 ; i<_weights.size() ; i++ )
	{
		_weights[i].AddMatrix(allWeightDeltas[i]);
	}
}



void Net::GetNodeOutputs(std::vector<std::vector<double>> & outNetOutput) const
{
	outNetOutput=_layersOutputs;
}


void Net::SetLearnRate( double inLearnRate )
{
	_learnRate=inLearnRate;
}




