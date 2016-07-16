
#ifndef SETTINGS_CPP_
#define SETTINGS_CPP_

#include "Settings.h"

namespace tsm {

template <typename T>
Settings<T>::Settings() {
	this->MinFace = MIN_FACE;
	this->MaxLevel = MAX_LEVEL;
	this->MinLevel = MIN_LEVEL;
	this->BoxCacheSize = BOXCACHESIZE;
	this->FindBuffer = FIND_BUFFER;
	this->Interval = INTERVAL;
	this->NmsLimit = NMS_LIMIT;
	this->NmsOverlap = NMS_OVERLAP;
	this->OutputType = OUTPUT_TYPE;
	this->Threshold = THRESHOLD;
	this->Version = VERSION;
}

template <typename T>
Settings<T>::~Settings() {}

} /* namespace tsm */

#endif

