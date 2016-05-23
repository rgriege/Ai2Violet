#ifndef __AI2VIOLETUTILITY_H__
#define __AI2VIOLETUTILITY_H__

#include "IllustratorSDK.h"
#include "SDKErrors.h"

namespace Ai2Vlt
{
	bool isShape(AIArtHandle art, const char * shapeName);

	class Error : public std::exception
	{
	public:

		Error(const char * context, AIErr err);
		Error & operator=(const Error &) = delete;
		operator AIErr () const;
		void report();

	private:

		const AIErr m_err;
		bool m_reported;
	};

	inline void check_ai_error(const char * context, AIErr err)
	{
		if (err) throw Ai2Vlt::Error(context, err);
	}

	inline void check_ai_error_except(const char * context, AIErr err, AIErr exception)
	{
		if (err && err != exception) throw Ai2Vlt::Error(context, err);
	}

#ifdef _DEBUG

#define _S1(x) #x
#define _S2(x) _S1(x)
#define LOCATION __FILE__ " : " _S2(__LINE__)

#define CHECK(call) Ai2Vlt::check_ai_error(LOCATION, call)
#define CHECK_EXCEPT(call, er) Ai2Vlt::check_ai_error_except(LOCATION, call, er)

#else

#define CHECK(call) Ai2Vlt::check_ai_error("", call)
#define CHECK_EXCEPT(call, er) Ai2Vlt::check_ai_error_except("", call, er)

#endif
}

#endif