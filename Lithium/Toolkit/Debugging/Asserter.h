#ifndef _ASSERTER_H
#define _ASSERTER_H

#define XASSERTS_ENABLED

namespace xassert {
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

#define XHALT() __debugbreak()
#define XUNUSED(x) do { (void)sizeof(x); } while(0)

#ifdef XASSERTS_ENABLED
#define XASSERT(cond) \
		do \
		{ \
			if (!(cond)) \
			{ \
				if (xassert::Assert::ReportFailure(#cond, __FILE__, __LINE__, 0) == \
					xassert::Assert::Halt) \
					XHALT(); \
			} \
		} while(0)

#define XASSERT_MSG(cond, msg, ...) \
		do \
		{ \
			if (!(cond)) \
			{ \
				if (xassert::Assert::ReportFailure(#cond, __FILE__, __LINE__, (msg), __VA_ARGS__) == \
					xassert::Assert::Halt) \
					XHALT(); \
			} \
		} while(0)

#define XASSERT_FAIL(msg, ...) \
		do \
		{ \
			if (xassert::Assert::ReportFailure(0, __FILE__, __LINE__, (msg), __VA_ARGS__) == \
				xassert::Assert::Halt) \
			XHALT(); \
		} while(0)

#define XVERIFY(cond) XASSERT(cond)
#define XVERIFY_MSG(cond, msg, ...) XASSERT_MSG(cond, msg, ##__VA_ARGS__)
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

#define XSTATIC_ASSERT(x) \
	typedef char XStaticAssert[(x) ? 1 : -1];

//lint -esym(751,XStaticAssert)
//lint -esym(751,xassert::XStaticAssert)

#endif
