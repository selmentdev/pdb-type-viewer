#pragma once
#include <string_view>
#include <memory>

namespace LibPdb
{
    class Type
    {
    public:
        virtual ~Type() noexcept = default;

        [[nodiscard]] virtual std::wstring_view GetName() const noexcept = 0;
    };
}
