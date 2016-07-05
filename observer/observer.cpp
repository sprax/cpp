// observer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"




// main.cpp

class meeting;
#include "setup_class.h"
#include "use_class.h"

int main()
{
	meeting * a_meeting = setup_class::setup();

	use_class::use(a_meeting);

	return 0;
}
