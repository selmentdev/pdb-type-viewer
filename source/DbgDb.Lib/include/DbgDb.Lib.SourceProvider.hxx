#pragma once
#include <dbgdblib_export.h>

namespace DbgDb::Lib
{
    class DBGDBLIB_EXPORT ISourceProvider
    {
    public:
        virtual ~ISourceProvider() noexcept {}
    };

    DBGDBLIB_EXPORT ISourceProvider* CreateProvider() noexcept;
}
