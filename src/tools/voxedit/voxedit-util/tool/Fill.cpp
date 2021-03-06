#include "Fill.h"

namespace voxedit {
namespace tool {

bool fill(voxel::RawVolume& target, const glm::ivec3& position, const Axis axis, const voxel::Voxel& voxel, bool overwrite, voxel::Region* modifiedRegion) {
	const voxel::Region& region = target.getRegion();
	int zStart = region.getLowerZ();
	glm::ivec3 modifiedMins(std::numeric_limits<int>::max());
	glm::ivec3 modifiedMaxs(std::numeric_limits<int>::min());
	int cnt = 0;
	if ((axis & Axis::Z) != Axis::None) {
		zStart = position.y;
	}
	for (int32_t z = zStart; z <= region.getUpperZ(); ++z) {
		int yStart = region.getLowerY();
		if ((axis & Axis::Y) != Axis::None) {
			yStart = position.y;
		}
		for (int32_t y = yStart; y <= region.getUpperY(); ++y) {
			int xStart = region.getLowerX();
			if ((axis & Axis::X) != Axis::None) {
				xStart = position.x;
			}
			for (int32_t x = xStart; x <= region.getUpperX(); ++x) {
				if (overwrite || isAir(target.getVoxel(x, y, z).getMaterial())) {
					if (target.setVoxel(x, y, z, voxel)) {
						++cnt;
						modifiedMins.x = glm::min(modifiedMins.x, x);
						modifiedMins.y = glm::min(modifiedMins.y, y);
						modifiedMins.z = glm::min(modifiedMins.z, z);

						modifiedMaxs.x = glm::max(modifiedMaxs.x, x);
						modifiedMaxs.y = glm::max(modifiedMaxs.y, y);
						modifiedMaxs.z = glm::max(modifiedMaxs.z, z);
					}
				}
				if ((axis & Axis::X) != Axis::None) {
					break;
				}
			}
			if ((axis & Axis::Y) != Axis::None) {
				break;
			}
		}
		if ((axis & Axis::Z) != Axis::None) {
			break;
		}
	}
	if (cnt <= 0) {
		return false;
	}
	if (modifiedRegion != nullptr) {
		*modifiedRegion = voxel::Region(modifiedMins, modifiedMaxs);
	}
	return true;
}

}
}
