#pragma once
#include <ptv/pdb_abstract_type_member.hxx>

namespace ptv
{
    class pdb_member_ending : public pdb_abstract_type_member
    {
    public:
        pdb_member_ending(
            uint64_t size,
            uint64_t offset
        ) noexcept
            : pdb_abstract_type_member{ size, offset }
        {
        }

        virtual ~pdb_member_ending() noexcept = default;

    public:
        virtual pdb_member_type get_member_type() const noexcept override
        {
            return pdb_member_type::ending;
        }
    };
}
