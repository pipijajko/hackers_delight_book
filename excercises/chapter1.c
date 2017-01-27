#include <stdio.h>

int main(void) {

  //
	//1.1: Express for(e1;e2;e3) loop as a `while` loop, `do-while` loop
	//
	int const kLimit = 10;
	int i = 0;
	
	for(i = 0; i < kLimit; i++){
		printf("%d\n",i);
	}
	
	i = 0;
	while(i < kLimit){
		printf("%d\n",i);
		i++;
	}
	
	i=0;
	do {
		printf("%d\n", i);
	}while(++i < kLimit);
	
	//
	//1.2 code a for loop which takes all unsigned integer values (32b)
	//
	unsigned int const kMaxUint = 0xffffffff;
	unsigned int ui;
	for(ui = 0; ui <= kMaxUint; ui++){
		if(kMaxUint == ui) printf("%d\n", ui);
	}
	
	
	
	
	
	return 0;
}
