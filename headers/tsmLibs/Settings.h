
#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "DefaultSettings.h"

namespace tsm {

template <typename T>
class Settings {
public:
	int MaxLevel;
	int MinLevel;
	int MinFace;
	T Threshold;
	int BoxCacheSize;
	int NmsLimit;
	float NmsOverlap;
	int Interval;
	int FindBuffer;
	int OutputType;
	int Version;

public:
	Settings();
	virtual ~Settings();
};

} /* namespace tsm */

#endif /* SETTINGS_H_ */
