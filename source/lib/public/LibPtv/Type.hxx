#pragma once
#include <string_view>
#include <memory>

namespace LibPdb
{
    class Type
    {
    public:
        virtual ~Type() noexcept = default;

        virtual std::wstring_view GetName() const noexcept = 0;

        virtual std::unique_ptr<Type> Clone() const noexcept = 0;
    };
}
