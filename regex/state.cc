#include "state.h"

namespace regex
{
    state::base* state::merge(const_states& s, const base& left, const base& right)
    {
	// Check for identity
	if( &left == &right )
	    return const_cast<base*>(&left);

	// Check for a state::final
	if( left.isFinal() )
	    return merge(s, (literal&)right, (final&)left);
	else if( right.isFinal() )
	    return merge(s, (literal&)left, (final&)right);

	return merge(s, (literal&)left, (literal&)right);
    }

    state::literal* state::merge(const_states& s, const literal& s1, const literal& s2)
    {
	literal* m = new literal;

	// Add the new state to the state list
	s.insert(m);

	// Merge the two transition maps, handling any duplicates
	literal::const_iterator i = s1.begin();
	for(; i != s1.end(); ++i)
	{
	    if( s2.next(i->first) )	// Does s2 have a duplicate transition?
	    {
		// Make a new state to handle the alternation
		m->insert(i->first, merge(s, *(i->second), *(s2.next(i->first))));
	    }
	    else
		m->insert(i->first, i->second);
	}

	// Merge all remaining transitions in s2
	for(i = s2.begin(); i != s2.end(); ++i)
	    if( !m->next(i->first) )
		m->insert(i->first, i->second);

	return m;
    }

    state::literal* state::merge(const_states& s, const literal& s1, const final& s2)
    {
	literal* m = new literal(&s2);

	// Add the new state to the state list
	s.insert(m);

	// Copy all transitions from the literal state
	literal::const_iterator i = s1.begin();
	for(; i != s1.end(); ++i)
	    m->insert(i->first, i->second);

	return m;
    }
}
