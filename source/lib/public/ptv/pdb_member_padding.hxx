#pragma once
#include <ptv/pdb_abstract_type_member.hxx>

namespace ptv
{
    class pdb_member_padding : public pdb_abstract_type_member
    {
    public:
        pdb_member_padding(
            uint64_t size,
            uint64_t offset
        ) noexcept
            : pdb_abstract_type_member{ size, offset }
        {
        }

        virtual ~pdb_member_padding() noexcept = default;

    public:
        virtual pdb_member_type get_member_type() const noexcept override
        {
            return pdb_member_type::padding;
        }
    };
}
