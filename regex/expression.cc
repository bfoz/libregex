#include "expression.h"

// Merge the state fragments for each expression in the alternation
const regex::state::base* regex::alternation::state(const_states& s, const state::base* final) const
{
    expressions::const_iterator i = _expressions.begin();
    const state::base* j = (*i)->state(s, final);
    for(++i; i != _expressions.end(); ++i)
	j = state::merge(s, *j, *(*i)->state(s, final));
    return j;
}

// Concatenate the state machines for multiple expressions
const regex::state::base* regex::concatenation::state(const_states& s, const state::base* final) const
{
    expressions::const_reverse_iterator i = _expressions.rbegin();
    for(; i != _expressions.rend(); ++i)
    {
	final = (*i)->state(s, final);
	s.push_back(final);
    }
    return final;
}

// Convert a string literal into a series of states, each matching a single character
//  Returns the state that matches the first character in the string literal
const regex::state::base* regex::literal::state(const_states& s, const state::base* final) const
{
    std::string::const_reverse_iterator i = _literal.rbegin();
    for(; i != _literal.rend(); ++i)
    {
	final = new state::literal(*i, final);
	s.push_back(final);
    }
    return final;
}