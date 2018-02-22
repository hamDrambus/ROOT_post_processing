#include "SingleRunResults.h"

SingleRunResults::SingleRunResults(SingleRunData *_of_what)
{
	curr_area = _of_what ? _of_what->getArea():curr_area;
	setValid(kTRUE);
	_current_status = Status::Ok;
}

SingleRunResults::Status SingleRunResults::getStatus(void) const
{	return _current_status;}

Bool_t SingleRunResults::isValid(void) const
{	return is_valid;}
void SingleRunResults::setValid(Bool_t val)
{	is_valid = val;}