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

/* Create an interval repetition by duplicating the expression's state machine
    fragments. The miminum is implemented similarly to a threshold repetition.
    Each repetition therafter, up to the maximum, ends with a default transition
    to the final state as well as the transitions to the next repetition. */
const regex::state::base* regex::interval::state(const_states& s, const state::base* final) const
{
    const state::base* a = final;

    // Create (_max - _min) repetitions
    for(size_t i=0; i < (_max - _min); ++i)
    {
	a = _expression->state(s, a);
	((state::literal*)a)->setDefault(final);	// Default transition to final
    }

    // Create _min repetitions of the expression
    for(size_t i=0; i < _min; ++i)
	a = _expression->state(s, a);

    return a;
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

/* Create a threshold repetition by duplicating the child state machine fragments
    When _threshold == 0, generate a single copy of _expression's states using a
     dummy final state. Set the dummy state's default transition to point back
     to the start state, and the start state's default transition to point to
     the true final state.
    When _threshold > 0, concatenate _threshold copies of _expression's states
     and then append a copy of the (_threshold == 0) case.
    In reality this is all performed in reverse, but the idea is the same. */
const regex::state::base* regex::threshold::state(const_states& s, const state::base* final) const
{
    state::literal* f = new state::literal;		// Create a dummy final state
    const state::base* a = _expression->state(s, f);	// Generate the states
    ((state::literal*)a)->setDefault(final);		// Default transition to final
    f->setDefault(a);					// Loop back to start state

    s.push_back(f);		// Add the dummy final state to the container

    for(size_t i=0; i < _threshold; ++i)
	a = _expression->state(s, a);

    return a;
}
