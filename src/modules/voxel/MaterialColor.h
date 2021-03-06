/**
 * @file
 */

#pragma once

#include "voxel/polyvox/Voxel.h"
#include "core/Color.h"
#include "io/File.h"
#include <vector>
#include "core/Random.h"

namespace voxel {

// this size must match the color uniform size in the shader
typedef std::vector<glm::vec4> MaterialColorArray;
typedef std::vector<uint8_t> MaterialColorIndices;

extern bool initDefaultMaterialColors();
extern bool initMaterialColors(const io::FilePtr& paletteFile, const io::FilePtr& luaFile);
extern const MaterialColorArray& getMaterialColors();
/**
 * @brief Get all known material color indices for the given VoxelType
 * @param type The VoxelType to get the indices for
 * @return Indices to the MaterialColorArray for the given VoxelType
 */
extern const MaterialColorIndices& getMaterialIndices(VoxelType type);
extern Voxel createRandomColorVoxel(VoxelType type);
extern Voxel createRandomColorVoxel(VoxelType type, core::Random& random);
/**
 * @brief Creates a voxel of the given type with the fixed colorindex that is relative to the
 * valid color indices for this type.
 */
extern Voxel createColorVoxel(VoxelType type, uint32_t colorIndex);

class RandomVoxel {
private:
	const MaterialColorIndices& indices;
	const core::Random& random;
	const VoxelType type;
	int _sameCount;
	mutable int _amount = 1;
	mutable uint8_t _currentIndex = 0u;
public:
	RandomVoxel(VoxelType _type, const core::Random& _random, int sameCount = 3) :
			indices(getMaterialIndices(_type)), random(_random), type(_type), _sameCount(sameCount) {
	}

	inline operator Voxel() const {
		if (indices.size() == 1) {
			return Voxel(type, indices.front());
		}
		if (_amount == 1) {
			auto i = random.randomElement(indices.begin(), indices.end());
			_currentIndex = *i;
		}
		++_amount;
		if (_amount >= _sameCount) {
			_amount = 1;
		}
		return Voxel(type, _currentIndex);
	}
};

}
