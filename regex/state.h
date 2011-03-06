#ifndef	REGEX_STATE_H
#define	REGEX_STATE_H

#include <set>
#include <map>

namespace regex
{
    namespace state
    {
	class base;	// Base class for all state objects
	class final;	// A terminating state
	class literal;	// A state that transitions on single-character input
    }

    typedef std::set<state::base*>		states;		// A container for state objects
    typedef std::set<const state::base*>	const_states;	// A container for const state objects

    namespace state
    {
	base* merge(const_states&, const base&, const base&);		// Merge two states
	literal* merge(const_states&, const literal&, const literal&);	// Merge two literal states
	literal* merge(const_states&, const literal&, const final&);	// Merge a literal state with a final state
    }
}

class regex::state::base
{
public:
    virtual const base*	getDefault() const	{ return NULL;	}
    virtual bool	isFinal() const		{ return false;	}
    virtual const base* next(char) const =0;
};

class regex::state::literal : public base
{
    typedef std::map<char, const base*>	transitions_type;

    const base*	_default;
    transitions_type transitions;

public:
    typedef transitions_type::const_iterator	const_iterator;
    typedef transitions_type::iterator	iterator;

    literal(const base* def=NULL) : _default(def) {}
    literal(char c, const base* target) : _default(NULL)
    {
	insert(c, target);
    }

    const_iterator begin() const { return transitions.begin();	 }
    const_iterator end() const { return transitions.end();	 }

    void insert(char c, const base* target)
    {
	transitions[c] = target;
    }

    virtual const base* next(char c) const
    {
	transitions_type::const_iterator i(transitions.find(c));
	if( transitions.end() == i )
	    return NULL;
	return i->second;
    }

    virtual const base* getDefault() const	{ return _default;	}
    void setDefault(base* def)		{ _default = def;	}
};

class regex::state::final : public base
{
public:
    virtual bool isFinal() const	{ return true;	}
    virtual base* next(char) const { return NULL; }
};

#endif	// REGEX_STATE_H
