#include "types.h"
#include "lib/rvl/OSTime.h"

class RPUtlRandom
{
public:
	static const u32 SEED_STEP = 69069;

public:
	// Based on 801bea28, but not void signature
	static void initialize(const OSCalendarTime&);

	static void setSeed(u32);
	static u32 getSeed();
	static u32 calc();   // inlined
	static void advance(u32);
	
	static u32 getU32(); // inlined
	static f32 getF32(); // inlined

private:
	inline RPUtlRandom() : mSeed(NULL) {}
	u32 mSeed; // at 804bf688

	static RPUtlRandom* mInstance;
	static RPUtlRandom* getInstance();
};
