/**
 * @file
 */

#include "Random.h"

#include <chrono>
#include <random>
#include <stdlib.h>

namespace core {

Random::Random() :
		_engine(std::default_random_engine()) {
}

Random::Random(unsigned int seed) :
		_engine(std::default_random_engine()) {
	_engine.seed(seed);
}

void Random::setSeed(unsigned int seed) {
	_engine.seed(seed);
}

float Random::randomf(float min, float max) const {
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(_engine);
}

int Random::random(int min, int max) const {
	if (min == max) {
		return min;
	}
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(_engine);
}

float Random::randomBinomial(float max) const {
	return this->randomf(0.0f, max) - this->randomf(0.0f, max);
}

}
