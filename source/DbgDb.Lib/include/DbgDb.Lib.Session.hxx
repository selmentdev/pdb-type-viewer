#pragma once
#include <dbgdblib_export.h>
#include <memory>
#include <string>
#include <DbgDb.Lib.Result.hxx>

namespace DbgDb::Lib
{
    class ISession
    {
    public:
        virtual ~ISession() noexcept = 0 {}
    };

    DBGDBLIB_EXPORT std::unique_ptr<ISession> CreateSession(std::string path, Result& result) noexcept;
}
