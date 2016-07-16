
#ifndef ERRORHANDLER_CPP_
#define ERRORHANDLER_CPP_

#include "ErrorHandler.h"

namespace tsm {

ErrorHandler::ErrorHandler() {
	this->code = 0;
}

ErrorHandler::~ErrorHandler() {
}

void ErrorHandler::error(int code){

	this->code = code;
	throw code;
}

} /* namespace tsm */

#endif /* ERRORHANDLER_CPP_ */

