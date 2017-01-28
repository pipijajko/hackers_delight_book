#include <cstdio>
#include <cstdint>
#include <cassert>
#include <type_traits>


template<typename T> //Enable for unsigned only
typename std::enable_if<std::is_unsigned<T>::value, T>::type
hd_snoob(T x){
	// Find next larger number with the same amount of `1`s,
	// when bits are used to represent array item indices, this
	// can be used to iterate over all subsets of the same size.
						    //x = xxx0 1111 0000
	T s = x & (-x);		    //s = 0000 0001 0000 -> smallest bit
	T r = s + x;			//r = 0001 0000 0000 -> ripple
	T y = x ^ r;			//y = 0001 1111 0000 -> ones everywhere
		     y = (y >> 2) / s; 	//y = 0000 0000 0111 -> right adjust and shift by 2 to discard unwanted bits
		     
	return r | y;	 
}

template<typename T,
         typename = std::enable_if<std::is_signed<T>::value>,
         //typename U = std::make_unsigned<T>::type, 
         int BitN = sizeof(T) * 8>
T hd_abs(T x){ 
    // Calculate absolute value
    T y = x >> (BitN - 1);
    T abs = (x ^ y) - y;
    T abs_alt = ( (x >> (BitN - 2)) | 1 ) * x; //but needs fast multiplication
    assert( abs == abs_alt);
    return abs;
}

template<typename T> 
typename std::enable_if<std::is_unsigned<T>::value, T>::type
hd_avg_ceil(T x, T y) {
    // Calculate  ceiling-average of two unsigned integers without causing overflow
    return (x & y) + ((x ^ y) >> 1); // (half of addidtion carries) + (sum with carries ignored / 2)
}

template<typename T, int BitN = sizeof(T) * 8> 
typename std::enable_if<std::is_signed<T>::value, T>::type
hd_s0avg(T x, T y) {
    // Calculate ceiling-average of two signed integers, ceiling goes towards zero
	typedef typename std::make_unsigned<T>::type unsigned_T;
    T t = (x & y) + ((x ^ y) >> 1);                     // (half of addidtion carries) + (sum with carries ignored / 2)
    T a = ((unsigned_T)t >> (BitN - 1)) & (x ^ y);      // 1 IFF one of the numbers had sign-bit set
    return t + a;
}


int main(void) {

	uint32_t x = 0b000011110000;
	for(int32_t i = 0; i < 10; i++){
		printf("i:%d,x:0x%x\n",i, x);
		x = hd_snoob(x);
	}

    for(int32_t i=0; i > -10; i--){
        printf("i=%d,abs(i)=%d\n",i, hd_abs(i));
    }

    for(int32_t i=-50; i < 50; i+=16){
        for(int32_t j=-25; j < 75; j+=16){
            printf("i:%d,j:%d,avg_ceil(abs(i), abs(j):%d,s0avg_ceil(i,j):%d\n", 
            	i, j, 
            	hd_avg_ceil((uint32_t)hd_abs(i), (uint32_t)hd_abs(j)), 
            	hd_s0avg(i,j));
        }
    }
	return 0;
} 
