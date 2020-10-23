#pragma once


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.



#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include "Point2.h"

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

	void set_num_sets(const int np);

	virtual void generate_samples(void) = 0;

	int get_num_samples(void);

	void shuffle_x_coordinates(void);

	void shuffle_y_coordinates(void);

	void setup_shuffled_indices(void);

	void map_samples_to_unit_disk(void);

	void
		map_samples_to_hemisphere(const float p);

	void
		map_samples_to_sphere(void);


	// the following functions are not const because they change count and jump

	Vector2											// get next sample on unit square
		sample_unit_square(void);

	Vector2											// get next sample on unit disk
		sample_unit_disk(void);

	Vector3											// get next sample on unit hemisphere
		sample_hemisphere(void);

	Vector3											// get next sample on unit sphere
		sample_sphere(void);

	Vector2											// only used to set up a vector noise table
		sample_one_set(void);							// this is not discussed in the book, but see the
														// file LatticeNoise.cpp in Chapter 31

protected:

	int 					num_samples;     		// the number of sample points in a set
	int 					num_sets;				// the number of sample sets
	std::vector<Point2>			samples;				// sample points on a unit square
	std::vector<int>				shuffled_indices;		// shuffled samples array indices
	std::vector<Point2>			disk_samples;			// sample points on a unit disk
	std::vector<Vector3> 		hemisphere_samples;		// sample points on a unit hemisphere
	std::vector<Vector3> 		sphere_samples;			// sample points on a unit sphere
	unsigned long 			count;					// the current number of sample points used
	int 					jump;					// random index jump
};



