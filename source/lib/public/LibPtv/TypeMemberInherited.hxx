#include <LibPtv/BaseTypeMember.hxx>
#include <memory>
#include <vector>
#include <string>
#include <string_view>

namespace LibPdb
{
    class TypeMemberInherited : public BaseTypeMember
    {
    private:
        uint64_t m_Padding;
        std::wstring m_Name;
        std::vector<std::unique_ptr<BaseTypeMember>> m_Members;

    public:
        TypeMemberInherited(
            uint64_t size,
            uint64_t offset,
            uint64_t padding,
            std::wstring_view name,
            std::vector<std::unique_ptr<BaseTypeMember>>&& members
        ) noexcept
            : BaseTypeMember{ size, offset }
            , m_Padding{ padding }
            , m_Name{ name }
            , m_Members{ std::move(members) }
        {
        }

        virtual ~TypeMemberInherited() noexcept = default;

    public:
        virtual MemberType GetMemberType() const noexcept override
        {
            return MemberType::Inherited;
        }

        virtual const std::vector<std::unique_ptr<BaseTypeMember>>& GetMembers() const noexcept
        {
            return m_Members;
        }

        std::wstring_view GetName() const noexcept
        {
            return m_Name;
        }

        uint64_t GetPadding() const noexcept
        {
            return m_Padding;
        }
    };
}
