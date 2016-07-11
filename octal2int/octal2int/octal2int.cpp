/** OctalToUint32
*   Evaluate C-style, null-terminated strings representing positive octal numbers
*   as 32-bit unsigned ints, with these error codes:
*	0 = no error, 1 = argument string is NULL, 2 = invalid character, 3 = overflow.
*   The evaluation iterates through the input string only once and from left to right.
*   Consequently, an input string such as "7777777777777777e99" will get the "overflow"
*   rathern than the "non-octal-digit" error code.  Input strings are not required to 
*   begin with 0.  We assume that an "unsigned int" is 32 bits on the target platform.
*   Of course this code would be divided into a .cpp and a header file in an real project.
*
*   Author: Sprax Lines
*/


class OctalToUint32 
{
public:

	typedef enum tagErrorCode {
	    NO_ERROR 			= 0,
		NULL_ARGUMENT		= 1,
		NON_OCTAL_DIGIT 	= 2,
		UINT32_OVERFLOW 	= 3
	} ErrorCode;

	static const unsigned int uint32_max = 0xffffffff;	// Let's not depend on limits.h

	static unsigned int convert(const char *octalStr, ErrorCode & errorCode)
	{
		errorCode = NO_ERROR;
		if (octalStr == 0) {							// 0 is NULL, as defined in stdio.h, etc.
			errorCode = NULL_ARGUMENT;
			return 0;
		} 
		unsigned int value = 0;
		for ( ; *octalStr; octalStr++) {
			int octalDigit = *octalStr - '0';

			// If the string contains any char that is not an octal digit, return 0 (like atoi)
			if ((octalDigit & 7) != octalDigit)  {  // faster than: (octalDigit < 0 || octalDigit > 7)
				errorCode = NON_OCTAL_DIGIT;
				return 0;
			}

			// If multiplying this value by 8 would overflow a uint32, return the maximum uint32 value
			// and set the error code to indicate overflow:
			if (value >= ((uint32_max >> 3) + 1)) {
				errorCode = UINT32_OVERFLOW;
				return uint32_max;
			}

			// Multiply the current value by 8 and add the value of the next octal digit.
			value = (value << 3) + octalDigit;
		}
		return value;
	}
};


// Test program:

#define UNIT_TEST
#ifdef  UNIT_TEST

#include <stdio.h>	// Assumes the unit test environment has access to a standard C library. 

int main(int argc, char *argv[]) {
	
	const char * octalStr[9] =	{ "012"
								, "-012"
								, "01234567"
								, "012345678"
								, "17777777777"		// max int32
								, "27777777777"
								, "37777777777"		// max uint32
								, "40000000000"		// max uint32 + 1
								, "77777777777e23"	// overflows before the non-octal digit 'e'
	};
	OctalToUint32::ErrorCode errorCode = OctalToUint32::NO_ERROR;

	printf("Testing: %s\n", argv[0]);
	printf("Converts C-style octal number strings to uint32, with these error codes:\n");
	printf("    0 = no error, 1 = null arg, 2 = invalid character, 3 = overflow.\n\n");
	for (int j = 0; j < 9; j++) {
		unsigned int value = OctalToUint32::convert(octalStr[j], errorCode);
		printf("%2d  \"%s\" -> %u,  error code %d\n", j, octalStr[j], value, errorCode);
	}
	return 0;
}

#endif	// UNIT_TEST
