//
// Created by luoli on 2022/9/14.
//

#ifndef COOLCOMPILER_ERROR_H
#define COOLCOMPILER_ERROR_H

#include <string>

namespace CoolCompiler {

    class Error {
    private:
        int contextPosition;
        int lineNumber;
        std::string errorMessage;

    public:
        Error(int contextPosition, int lineNumber, const std::string &errorMessage);

        std::string getErrorMessage();

        [[nodiscard]] int getContextPosition() const;

        [[nodiscard]] int getLineNumber() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_ERROR_H
