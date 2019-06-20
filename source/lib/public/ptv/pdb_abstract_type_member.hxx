#pragma once
#include <cstdint>

namespace ptv
{
    enum class pdb_member_type
    {
        inherited,
        field,
        padding,
    };

    class pdb_abstract_type_member
    {
    private:
        uint64_t m_size{};
        uint64_t m_offset{};

    public:
        pdb_abstract_type_member(
            uint64_t size,
            uint64_t offset
        ) noexcept
            : m_size{ size }
            , m_offset{ offset }
        {
        }

        virtual ~pdb_abstract_type_member() noexcept = default;

    public:
        virtual pdb_member_type get_member_type() const noexcept = 0;

        uint64_t get_size() const noexcept
        {
            return m_size;
        }

        uint64_t get_offset() const noexcept
        {
            return m_offset;
        }

        uint64_t get_next_offset() const noexcept
        {
            return m_offset + m_size;
        }
    };
}