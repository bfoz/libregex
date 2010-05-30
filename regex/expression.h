#ifndef	REGEX_EXPRESSION_H
#define	REGEX_EXPRESSION_H

#include <list>
#include <string>

#include "state.h"

namespace regex
{
    class expression;	// Abstract base class for syntax specification classes
    class alternation;	// Alternation of multiple expressions
    class concatenation;// Concatenation of multiple expressions
    class interval;	// Matches N-M repetitions of an expression
    class literal;	// A literal sequence of characters to match
    class threshold;	// Matches at least N repetitions of an expression

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

class regex::alternation : public expression
{
    expressions	_expressions;

public:
    alternation(expressions& regexs) : _expressions(regexs) {}
    alternation(expression& regex) : _expressions(1, &regex) {}
    alternation(expression& regex1, expression& regex2) : _expressions(1, &regex1)
    {
	_expressions.push_back(&regex2);
    }

    void push_back(expression& regex)	{ _expressions.push_back(&regex);   }

    virtual const state::base* state(const_states& s, const state::base* final) const;
};

class regex::concatenation : public expression
{
    expressions	_expressions;

public:
    concatenation(expressions& regexs) : _expressions(regexs) {}
    concatenation(expression& regex) : _expressions(1, &regex) {}
    concatenation(expression& regex1, expression& regex2) : _expressions(1, &regex1)
    {
	_expressions.push_back(&regex2);
    }

    void push_back(expression& regex)	{ _expressions.push_back(&regex);   }

    virtual const state::base* state(const_states& s, const state::base* final) const;
};

class regex::interval : public expression
{
    expression*	_expression;
    size_t	_min, _max;

public:
    interval(expression& re, size_t minmax) : _expression(&re), _min(minmax), _max(minmax) {}
    interval(expression& re, size_t min, size_t max) : _expression(&re), _min(min), _max(max)
    {
	if( _max < _min )
	    std::swap(_min, _max);
    }

    virtual const state::base* state(const_states& s, const state::base* final) const;
};

class regex::literal : public expression
{
    std::string _literal;

public:
    literal(const char* s) : _literal(s) {}
    literal(std::string& s) : _literal(s) {}

    virtual const state::base* state(const_states& s, const state::base* final) const;
};

class regex::threshold : public expression
{
    expression*	_expression;
    size_t	_threshold;

public:
    threshold(expression& re, size_t num) : _expression(&re), _threshold(num) {}

    virtual const state::base* state(const_states& s, const state::base* final) const;
};

#endif	// REGEX_EXPRESSION_H
