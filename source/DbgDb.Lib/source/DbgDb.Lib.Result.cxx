#include <DbgDb.Lib.Result.hxx>

namespace DbgDb::Lib
{
    DBGDBLIB_EXPORT const char* ToString(Result result) noexcept
    {
        // Fucking Qt, does not accept `std::string_view` to `QString` conversion...
        switch (result)
        {
        case Result::Success:
            return "Success";
        case Result::FailedCreateSchema:
            return "Failed to create schema";
        case Result::FailedCreateDatabase:
            return "Failed to create database";
        case Result::NotImplemented:
            return "Not Implemented";
        }

        return "<<unknown>>";
    }
}
