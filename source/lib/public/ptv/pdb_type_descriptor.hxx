#pragma once
#include <ptv/pdb_abstract_type_member.hxx>
#include <vector>
#include <memory>

namespace ptv
{
    class pdb_type_descriptor
    {
    private:
        std::vector<std::unique_ptr<pdb_abstract_type_member>> m_members;

    public:
        pdb_type_descriptor(
            std::vector<std::unique_ptr<pdb_abstract_type_member>>&& members
        ) noexcept
            : m_members{ std::move(members) }
        {
        }

        const std::vector<std::unique_ptr<pdb_abstract_type_member>>& get_members() const noexcept
        {
            return m_members;
        }
    };
}
