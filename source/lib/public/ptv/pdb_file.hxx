#pragma once
#include <memory>
#include <vector>
#include <string_view>
#include <functional>
#include <ptv/pdb_type.hxx>
#include <ptv/pdb_type_descriptor.hxx>

namespace ptv
{
    class pdb_file
    {
    public:
        virtual ~pdb_file() noexcept = default;

        virtual bool load(
            std::wstring_view path,
            std::function<void(int32_t current, int32_t total)> progress
        ) noexcept = 0;

    public:
        virtual const std::vector<std::unique_ptr<pdb_type>>& get_types() const noexcept = 0;

        virtual std::unique_ptr<pdb_type_descriptor> get_descriptor(
            const pdb_type& type
        ) const noexcept = 0;
    };
}
