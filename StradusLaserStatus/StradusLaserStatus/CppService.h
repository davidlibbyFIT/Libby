// MixedCode.h
// (C) 2016 Vortran Laser Technology, Inc.

#pragma once

#ifdef INSIDE_MANAGED_CODE
#    define DECLSPECIFIER __declspec(dllexport)
#    define EXPIMP_TEMPLATE
#else
#    define DECLSPECIFIER __declspec(dllimport)
#    define EXPIMP_TEMPLATE extern
#endif

#include <string>

namespace MixedCode
{
    class DECLSPECIFIER StradusService
    {
		public:
			StradusService();
			virtual ~StradusService();

		public:
			void InitStradusHub();
			int StradusCount();
			std::string StradusGetReply(int myLaser);
			void StradusSend(long, std::string);

		private:
			void * m_impl;
    };
}