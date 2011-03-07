#ifndef	REGEX_H
#define	REGEX_H

#include <list>
#include <string>

#include "expression.h"

namespace regex
{
    // Find the left-most match in the string
    bool find(const std::string& s, const expression& re);

    // Read from the input stream until EOS or a match is found
    bool find(std::istream* is, expression& re);

    // Try to match the expression against the entire string
    bool match(const std::string& s, const expression& re);

    // Try to match the expression against the input stream
    bool match(std::istream& is, expression& re);


    // Find the left-most matching expressions in the string
    expressions* find(const std::string& s, expressions& regexs);

    // Read from the input stream until EOS or a match is found
    expressions* find(std::istream& is, expressions& regexs);

    // Return the expressions that match the entire input string
    expressions* match(const std::string& s, expressions& regexs);

    // Return the expressions that match the input stream
    expressions* match(std::istream& is, expressions& regexs);
}

#endif	// REGEX_H
