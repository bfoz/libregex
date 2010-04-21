#ifndef	REGEX_EXPRESSION_H
#define	REGEX_EXPRESSION_H

#include <list>
#include <string>

#include "state.h"

namespace regex
{
    class expression;	// Abstract base class for syntax specification classes
    class literal;	// A literal sequence of characters to match

    typedef std::list<expression*> expressions;	// A container for multiple regular expressions
}

class regex::expression
{
public:
    // Convert the expression into one or more matching states
    //  The caller passes a container to add the new states to, as well as the state
    //   object to transition to after the last character.
    //  Returns the first of the new states
    virtual const state::base* state(const_states& s, const state::base* final) const =0;
};

class regex::literal : public expression
{
    std::string _literal;

public:
    literal(const char* s) : _literal(s) {}
    literal(std::string& s) : _literal(s) {}

    virtual const state::base* state(const_states& s, const state::base* final) const;
};

#endif	// REGEX_EXPRESSION_H
