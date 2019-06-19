#pragma once
#include <memory>
#include <vector>
#include <string_view>
#include <ptv/pdb_type.hxx>
#include <ptv/pdb_type_descriptor.hxx>

namespace ptv
{
    class pdb_file
    {
    public:
        virtual ~pdb_file() noexcept = default;

        virtual bool load(
            std::wstring_view path
        ) noexcept = 0;

    public:
        virtual const std::vector<pdb_type>& get_types() const noexcept = 0;

        virtual std::unique_ptr<pdb_type_descriptor> get_descriptor(
            const pdb_type& type
        ) const noexcept = 0;
    };
}
