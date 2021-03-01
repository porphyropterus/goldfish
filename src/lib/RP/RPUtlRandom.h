#include "types.h"
#include "lib/rvl/OSTime.h"

#define INSTANCE_GUARD(x) \
	if (!mInstance) { mInstance = new x(); }

class RPUtlRandom
{
public:
	static const u32 SEED_STEP = 69069;

public:
#ifdef __MWERKS__
	static void initialize();
#else
	static void initialize(u64);
	static void initialize(const OSCalendarTime&);
#endif

	static void setSeed(u32);
	static u32 calc();
	
	static u32 getU32();

private:
	inline RPUtlRandom() : mSeed(NULL) {}
	u32 mSeed;

	static RPUtlRandom* mInstance;
	static RPUtlRandom* getInstance();
};
