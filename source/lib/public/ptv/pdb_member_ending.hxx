#pragma once
#include <ptv/pdb_abstract_type_member.hxx>

namespace ptv
{
    class pdb_member_ending : public pdb_abstract_type_member
    {
    private:
        bool m_is_spurious;

    public:
        pdb_member_ending(
            uint64_t size,
            uint64_t offset
        ) noexcept
            : pdb_abstract_type_member{ size, offset }
            , m_is_spurious{ false }
        {
        }

        virtual ~pdb_member_ending() noexcept = default;

    public:
        virtual pdb_member_type get_member_type() const noexcept override
        {
            return pdb_member_type::ending;
        }

        void set_spurious(bool value) noexcept
        {
            m_is_spurious = value;
        }
    };
}
