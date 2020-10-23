// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include <algorithm>   // for random_shuffle in Sampler::SetupShuffledIndices
#include "Sampler.h"
#include <cstdlib>
#include <random>

const float PI = 3.14159265358979323846f;
const float TWO_PI = (2.0f * PI);

// ------------------------------------------------------------------ default constructor

Sampler::Sampler(void)
	: num_samples(1),
	num_sets(83),
	count(0),
	jump(0)
{
	samples.reserve(num_samples * num_sets);
	SetupShuffledIndices();
}


// ------------------------------------------------------------------ constructor

Sampler::Sampler(const int ns)
	: num_samples(ns),
	num_sets(83),
	count(0),
	jump(0)
{
	samples.reserve(num_samples * num_sets);
	SetupShuffledIndices();
}


// ------------------------------------------------------------------ constructor

Sampler::Sampler(const int ns, const int n_sets)
	: num_samples(ns),
	num_sets(n_sets),
	count(0),
	jump(0)
{
	samples.reserve(num_samples * num_sets);
	SetupShuffledIndices();
}


// ------------------------------------------------------------------ copy constructor

Sampler::Sampler(const Sampler& s)
	: num_samples(s.num_samples),
	num_sets(s.num_sets),
	samples(s.samples),
	shuffled_indices(s.shuffled_indices),
	disk_samples(s.disk_samples),
	mHemisphereSamples(s.mHemisphereSamples),
	mSphereSamples(s.mSphereSamples),	
	jump(s.jump)
{
	std::memory_order order = std::memory_order_seq_cst;
	count = s.count.load(order);
}


// ------------------------------------------------------------------ assignment operator

Sampler& Sampler::operator= (const Sampler& rhs)
{
	if (this == &rhs)
		return (*this);

	num_samples = rhs.num_samples;
	num_sets = rhs.num_sets;
	samples = rhs.samples;
	shuffled_indices = rhs.shuffled_indices;
	disk_samples = rhs.disk_samples;
	mHemisphereSamples = rhs.mHemisphereSamples;
	mSphereSamples = rhs.mSphereSamples;	
	jump = rhs.jump;

	std::memory_order order = std::memory_order_seq_cst;
	count = rhs.count.load(order);

	return (*this);
}

Sampler::~Sampler(void) {}


// ------------------------------------------------------------------- SetNumSets

void Sampler::SetNumSets(const int np)
{
	num_sets = np;
}


// ------------------------------------------------------------------- GetNumSamples

int Sampler::GetNumSamples(void)
{
	return (num_samples);
}


// ------------------------------------------------------------------- ShuffleXCoordinates
// shuffle the x coordinates of the points within each set

void Sampler::ShuffleXCoordinates(void)
{
	for (int p = 0; p < num_sets; p++)
	{
		for (int i = 0; i < num_samples - 1; i++) 
		{
			int target = std::rand() % num_samples + p * num_samples;
			float temp = samples[i + p * num_samples + 1].X;
			samples[i + p * num_samples + 1].X = samples[target].X;
			samples[target].X = temp;
		}
	}
}


// ------------------------------------------------------------------- ShuffleYCoordinates
// shuffle the y coordinates of the points within set

void Sampler::ShuffleYCoordinates(void)
{
	for (int p = 0; p < num_sets; p++)
	{
		for (int i = 0; i < num_samples - 1; i++)
		{
			int target = std::rand() % num_samples + p * num_samples;
			float temp = samples[i + p * num_samples + 1].Y;
			samples[i + p * num_samples + 1].Y = samples[target].Y;
			samples[target].Y = temp;
		}
	}
}


// ------------------------------------------------------------------- SetupShuffledIndices
// sets up randomly shuffled indices for the samples array

void
Sampler::SetupShuffledIndices(void)
{
	shuffled_indices.reserve(num_samples * num_sets);
	std::vector<int> indices;

	std::random_device rd;
	std::mt19937 g(rd());
	
	for (int j = 0; j < num_samples; j++)
		indices.push_back(j);

	for (int p = 0; p < num_sets; p++) {
		
		std::shuffle(indices.begin(), indices.end(), g);

		for (int j = 0; j < num_samples; j++)
			shuffled_indices.push_back(indices[j]);
	}
}


// ------------------------------------------------------------------- MapSamplesToUnitDisk

// Maps the 2D sample points in the square [-1,1] X [-1,1] to a unit disk, using Peter Shirley's
// concentric map function

