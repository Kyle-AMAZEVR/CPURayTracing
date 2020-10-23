#pragma once

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include <atomic>
#include <vector>
#include "Vector3.h"
#include "Point2.h"
#include "Point3.h"

class Sampler
{
public:
	Sampler();

	Sampler(const int num);

	Sampler(const int num, const int num_sets);

	Sampler(const Sampler& s);

	Sampler& operator= (const Sampler& rhs);

	virtual Sampler* clone(void) const = 0;

	virtual ~Sampler(void);

	void SetNumSets(const int np);

	virtual void GenerateSamples(void) = 0;

	int GetNumSamples(void);

	void ShuffleXCoordinates(void);

	void ShuffleYCoordinates(void);

	void SetupShuffledIndices(void);

	void MapSamplesToUnitDisk(void);

	void MapSamplesToHemiSphere(const float p);

	void MapSamplesToSphere(void);

	// the following functions are not const because they change count and jump

	// get next sample on unit square
	Point2	SampleUnitSquare(void);

	// get next sample on unit disk
	Point2	SampleUnitDisk(void);

	Point3											// get next sample on unit hemisphere
		SampleHemiSphere(void);

	Point3											// get next sample on unit sphere
		SampleSphere(void);

	Point2											// only used to set up a vector noise table
		SampleOneSet(void);							// this is not discussed in the book, but see the
														// file LatticeNoise.cpp in Chapter 31

protected:

	int 					num_samples;     		// the number of sample points in a set
	int 					num_sets;				// the number of sample sets
	std::vector<Point2>			samples;				// sample points on a unit square
	std::vector<int>				shuffled_indices;		// shuffled samples array indices
	std::vector<Point2>			disk_samples;			// sample points on a unit disk
	std::vector<Point3> 		mHemisphereSamples;		// sample points on a unit hemisphere
	std::vector<Point3> 		mSphereSamples;			// sample points on a unit sphere
	std::atomic<unsigned long>	count;					// the current number of sample points used
	int 					jump;					// random index jump
};



