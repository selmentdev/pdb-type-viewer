#pragma once
#include <ptv/pdb_abstract_type_member.hxx>
#include <string_view>
#include <string>
#include <optional>

namespace ptv
{
    class pdb_member_field : public pdb_abstract_type_member
    {
    private:
        std::wstring_view m_name;
        std::wstring m_type_name;
        std::optional<std::pair<uint64_t, uint64_t>> m_bits;

    public:
        pdb_member_field(
            uint64_t size,
            uint64_t offset,
            std::wstring_view name,
            std::wstring_view type_name,
            std::optional<std::pair<uint64_t, uint64_t>> bits
        ) noexcept
            : pdb_abstract_type_member{ size, offset }
            , m_name{ name }
            , m_type_name{ type_name }
            , m_bits{ bits }
        {
        }

        virtual ~pdb_member_field() noexcept = default;

    public:
        virtual pdb_member_type get_member_type() const noexcept override
        {
            return pdb_member_type::field;
        }

        std::wstring_view get_name() const noexcept
        {
            return { m_name };
        }

        std::wstring_view get_type_name() const noexcept
        {
            return m_type_name;
        }

        std::optional<std::pair<uint64_t, uint64_t>> get_bit_start() const noexcept
        {
            return m_bits;
        }
    };
}
