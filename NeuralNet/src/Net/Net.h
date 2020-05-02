/*
 * Net.h
 *
 *  Created on: 12.04.2020
 *      Author: simon
 */

#ifndef NET_NET_H_
#define NET_NET_H_

#include <vector>
#include "../Matrix/Matrix.h"

class Net
{
public:
	Net(std::vector<size_t> inTopology, double inMinRandForInit=-20., double inMaxRandForInit=20.);

	Net(std::string specialInit);

	void FeedForward(
		const std::vector<double> & inInputValues) const;

	void BackPropagation(
		const std::vector<double> & inTarget);

	void GetNodeOutputs(std::vector<std::vector<double>> & outNetOutput) const;

	virtual ~Net();

	void SetLearnRate( double inLearnRate );

	void SetLayerBiases(const std::vector<double> & inBiases);

	std::string PrintFinalOutput();
	std::string PrintWeights();
private:
	void weightCorrection();

	// Defines how many neurons are in each layer
	// e.g. {3,3,1} ->
	// l1->	l2->
	// l1->	l2-> l3
	// l1->	l2->
	std::vector<size_t> _layers;

	std::vector<Matrix<double>> _weights;
	std::vector<std::vector<double>> _bias;

	mutable std::vector<std::vector<double>> _layersInputs;
	mutable std::vector<std::vector<double>> _layersOutputs;


	double _learnRate;
};

#endif /* NET_NET_H_ */
