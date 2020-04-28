#pragma once
#include <dbgdblib_export.h>
#include <cstdint>
#include <string_view>

namespace DbgDb::Lib
{
    enum class Result : int32_t
    {
        Success,
        FailedCreateSchema,
        FailedCreateDatabase,
        NotImplemented,
    };

    DBGDBLIB_EXPORT const char* ToString(Result result) noexcept;
}
