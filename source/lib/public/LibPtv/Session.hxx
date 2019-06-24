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
            std::function<void(int32_t current, int32_t total)> progress
        ) noexcept = 0;

    public:
        virtual const std::vector<std::unique_ptr<Type>>& GetTypes() const noexcept = 0;

        virtual std::unique_ptr<TypeDescriptor> GetDescriptor(
            const Type& type
        ) const noexcept = 0;
    };
}
