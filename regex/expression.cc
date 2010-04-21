#include "expression.h"

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