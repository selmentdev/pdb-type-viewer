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
            return "FailedCreateSchema";
        case Result::FailedCreateDatabase:
            return "FailedCreateDatabase";
        }

        return "<<unknown>>";
    }
}
