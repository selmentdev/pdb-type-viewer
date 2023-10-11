#pragma once
#include <LibPtv/BaseTypeMember.hxx>

namespace LibPdb
{
    class TypeMemberPadding : public BaseTypeMember
    {
    private:
        bool m_IsSpurious;

    public:
        TypeMemberPadding(
            uint64_t size,
            uint64_t offset
        ) noexcept
            : BaseTypeMember{ size, offset }
            , m_IsSpurious{ false }
        {
        }

        ~TypeMemberPadding() noexcept override = default;

    public:
        [[nodiscard]] MemberType GetMemberType() const noexcept override
        {
            return MemberType::Padding;
        }

        void SetSpurious(bool value) noexcept
        {
            m_IsSpurious = value;
        }

        [[nodiscard]] bool IsSpurious() const noexcept
        {
            return m_IsSpurious;
        }
    };
}