void Sampler::MapSamplesToUnitDisk(void)
{
	auto size = samples.size();
	float r, phi;		// polar coordinates
	Point2 sp; 		// sample point on unit disk

	disk_samples.reserve(size);

	for (int j = 0; j < size; j++) {
		// map sample point to [-1, 1] X [-1,1]

		sp.X = 2.0f * samples[j].X - 1.0f;
		sp.Y = 2.0f * samples[j].Y - 1.0f;

		if (sp.X > -sp.Y) {			// sectors 1 and 2
			if (sp.X > sp.Y) {		// sector 1
				r = sp.X;
				phi = sp.Y / sp.X;
			}
			else {					// sector 2
				r = sp.Y;
				phi = 2 - sp.X / sp.Y;
			}
		}
		else {						// sectors 3 and 4
			if (sp.X < sp.Y) {		// sector 3
				r = -sp.X;
				phi = 4 + sp.Y / sp.X;
			}
			else {					// sector 4
				r = -sp.Y;
				if (sp.Y != 0.0)	// avoid division by zero at origin
					phi = 6 - sp.X / sp.Y;
				else
					phi = 0.0;
			}
		}

		phi *= PI / 4.0;

		disk_samples[j].X = r * cos(phi);
		disk_samples[j].Y = r * sin(phi);
	}

	samples.erase(samples.begin(), samples.end());
}


// ------------------------------------------------------------------- MapSamplesToHemiSphere

// Maps the 2D sample points to 3D points on a unit hemisphere with a cosine power
// density distribution in the polar angle

void
Sampler::MapSamplesToHemiSphere(const float exp)
{
	auto size = samples.size();
	mHemisphereSamples.reserve(num_samples * num_sets);

	for (int j = 0; j < size; j++) 
	{
		float cos_phi = cos(2.0 * PI * samples[j].X);
		float sin_phi = sin(2.0 * PI * samples[j].X);
		float cos_theta = pow((1.0 - samples[j].Y), 1.0 / (exp + 1.0));
		float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		float pu = sin_theta * cos_phi;
		float pv = sin_theta * sin_phi;
		float pw = cos_theta;
		mHemisphereSamples.push_back(Point3(pu, pv, pw));
	}
}


// ------------------------------------------------------------------- MapSamplesToSphere

// Maps the 2D sample points to 3D points on a unit sphere with a uniform density 
// distribution over the surface
// this is used for modelling a spherical light

void Sampler::MapSamplesToSphere(void)
{
	float r1, r2;
	float x, y, z;
	float r, phi;

	mSphereSamples.reserve(num_samples * num_sets);

	for (int j = 0; j < num_samples * num_sets; j++)
	{
		r1 = samples[j].X;
		r2 = samples[j].Y;
		z = 1.0f - 2.0f * r1;
		r = sqrt(1.0 - z * z);
		phi = TWO_PI * r2;
		x = r * cos(phi);
		y = r * sin(phi);
		mSphereSamples.push_back(Point3(x, y, z));
	}
}


// ------------------------------------------------------------------- SampleUnitSquare
// the final version in Listing 5.13

Point2 Sampler::SampleUnitSquare(void)
{
	if (count % num_samples == 0)  									// start of a new pixel
		jump = (std::rand() % num_sets) * num_samples;				// random index jump initialised to zero in constructor

	return (samples[jump + shuffled_indices[jump + count++ % num_samples]]);
}




/*

// ------------------------------------------------------------------- SampleUnitSquare
// the first revised version in Listing in Listing 5.8

Point2
Sampler::SampleUnitSquare(void) {
	if (count % num_samples == 0)  									// start of a new pixel
		jump = (rand_int() % num_sets) * num_samples;				// random index jump initialised to zero in constructor

	return (samples[jump + count++ % num_samples]);
}

*/



/*

// ------------------------------------------------------------------- SampleUnitSquare
// the original version in Listing 5.7

Point2
Sampler::SampleUnitSquare(void) {
	return (samples[count++ % (num_samples * num_sets)]);
}

*/



// ------------------------------------------------------------------- SampleUnitDisk

Point2
Sampler::SampleUnitDisk(void)
{
	if (count % num_samples == 0)  									// start of a new pixel
		jump = (std::rand() % num_sets) * num_samples;

	return (disk_samples[jump + shuffled_indices[jump + count++ % num_samples]]);
}



// ------------------------------------------------------------------- SampleHemiSphere

Point3
Sampler::SampleHemiSphere(void)
{
	if (count % num_samples == 0)  									// start of a new pixel
		jump = (std::rand() % num_sets) * num_samples;

	return (mHemisphereSamples[jump + shuffled_indices[jump + count++ % num_samples]]);
}



// ------------------------------------------------------------------- SampleSphere

Point3
Sampler::SampleSphere(void)
{
	if (count % num_samples == 0)  									// start of a new pixel
		jump = (std::rand() % num_sets) * num_samples;

	return (mSphereSamples[jump + shuffled_indices[jump + count++ % num_samples]]);
}



// ------------------------------------------------------------------- SampleOneSet
// This is a specialised function called in LatticeNoise::init_vector_table
// It doesn't shuffle the indices

Point2
Sampler::SampleOneSet(void)
{
	return(samples[count++ % num_samples]);
}



