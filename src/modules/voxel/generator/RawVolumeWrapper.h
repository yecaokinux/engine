/**
 * @file
 */

#pragma once

#include "voxel/polyvox/RawVolume.h"

namespace voxel {
namespace generate {

/**
 * @brief A wrapper for a RawVolume that performs a sanity check for the setVoxel call.
 */
class RawVolumeWrapper {
private:
	RawVolume* _volume;
	const Region& _region;

public:
	RawVolumeWrapper(voxel::RawVolume* volume) :
			_volume(volume), _region(volume->getEnclosingRegion()) {
	}

	inline operator RawVolume& () const {
		return *_volume;
	}

	inline operator const RawVolume& () const {
		return *_volume;
	}

	inline operator RawVolume* () const {
		return _volume;
	}

	inline operator const RawVolume* () const {
		return _volume;
	}

	inline const Voxel& getVoxel(const glm::ivec3& pos) const {
		return _volume->getVoxel(pos.x, pos.y, pos.z);
	}

	inline const Voxel& getVoxel(int x, int y, int z) const {
		return _volume->getVoxel(x, y, z);
	}

	inline bool setVoxel(const glm::ivec3& pos, const Voxel& voxel) {
		return setVoxel(pos.x, pos.y, pos.z, voxel);
	}

	/**
	 * @return @c false if the voxel was not placed because the given position is outside of the valid region, @c
	 * true if the voxel was placed in the region.
	 * @note The return values have a different meaning as for the wrapped RawVolume.
	 */
	inline bool setVoxel(int x, int y, int z, const Voxel& voxel) {
		if (!_region.containsPoint(x, y, z)) {
			return false;
		}
		_volume->setVoxel(x, y, z, voxel);
		return true;
	}

	inline bool setVoxels(int x, int z, const Voxel* voxels, int amount) {
		for (int y = 0; y < amount; ++y) {
			if (_region.containsPoint(x, y, z)) {
				_volume->setVoxel(x, y, z, voxels[y]);
			}
		}
		return true;
	}
};

}
}
