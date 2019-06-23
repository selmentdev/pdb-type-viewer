#pragma once
#include <string_view>
#include <memory>

namespace ptv
{
    class pdb_type
    {
    public:
        virtual ~pdb_type() noexcept = default;

        virtual std::wstring_view get_name() const noexcept = 0;

        virtual std::unique_ptr<pdb_type> clone() const noexcept = 0;
    };
}
