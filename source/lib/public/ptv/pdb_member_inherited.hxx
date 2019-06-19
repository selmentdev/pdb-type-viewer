#include <ptv/pdb_abstract_type_member.hxx>
#include <memory>
#include <vector>
#include <string>
#include <string_view>

namespace ptv
{
    class pdb_member_inherited : public pdb_abstract_type_member
    {
    private:
        std::wstring m_name;
        std::vector<std::unique_ptr<pdb_abstract_type_member>> m_members;

    public:
        pdb_member_inherited(
            uint64_t size,
            uint64_t offset,
            std::wstring_view name,
            std::vector<std::unique_ptr<pdb_abstract_type_member>>&& members
        ) noexcept
            : pdb_abstract_type_member{ size, offset }
            , m_name{ name }
            , m_members{ std::move(members) }
        {
        }

        virtual ~pdb_member_inherited() noexcept = default;

    public:
        virtual pdb_member_type get_member_type() const noexcept override
        {
            return pdb_member_type::inherited;
        }

        virtual const std::vector<std::unique_ptr<pdb_abstract_type_member>>& get_members() const noexcept
        {
            return m_members;
        }

        std::wstring_view get_name() const noexcept
        {
            return m_name;
        }
    };
}
