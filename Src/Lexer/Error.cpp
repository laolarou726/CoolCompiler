//
// Created by luoli on 2022/9/14.
//

#include "Error.h"

#include <utility>

namespace CoolCompiler {
    Error::Error(int contextPosition, int lineNumber, const std::string &errorMessage) {
        this->contextPosition = contextPosition;
        this->lineNumber = lineNumber;
        this->errorMessage = errorMessage;
    }

    std::string Error::getErrorMessage() {
        return errorMessage;
    }

    int Error::getContextPosition() const {
        return contextPosition;
    }

    int Error::getLineNumber() const {
        return lineNumber;
    }
} // CoolCompiler