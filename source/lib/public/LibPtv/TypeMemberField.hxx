#pragma once
#include <LibPtv/BaseTypeMember.hxx>
#include <string_view>
#include <string>
#include <optional>
#include <utility>

namespace LibPdb
{
    enum class MemberKind
    {
        Unknown,
        Value,
        VTable,
        Pointer,
        Reference,
        Array,
    };

    class TypeMemberField : public BaseTypeMember
    {
    private:
        std::wstring_view m_Name;
        std::wstring m_TypeName;
        MemberKind m_Kind;
        std::optional<std::pair<uint64_t, uint64_t>> m_Bits;

    public:
        TypeMemberField(
            uint64_t size,
            uint64_t offset,
            std::wstring_view name,
            std::wstring_view type_name,
            MemberKind kind,
            std::optional<std::pair<uint64_t, uint64_t>> bits
        ) noexcept
            : BaseTypeMember{ size, offset }
            , m_Name{ name }
            , m_TypeName{ type_name }
            , m_Kind{ kind }
            , m_Bits{std::move( bits )}
        {
        }

    public:
        [[nodiscard]] MemberType GetMemberType() const noexcept override
        {
            return MemberType::Field;
        }

        [[nodiscard]] std::wstring_view GetName() const noexcept
        {
            return { m_Name };
        }

        [[nodiscard]] std::wstring_view GetTypeName() const noexcept
        {
            return m_TypeName;
        }

        [[nodiscard]] MemberKind GetKind() const noexcept
        {
            return m_Kind;
        }

        [[nodiscard]] std::optional<std::pair<uint64_t, uint64_t>> GetBits() const noexcept
        {
            return m_Bits;
        }
    };
}
