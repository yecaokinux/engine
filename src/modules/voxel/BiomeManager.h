/**
 * @file
 */

#pragma once

#include "core/Common.h"
#include "core/Trace.h"
#include "Constants.h"
#include "polyvox/Region.h"
#include "polyvox/Voxel.h"
#include "MaterialColor.h"
#include <glm/glm.hpp>

namespace voxel {

class Biome {
private:
	friend class BiomeManager;
	Biome() :
			Biome(VoxelType::Grass, getMaterialIndices(VoxelType::Grass), 0, MAX_MOUNTAIN_HEIGHT, 0.5f, 0.5f, false) {
	}

public:
	Biome(VoxelType _type, const MaterialColorIndices& _indices, int16_t _yMin, int16_t _yMax,
			float _humidity, float _temperature, bool _underground) :
			indices(_indices), yMin(_yMin), yMax(_yMax), humidity(_humidity), temperature(
					_temperature), underground(_underground), type(_type) {
	}

	const MaterialColorIndices indices;
	const int16_t yMin;
	const int16_t yMax;
	const float humidity;
	const float temperature;
	const bool underground;
	const VoxelType type;

	inline Voxel voxel(core::Random& random) const {
		return Voxel(type, *random.randomElement(indices.begin(), indices.end()));
	}

	inline Voxel voxel(core::Random& random, uint8_t colorIndex) const {
		const uint8_t max = indices.size() - 1u;
		const uint8_t min = 0u;
		return Voxel(type, glm::clamp(colorIndex, min, max));
	}

	inline Voxel voxel() const {
		thread_local core::Random random;
		return voxel(random);
	}
};

class BiomeManager {
private:
	std::vector<Biome> bioms;

public:
	BiomeManager();
	~BiomeManager();

	bool init(const std::string& luaString);

	bool addBiome(int lower, int upper, float humidity, float temperature, VoxelType type, bool underGround = false);

	// this lookup must be really really fast - it is executed once per generated voxel
	inline Voxel getVoxel(const glm::ivec3& pos, bool underground = false) const {
		core_trace_scoped(BiomeGetVoxel);
		const Biome* biome = getBiome(pos, underground);
		return biome->voxel();
	}

	inline Voxel getVoxel(int x, int y, int z, bool underground = false) const {
		return getVoxel(glm::ivec3(x, y, z), underground);
	}

	bool hasCactus(const glm::ivec3& pos) const;
	bool hasTrees(const glm::ivec3& pos) const;
	bool hasCity(const glm::ivec3& pos) const;
	bool hasClouds(const glm::ivec3& pos) const;
	bool hasPlants(const glm::ivec3& pos) const;

	int getCityDensity(const glm::ivec3& pos) const;
	float getCityGradient(const glm::ivec3& pos) const;
	int getAmountOfTrees(const Region& region) const;

	/**
	 * @return Humidity noise in the range [0-1]
	 */
	float getHumidity(int x, int z) const;
	/**
	 * @return Temperature noise in the range [0-1]
	 */
	float getTemperature(int x, int z) const;

	const Biome* getBiome(const glm::ivec3& pos, bool underground = false) const;
};

}
