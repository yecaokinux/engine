/**
 * @file
 */

#include "Filesystem.h"
#include "core/Var.h"
#include "core/Log.h"
#include <SDL.h>

namespace io {

Filesystem::Filesystem() :
		_threadPool(1, "IO") {
}

void Filesystem::init(const std::string& organisation, const std::string& appname) {
	_organisation = organisation;
	_appname = appname;

	char *path = SDL_GetBasePath();
	if (path == nullptr) {
		_basePath = "";
	} else {
		_basePath = path;
		SDL_free(path);
	}

	char *prefPath = SDL_GetPrefPath(_organisation.c_str(), _appname.c_str());
	if (prefPath == nullptr) {
		_homePath = "";
	} else {
		_homePath = prefPath;
		SDL_free(prefPath);
	}
	Log::debug("basepath: %s", _basePath.c_str());
	Log::debug("homepath: %s", _homePath.c_str());
	core::Var::get(cfg::AppHomePath, _homePath.c_str(), core::CV_READONLY | core::CV_NOPERSIST);
	core::Var::get(cfg::AppBasePath, _basePath.c_str(), core::CV_READONLY | core::CV_NOPERSIST);
}

bool Filesystem::isRelativeFilename(const std::string& name) const {
	const size_t size = name.size();
#ifdef __WINDOWS__
	if (size < 3) {
		return true;
	}
	// TODO: hm... not cool and most likely not enough
	return name[1] != ":";
#else
	if (size == 0) {
		return false;
	}
	return name[0] != '/';
#endif
}

io::FilePtr Filesystem::open(const std::string& filename, FileMode mode) const {
	if (mode == FileMode::Write && !isRelativeFilename(filename)) {
		return std::make_shared<io::File>(filename, mode);
	}
	if (io::File(filename, FileMode::Read).exists()) {
		Log::debug("loading file %s from current working dir", filename.c_str());
		return std::make_shared<io::File>(filename, mode);
	}
	const std::string homePath = _homePath + filename;
	if (io::File(homePath, FileMode::Read).exists()) {
		Log::debug("loading file %s from %s", filename.c_str(), _homePath.c_str());
		return std::make_shared<io::File>(homePath, mode);
	}
	Log::debug("loading file %s from %s (doesn't exist at %s)", filename.c_str(), _basePath.c_str(), homePath.c_str());
	return std::make_shared<io::File>(_basePath + filename, mode);
}

std::string Filesystem::load(const std::string& filename) const {
	const io::FilePtr& f = open(filename);
	return f->load();
}

bool Filesystem::write(const std::string& filename, const uint8_t* content, size_t length) {
	io::File f(_homePath + filename, FileMode::Write);
	createDir(f.path());
	return f.write(content, length) == static_cast<long>(length);
}

bool Filesystem::write(const std::string& filename, const std::string& string) {
	const uint8_t* buf = reinterpret_cast<const uint8_t*>(string.c_str());
	return write(filename, buf, string.size());
}

bool Filesystem::syswrite(const std::string& filename, const uint8_t* content, size_t length) {
	io::File f(filename, FileMode::Write);
	createDir(f.path());
	return f.write(content, length) == static_cast<long>(length);
}

bool Filesystem::syswrite(const std::string& filename, const std::string& string) {
	const uint8_t* buf = reinterpret_cast<const uint8_t*>(string.c_str());
	return syswrite(filename, buf, string.size());
}

}
