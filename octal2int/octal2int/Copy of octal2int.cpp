/** octalToUint32
*   Convert C-style, null-terminated strings representing octal numbers
*   to 32-bit unsigned ints, with these error codes:
*	0 = no error, 1 = argument string is NULL, 2 = invalid character, 3 = overflow.
*   
*/


class octalToUint32 
{
public:

	typedef enum ErrorCode {
	    NO_ERROR 			= 0,
		NULL_ARGUMENT		= 1,
		NON_OCTAL_DIGIT 	= 2,
		UINT32_OVERFLOW 	= 3
	};

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

			// If multiplying this value by 8 would overflow a uint32, return the maximum uinit32 value
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


#define DEBUG

// Test program: build only for DEBUG
#ifdef DEBUG

#include <stdio.h>	// defines NULL

int main(int argc, char *argv[]) {
	
	const char * octalStr[8] =	{ "012"
								, "-012"
								, "01234567"
								, "012345678"
								, "17777777777"		// max int32
								, "27777777777"
								, "37777777777"		// max uint32
								, "40000000000"		// max uint32 + 1
	};
	octalToUint32::ErrorCode errorCode = octalToUint32::NO_ERROR;

	printf("Testing: %s\n", argv[0]);
	printf("Converts C-style octal number strings to uint32, with these error codes:\n");
	printf("    0 = no error, 1 = null arg, 2 = invalid character, 3 = overflow.\n\n");
	for (int j = 0; j < 8; j++) {
		unsigned int value = octalToUint32::convert(octalStr[j], errorCode);
		printf("%2d  \"%s\" -> %u, error code %d\n", j, octalStr[j], value, errorCode);
	}
	return 0;
}

#endif

#if 0

unsigned int convertOctalToUint32(const char *octalStr)
{
    static const unsigned int uint32_max = 0xffffffff; 
	static const unsigned int max_over_8 = (uint32_max >> 3) + 1;
	unsigned int value = 0;
	if (octalStr != NULL) {
		for ( ; *octalStr; octalStr++) {
			int octalDigit = *octalStr - '0';

			// If the string contains any char that is not an octal digit, return 0 (like atoi)
			if ((octalDigit & 7) != octalDigit)  {  // faster than: (octalDigit < 0 || octalDigit > 7)
				return 0;
			}

			// If multiplying this value by 8 would overflow a uint32, return the maximum uinit32 value
			if (value > max_over_8) {
				return uint32_max;
			}

			// Multiply the current value by 8 and add the value of the next octal digit.
			value = (value << 3) + octalDigit;
		}
	}
	return value;
}
#endif