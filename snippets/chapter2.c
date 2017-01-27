#include <stdio.h>
#include <stdint.h>
#include <assert.h>

uint32_t snoob(uint32_t x){
	// Find next larger number with the same amount of `1`s,
	// when bits are used to represent array item indices, this
	// can be used to iterate over all subsets of the same size.
	
								//x = xxx0 1111 0000
	uint32_t s = x & (-x);		//s = 0000 0001 0000 -> smallest bit
	uint32_t r = s + x;			//r = 0001 0000 0000 -> ripple
	uint32_t y = x ^ r;			//y = 0001 1111 0000 -> ones everywhere
		     y = (y >> 2) / s; 	//y = 0000 0000 0111 -> right adjust and shift by 2 to discard unwanted bits
		     
	return r | y;	 
}

int32_t abs(int32_t x){

    int32_t y = x >> 31;
    int32_t abs = (x ^ y) - y;

    int32_t abs_alt = ( (x >> 30) | 1 ) * x; //but needs fast multiplication
    assert( abs == abs_alt);
    return abs;
}


int main(void) {
	uint32_t x = 0b000011110000;
	for(int i = 0; i < 10; i++){
		printf("i:%d,x:0x%x\n",i, x);
		x = snoob(x);
	}

    for(int i=0; i > -10; i--){
        printf("i=%d,abs(i)=%d\n",i, abs(i));
    }

	return 0;
}