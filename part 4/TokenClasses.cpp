
#include "tokenClasses.hpp"

// I'm not sure if next line works with other than GNU compiler
#include <cxxabi.h>

using namespace std;

namespace ensc251 {
//new may or may not need

//TokenSP int_assignment_exp();
//TokenSP shift_exp();
//TokenSP int_div_exp();

// global variables
std::vector<TokenSP> tokenObjectPs; // objects for abstract syntax tree
std::size_t tracker{0}; // tracker index

char *demangle(const char *typeName) {
	int status;
	// I'm not sure if the below line works with compilers other than GNU
	return abi::__cxa_demangle(typeName, 0, 0, &status);
}

TokenSP postfix_exp()
{// TODO: ***** Complete this function
	int old_tracker = tracker;
	    TokenSP lowerNodeP;
	    TokenSP upperNodeP;
	    if((upperNodeP = tokenObjectPs[tracker]->process_id()))
	    {
	        if((lowerNodeP = tokenObjectPs[tracker]->process_postfix_operator()))
	        {
	            upperNodeP->add_childP(lowerNodeP);
	            lowerNodeP = upperNodeP;
	            return lowerNodeP;
	        }
	    }

	    tracker = old_tracker;
	auto subTreeP{tokenObjectPs[tracker]->process_primary_exp()};
	// TODO: ***** Complete this function
	if(!subTreeP)
		return nullptr;
	return subTreeP;
}

TokenSP int_postfix_exp()
{// TODO: ***** Complete this function
	int old_tracker = tracker;

TokenSP lowerNodeP;
TokenSP upperNodeP;
	    if((upperNodeP = tokenObjectPs[tracker]->process_int_id()))
	    {
	        if((lowerNodeP = tokenObjectPs[tracker]->process_postfix_operator()))
	        {
	            upperNodeP->add_childP(lowerNodeP);
	            lowerNodeP = upperNodeP;
	            return lowerNodeP;
	        }
	    }

	    tracker = old_tracker;
	auto subTreeP = tokenObjectPs[tracker]-> process_int_primary_exp();
	// TODO: ***** Complete this function
	if(!subTreeP)
		return nullptr;
//};
	return subTreeP;
}

TokenSP comp_exp()
{
	std::size_t old_tracker{tracker};

	TokenSP tObjP{tokenObjectPs[tracker]->process_int_comp()};


	if(!tObjP) {
	tracker = old_tracker;
	tObjP = postfix_exp();
	}
	return tObjP;
}

TokenSP int_comp_exp()
{
	std::size_t old_tracker{tracker};

	TokenSP tObjP{tokenObjectPs[tracker]->process_int_comp()};

	if(!tObjP) {
		tracker = old_tracker;
		tObjP = int_postfix_exp();
	}
	return tObjP;
}

TokenSP int_div_exp()
{

	TokenSP lowerNodeP{int_comp_exp()};
	if (lowerNodeP) {
		for (TokenSP upperNodeP{nullptr};
				(upperNodeP = tokenObjectPs[tracker]->advance_past_div_operator()) ||
				(upperNodeP = tokenObjectPs[tracker]->advance_past_mod_operator());)
			if(TokenSP ueTreeP{int_comp_exp()}) {
				upperNodeP->add_childP(lowerNodeP);
				upperNodeP->add_childP(ueTreeP);
				lowerNodeP = upperNodeP;
			}
			else
				return nullptr;
	}
	return lowerNodeP;
}

TokenSP div_exp()
{
	std::size_t old_tracker{tracker};

	TokenSP lowerNodeP{comp_exp()};
	if (lowerNodeP) {
		while (TokenSP upperNodeP{tokenObjectPs[tracker]->advance_past_div_operator()}) {
			if(TokenSP ueTreeP{comp_exp()}) {
				upperNodeP->add_childP(lowerNodeP);
				upperNodeP->add_childP(ueTreeP);
				lowerNodeP = upperNodeP;
			}
			else
				return nullptr;
		}
	}
	else {
		tracker = old_tracker;
		lowerNodeP = int_div_exp();
	}

	return lowerNodeP;
}
// TODO: ***** Add more functions around here somewhere *****
TokenSP additive_exp(){
    TokenSP subTreeP = nullptr;
    TokenSP lowerNodeP = nullptr;
    TokenSP upperNodeP = nullptr;

    if ((lowerNodeP = div_exp())) {
        while ((upperNodeP = tokenObjectPs[tracker]->advance_past_additive_operator()))
        {
            if(!(subTreeP = div_exp()))
                    return nullptr;
            upperNodeP->add_childP(lowerNodeP);
            upperNodeP->add_childP(subTreeP);
            lowerNodeP = upperNodeP;
        }
    }
    return lowerNodeP;
}

TokenSP int_additive_exp(){
    TokenSP subTreeP = nullptr;
    TokenSP lowerNodeP = nullptr;
    TokenSP upperNodeP = nullptr;

    if ((lowerNodeP = int_div_exp())) {
        while ((upperNodeP = tokenObjectPs[tracker]->advance_past_additive_operator()))
        {
            if(!(subTreeP = int_div_exp()))
                    return nullptr;
            upperNodeP->add_childP(lowerNodeP);
            upperNodeP->add_childP(subTreeP);
            lowerNodeP = upperNodeP;
        }
    }
    return lowerNodeP;
}


TokenSP shift_exp()
{// TODO: ***** Fix and complete this function
	    TokenSP subTreeP = nullptr;
	    TokenSP lowerNodeP = nullptr;
	    TokenSP upperNodeP = nullptr;
	  //TokenSP lowerNodeP{int_div_exp()};    --not needed

	if ((lowerNodeP = int_additive_exp())) {
		while (TokenSP upperNodeP{tokenObjectPs[tracker]->advance_past_shift_operator()})
		{
			// TODO: ***** Fix and complete this code
			if(!(subTreeP = int_additive_exp()))
			                    return nullptr;
			            upperNodeP->add_childP(lowerNodeP);
			            upperNodeP->add_childP(subTreeP);
			            lowerNodeP = upperNodeP;
//			}
//			    else   -extra - was removed
	//			return nullptr;   -removed
		}
	}
	return lowerNodeP;
}

TokenSP ternary_exp()
{
	std::size_t old_tracker{tracker};
	TokenSP subTreeP;
	// TODO: ***** Complete this function


	if((subTreeP = tokenObjectPs[tracker]->process_id()))
	    {
	        TokenSP tObjP;
	        if((tObjP = tokenObjectPs[tracker]->advance_past_conditional_operator()))
	        {
	            tObjP->add_childP(subTreeP);
	            subTreeP = assignment_exp();
	            if(subTreeP)
	            {
	                tObjP->add_childP(subTreeP);
	                if(tokenObjectPs[tracker]->process_punctuator(":"))
	                {
	                    subTreeP = ternary_exp();
	                    if(subTreeP)
	                    {
	                        tObjP->add_childP(subTreeP);
	                        return tObjP;
	                    }
	                }
	            }
	            return nullptr;
	        }
	    }


	tracker = old_tracker;
	subTreeP = shift_exp();

	if(!subTreeP)
	{
		tracker = old_tracker;
		// TODO: ***** Fix and complete this function
        subTreeP = additive_exp();
	//	subTreeP = div_exp();   //not sure which line is needed between 273 and 274
		if(!subTreeP)
			return nullptr;
	}
	return subTreeP;
}

TokenSP int_ternary_exp()
{
	std::size_t old_tracker{tracker};
	TokenSP subTreeP;

	if((subTreeP = tokenObjectPs[tracker]->process_id()))
	{
		if(TokenSP tObjP{tokenObjectPs[tracker]->advance_past_conditional_operator()})
		{
			tObjP->add_childP(subTreeP);
			subTreeP = int_assignment_exp();
			if(subTreeP)
			{
				tObjP->add_childP(subTreeP);
				if(tokenObjectPs[tracker]->process_punctuator(":"))
				{
					subTreeP = int_ternary_exp();
					if(subTreeP)
					{
						tObjP->add_childP(subTreeP);
						return tObjP;
					}
				}
			}
			return nullptr;
		}
	}
	tracker = old_tracker;
	return shift_exp();
}

TokenSP assignment_exp()
{
	std::size_t old_tracker{tracker};

	// int_id int_assignment_operator int_assignment_exp
	TokenSP subTreeP{tokenObjectPs[tracker]->process_int_id()};
	if(subTreeP)
	{
		if(TokenSP tObjP{tokenObjectPs[tracker]->advance_past_int_assignment_operator()})
		{
			tObjP->add_childP(subTreeP);
			if((subTreeP = int_assignment_exp()))
			{
				tObjP->add_childP(subTreeP);
				return tObjP;
			}
			else
				return nullptr;
		}
	}

	// production: id gen_assignment_operator assignment_exp
	tracker = old_tracker;
	if ((subTreeP = tokenObjectPs[tracker]->process_id()))
	{
		if(TokenSP tObjP{tokenObjectPs[tracker]->advance_past_gen_assignment_operator()})
		{
			tObjP->add_childP(subTreeP);
			if((subTreeP = assignment_exp()))
			{
				tObjP->add_childP(subTreeP);
				return tObjP;
			}
			else
				// we have assignment operator, but not valid assignment expression,
				//	so there must be an error and it should be reported.
				return nullptr;
		}
		// no assignment operator, so this is not this production

		// start production:  ternary_exp
		if(tokenObjectPs[tracker]->has_string_value(";"))
			// we have already processed an id, which satisfies the ternary_exp production.
			return subTreeP;
	}

	// production:  ternary_exp
	tracker = old_tracker;
	return ternary_exp();
}

TokenSP int_assignment_exp()
{
	std::size_t old_tracker{tracker};

	TokenSP subTreeP{tokenObjectPs[tracker]->process_int_id()};
	if(subTreeP)
	{
		if(TokenSP tObjP{tokenObjectPs[tracker]->advance_past_gen_assignment_operator()})
		{
			tObjP->add_childP(subTreeP);
			if((subTreeP = assignment_exp()))
			{
				tObjP->add_childP(subTreeP);
				return tObjP;
			}
			else
				return nullptr;
		}

		if(TokenSP tObjP{tokenObjectPs[tracker]->advance_past_int_assignment_operator()})
		{
			tObjP->add_childP(subTreeP);
			if((subTreeP = int_assignment_exp()))
			{
				tObjP->add_childP(subTreeP);
				return tObjP;
			}
			else
				return nullptr;
		}
	}

	// production:  int_ternary_exp
	tracker = old_tracker;
	return int_ternary_exp();
}

TokenSP stat()
{
	if(auto subTreeP{tokenObjectPs[tracker]->process_declaration()})
		return subTreeP;
	else
		return assignment_exp();
}

// because the below function deals with ";", it is a bit different than the pattern seen elsewhere.
TokenSP stat_list()
{
	if(auto lowerNodeP{stat()}) {

		while (tokenObjectPs[tracker]->has_string_value(";")) {
			tracker++;
			if (tracker == tokenObjectPs.size())
				return lowerNodeP;
			TokenSP upperNodeP{tokenObjectPs[tracker - 1]};

			if(TokenSP statTreeP{stat()}) {
				upperNodeP->add_childP(lowerNodeP);
				upperNodeP->add_childP(statTreeP);
				lowerNodeP = upperNodeP;			}
			else
				throw ensc251::TokenException(tokenObjectPs[tracker]);
		}
	}
	throw ensc251::TokenException(tokenObjectPs[tracker]);
}

TokenSP recursive_parser()
{
	if (tokenObjectPs.size())
		return stat_list();
	else
		throw ensc251::TokenException();
}

}
