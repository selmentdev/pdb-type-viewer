#pragma once
#include <cstdint>

namespace LibPdb
{
    enum class MemberType
    {
        Inherited,
        Field,
        Padding,
    };

    class BaseTypeMember
    {
    private:
        uint64_t m_Size{};
        uint64_t m_Offset{};

    public:
        BaseTypeMember(
            uint64_t size,
            uint64_t offset
        ) noexcept
            : m_Size{ size }
            , m_Offset{ offset }
        {
        }

        virtual ~BaseTypeMember() noexcept = default;

    public:
        virtual MemberType GetMemberType() const noexcept = 0;

        uint64_t GetSize() const noexcept
        {
            return m_Size;
        }

        uint64_t GetOffset() const noexcept
        {
            return m_Offset;
        }

        uint64_t GetNextOffset() const noexcept
        {
            return m_Offset + m_Size;
        }
    };
}