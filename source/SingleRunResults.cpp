#include "SingleRunResults.h"

SingleRunResults::SingleRunResults(SingleRunData *_of_what)
{
	curr_area = _of_what ? _of_what->getArea():curr_area;
	setValid(true);
	_current_status = Status::Ok;
}

SingleRunResults::Status SingleRunResults::getStatus(void) const
{	return _current_status;}

bool SingleRunResults::isValid(void) const
{	return is_valid;}
void SingleRunResults::setValid(bool val)
{	is_valid = val;}