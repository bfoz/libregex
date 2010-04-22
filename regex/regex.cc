#include "regex.h"
#include "state.h"

// Find the left-most shortest match in the string
bool regex::find(const std::string& s, expression& re)
{
    state::final    final;
    const_states    states(1, &final);
    
    const state::base* next = re.state(states, &final);
    
    std::string::const_iterator i = s.begin();
    while( (i != s.end()) && next && (next != &final) && (next = next->next(*i)) )
	++i;

    // Return true if a match state is encountered
    return next == &final;
}

bool regex::match(const std::string& s, const expression& re)
{
    state::final    final;
    const_states    states;
    
    const state::base* next = re.state(states, &final);
    
    std::string::const_iterator i = s.begin();
    while( (i != s.end()) && next && (next = next->next(*i)) )
	++i;
    
    // Return true if a match state is encountered at the last character
    return (i == s.end()) && (next == &final);
}
