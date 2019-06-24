#pragma once

#if defined(PTV_BUILDING_LIB)
#define PTV_API __declspec(dllexport)
#else
#define PTV_API __declspec(dllimport)
#endif

#include <LibPtv/Session.hxx>


namespace LibPdb
{
    PTV_API std::unique_ptr<Session> CreateSession() noexcept;
}

namespace LibPdb::Helpers
{
    PTV_API std::wstring Convert(
        std::string_view value
    ) noexcept;

    PTV_API std::string Convert(
        std::wstring_view value
    ) noexcept;
}
