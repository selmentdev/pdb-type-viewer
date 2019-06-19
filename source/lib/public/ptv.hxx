#pragma once

#if defined(PTV_BUILDING_LIB)
#define PTV_API __declspec(dllexport)
#else
#define PTV_API __declspec(dllimport)
#endif

#include <ptv/pdb_file.hxx>


namespace ptv
{
    PTV_API std::unique_ptr<pdb_file> create() noexcept;
}
