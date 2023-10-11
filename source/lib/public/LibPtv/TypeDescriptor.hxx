#pragma once
#include <LibPtv/BaseTypeMember.hxx>
#include <vector>
#include <memory>

namespace LibPdb
{
    class TypeDescriptor
    {
    private:
        std::vector<std::unique_ptr<BaseTypeMember>> m_Members;

    public:
        explicit TypeDescriptor(
            std::vector<std::unique_ptr<BaseTypeMember>>&& members
        ) noexcept
            : m_Members{ std::move(members) }
        {
        }

        [[nodiscard]] const std::vector<std::unique_ptr<BaseTypeMember>>& GetMembers() const noexcept
        {
            return m_Members;
        }
    };
}
