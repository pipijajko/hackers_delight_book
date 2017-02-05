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
        int BitN = sizeof(T) * 8>
T hd_abs(T x){ 
    // Calculate absolute value
    T y = x >> (BitN - 1);
    T abs = (x ^ y) - y;
    T abs_alt = ( (x >> (BitN - 2)) | 1 ) * x; //but needs fast multiplication
    assert( abs == abs_alt);
    return abs;
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

template<typename T> 
typename std::enable_if<std::is_unsigned<T>::value, T>::type
hd_avg_ceil(T x, T y) {
    // Calculate  ceiling-average of two unsigned integers without causing overflow
    return (x & y) + ((x ^ y) >> 1); // (half of addidtion carries) + (sum with carries ignored / 2)
}

template<typename T, int BitN = sizeof(T) * 8> 
T hd_add_check_overflow(T x, T y) {
    // Return 1 if the operation of addition of two signed numbers would set the overflow flag, otherwise return 0.
    // NOTE: Overflow occurs IFF operands have same sign and the sum has the sign that is opposite.
    static_assert(std::is_signed<T>::value, "generic type T is unsigned!"); //maybe this is better than SFINAE for type constraints? errors look much better IMO
    T s = x + y;
    return ~(x ^ y) & (s ^ x);
}
    


template<typename T, int BitN = sizeof(T) * 8> 
bool hd_add_check_overflow(T x, T y, T c) {
    // Return 1 if the operation of addition of two signed numbers and an optional carry (0 or 1) would set the overflow flag.
    static_assert(std::is_signed<T>::value, "generic type T is unsigned!"); 
    T constexpr sign_mask = 1 << (BitN-1);
    
    printf("0x%hhx 0x%hhx 0x%hhx 0x%hhx 0x%hhx \n", sign_mask, (x ^ sign_mask), (y ^ sign_mask), c << (BitN-1), BitN);
    return (((x ^ sign_mask) + (y ^ sign_mask)) & (sign_mask)) == (c << (BitN-1));
}



int main(void) {

	uint32_t x = 0b000011110000;
	for(int32_t i = 0; i < 10; i++){
		printf("i:%d,x:0x%x\n",i, x);
		x = hd_snoob(x);
	}

    for(int32_t i=2; i > -3; i--){
       printf("i=%d,abs(i)=%d\n",i, hd_abs(i));
    }

    for(int32_t i=-50; i < 50; i+=32){
        for(int32_t j=-25; j < 75; j+=32){
            printf("i:%d,j:%d,avg_ceil(abs(i), abs(j):%d,s0avg_ceil(i,j):%d\n", 
            	i, j, 
            	hd_avg_ceil((uint32_t)hd_abs(i), (uint32_t)hd_abs(j)), 
            	hd_s0avg(i,j));
        }
    }

	printf("hd_add_check_overflow<int8_t>(0x0F, 0xF0):%d\n",hd_add_check_overflow<int8_t>(0x0F, 0xF4));
    printf("hd_add_check_overflow<int8_t>(0x01, 0xF0, 0):%d\n",hd_add_check_overflow((int8_t)0x0F, (int8_t)0xFE, (int8_t)0));
    printf("hd_add_check_overflow<int8_t>(0xFF, 0xFE, 0):%d\n",hd_add_check_overflow<int8_t>(0xFF, 0xFE, 0));
    printf("hd_add_check_overflow<int8_t>(0x0E, 0xF0, 0):%d\n",hd_add_check_overflow<int8_t>(0x0E, 0xF0, 0));
    printf("hd_add_check_overflow<int8_t>(0x0E, 0xF0, 1):%d\n",hd_add_check_overflow<int8_t>(0x0E, 0xF0, 1));
    printf("hd_add_check_overflow<int8_t>(0x0F, 0xF0, 0):%d\n",hd_add_check_overflow<int8_t>(0x0F, 0xF0, 0));
    printf("hd_add_check_overflow<int8_t>(0x0F, 0xF0, 1):%d\n",hd_add_check_overflow<int8_t>(0x0F, 0xF0, 1));
	return 0;
} 
