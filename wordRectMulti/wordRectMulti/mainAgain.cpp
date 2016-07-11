#include <stdio.h>
#include "sets.hpp"

#include <iostream>
#include "wordRectMain.hpp"


int fun()
{
	printf("\ncrap");
	return 0;
}

void implicitReturnValue()
{
	printf("\n return value of fun %d\n", fun());
}


int main (int argc, const char * argv[]) 
{
    // insert code here...
    printf("Hello, World!\n");
	fun();
	implicitReturnValue();
	//Sets sets = new Sets();
	
	size_t size = 12;
	//hash_set<int> hsi;
	//hsi.insert(0);
	//hsi.erase(0);
	//cout << hsi.size() << endl;
	
	std::cout << "Couted:  " << size << std::endl;
	
	printf("size is (%u)\n", (unsigned int)size);
	
	char *programName = "wordRectMac";
	char *wordRectArgv[] = { programName, "-s" };
	size_t sz = sizeof(wordRectArgv);
	int numArgs = sz/sizeof(char *);
	printf("sizeof(argv) is %u, and numArgs is %d\n", (unsigned int)sz, numArgs);
	return wordRectMain(2, wordRectArgv);
}
