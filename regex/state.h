#ifndef	REGEX_STATE_H
#define	REGEX_STATE_H

#include <list>
#include <map>

namespace regex
{
    namespace state
    {
	class base;	// Base class for all state objects
	class final;	// A terminating state
	class literal;	// A state that transitions on single-character input
    }

    typedef std::list<state::base*>		states;		// A container for state objects
    typedef std::list<const state::base*>	const_states;	// A container for const state objects
}

class regex::state::base
{
public:
    virtual const base* next(char) const =0;
};

class regex::state::literal : public base
{
    typedef std::map<char, const base*>	transition_type;

    transition_type transitions;

public:
    literal(char c, const base* target)
    {
	insert(c, target);
    }

    void insert(char c, const base* target)
    {
	transitions[c] = target;
    }

    virtual const base* next(char c) const
    {
	transition_type::const_iterator i(transitions.find(c));
	if( transitions.end() == i )
	    return NULL;
	return i->second;
    }
};

class regex::state::final : public base
{
public:
    virtual base* next(char) const { return NULL; }
};

#endif	// REGEX_STATE_H
