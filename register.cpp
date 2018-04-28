// Simple code to fix compilation errors with latest g++

#include <avr/io.h>

// This internal macro is used by individual boards headers
//#define R_(REG) ((uint16_t)&REG)
#define R_(REG) (uint16_t(__builtin_constant_p(&REG) ? &REG : &REG))
//#define R_(REG) ((uintptr_t)&REG)

#define INLINE __attribute__((always_inline))

template<typename T>
class REGISTER
{
public:
	constexpr REGISTER():ADDR(0xFFFF) {}
	constexpr REGISTER(uint16_t ADDR) INLINE:ADDR(ADDR) {}

	bool is_no_reg() const INLINE
	{
		return ADDR == 0xFFFF;
	}
	void operator =(int value) const INLINE 
	{
		*((volatile T*) ADDR) = (T) value;
	}
	void operator |=(int value) const INLINE 
	{
		*((volatile T*) ADDR) |= (T) value;
	}
	void operator &=(int value) const INLINE 
	{
		*((volatile T*) ADDR) &= (T) value;
	}
	void operator ^=(int value) const INLINE 
	{
		*((volatile T*) ADDR) ^= (T) value;
	}
	T operator ~() const INLINE 
	{
		return ~(*((volatile T*) ADDR));
	}
	void loop_until_bit_set(uint8_t bit) const INLINE
	{
		while (!(*((volatile T*) ADDR) & _BV(bit))) ;
	}
	void loop_until_bit_clear(uint8_t bit) const INLINE
	{
		while (*((volatile T*) ADDR) & _BV(bit)) ;
	}
	bool operator ==(int value) const INLINE
	{
		return *((volatile T*) ADDR) == (T) value;
	}
	bool operator !=(int value) const INLINE
	{
		return *((volatile T*) ADDR) != (T) value;
	}
	bool operator >(int value) const INLINE
	{
		return *((volatile T*) ADDR) > (T) value;
	}
	bool operator >=(int value) const INLINE
	{
		return *((volatile T*) ADDR) >= (T) value;
	}
	bool operator <(int value) const INLINE
	{
		return *((volatile T*) ADDR) < (T) value;
	}
	bool operator <=(int value) const INLINE
	{
		return *((volatile T*) ADDR) <= (T) value;
	}
	operator volatile T&() const INLINE
	{
		return *((volatile T*) ADDR);
	}

private:	
	const uint16_t ADDR;
};

using REG8 = REGISTER<uint8_t>;
using REG16 = REGISTER<uint16_t>;

using REG = uint16_t;
//using REG = uintptr_t;
static constexpr REG NO_REG = 0xFFFF;
	
template<REG PIN_> struct Port_trait
{
	static constexpr const REG8 PIN{PIN_};
};

using TRAIT = Port_trait<R_(PINB)>;

int main()
{
	TRAIT::PIN = 0xFF;
}

