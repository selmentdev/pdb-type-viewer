#pragma once
#include <memory>
#include <vector>
#include <string_view>
#include <functional>
#include <LibPtv/Type.hxx>
#include <LibPtv/TypeDescriptor.hxx>

namespace LibPdb
{
    class Session
    {
    public:
        virtual ~Session() noexcept = default;

        virtual bool Load(
            std::wstring_view path,
            std::function<void(int32_t current, int32_t total, std::wstring_view name)> progress
        ) noexcept = 0;

    public:
        [[nodiscard]] virtual const std::vector<std::unique_ptr<Type>>& GetTypes() const noexcept = 0;

        [[nodiscard]] virtual std::unique_ptr<TypeDescriptor> GetDescriptor(
            const Type& type
        ) const noexcept = 0;
    };
}
