// Simple code to fix compilation errors with latest g++

#include <avr/io.h>

#ifdef _SFR_IO8
#undef _SFR_IO8
#endif
#define _SFR_IO8(x) ((x) + __SFR_OFFSET)
#ifdef _SFR_IO16
#undef _SFR_IO16
#endif
#define _SFR_IO16(x) ((x) + __SFR_OFFSET)
#ifdef _SFR_MEM8
#undef _SFR_MEM8
#endif
#define _SFR_MEM8(x) (x)
#ifdef _SFR_MEM16
#undef _SFR_MEM16
#endif
#define _SFR_MEM16(x) (x)

// This internal macro is used by individual boards headers
#define R_(REG) (uint16_t(REG))

#define INLINE __attribute__((always_inline))

class REG8
{
public:
	// constexpr REGISTER():ADDR(0xFFFF) {}
	constexpr REG8(uint16_t ADDR) INLINE:ADDR{ADDR} {}

	void operator =(int value) const INLINE 
	{
		*((volatile uint8_t*) ADDR) = (uint8_t) value;
	}

private:	
	const uint16_t ADDR;
};

using REG = uint16_t;
template<REG PIN_> struct Port_trait
{
	static constexpr const REG8 PIN{PIN_};
};

using TRAIT = Port_trait<R_(PINB)>;
// using TRAIT = Port_trait<__builtin_constant_p(DUMMY) ? DUMMY : DUMMY>;
// using TRAIT = Port_trait<R_(PINB)>;

int main()
{
	TRAIT::PIN = 0xFF;
}

