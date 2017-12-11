#ifndef _ASSERTER_H
#define _ASSERTER_H

#define POW2_ASSERTS_ENABLED

namespace pow2 {
    namespace Assert
    {
        enum FailBehavior
        {
            Halt,
            Continue,
        };

        typedef FailBehavior(*Handler)(const char* condition,
            const char* msg,
            const char* file,
            int line);

        Handler GetHandler();
        void SetHandler(Handler newHandler);

        FailBehavior ReportFailure(const char* condition,
            const char* file,
            int line,
            const char* msg, ...);
    }
}

#define POW2_HALT() __debugbreak()
#define POW2_UNUSED(x) do { (void)sizeof(x); } while(0)

#ifdef POW2_ASSERTS_ENABLED
#define POW2_ASSERT(cond) \
		do \
		{ \
			if (!(cond)) \
			{ \
				if (pow2::Assert::ReportFailure(#cond, __FILE__, __LINE__, 0) == \
					pow2::Assert::Halt) \
					POW2_HALT(); \
			} \
		} while(0)

#define POW2_ASSERT_MSG(cond, msg, ...) \
		do \
		{ \
			if (!(cond)) \
			{ \
				if (pow2::Assert::ReportFailure(#cond, __FILE__, __LINE__, (msg), __VA_ARGS__) == \
					pow2::Assert::Halt) \
					POW2_HALT(); \
			} \
		} while(0)

#define POW2_ASSERT_FAIL(msg, ...) \
		do \
		{ \
			if (pow2::Assert::ReportFailure(0, __FILE__, __LINE__, (msg), __VA_ARGS__) == \
				pow2::Assert::Halt) \
			POW2_HALT(); \
		} while(0)

#define POW2_VERIFY(cond) POW2_ASSERT(cond)
#define POW2_VERIFY_MSG(cond, msg, ...) POW2_ASSERT_MSG(cond, msg, ##__VA_ARGS__)
#else
#define POW2_ASSERT(condition) \
		do { POW2_UNUSED(condition); } while(0)
#define POW2_ASSERT_MSG(condition, msg, ...) \
		do { POW2_UNUSED(condition); POW2_UNUSED(msg); } while(0)
#define POW2_ASSERT_FAIL(msg, ...) \
		do { POW2_UNUSED(msg); } while(0)
#define POW2_VERIFY(cond) (void)(cond)
#define POW2_VERIFY_MSG(cond, msg, ...) \
		do { (void)(cond); POW2_UNUSED(msg); } while(0)
#endif

#define POW2_STATIC_ASSERT(x) \
	typedef char pow2StaticAssert[(x) ? 1 : -1];

//lint -esym(751,pow2StaticAssert)
//lint -esym(751,pow2::pow2StaticAssert)

#endif
