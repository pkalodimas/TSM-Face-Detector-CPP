
#ifndef ERRORHANDLER_H_
#define ERRORHANDLER_H_

#include <string>
#include "ErrorCodes.h"

namespace tsm {

class ErrorHandler {
public:
	int code;
public:
	ErrorHandler();
	virtual ~ErrorHandler();

	void error(int code);
};

} /* namespace tsm */

#endif /* ERRORHANDLER_H_ */
