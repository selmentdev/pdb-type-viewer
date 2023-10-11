#pragma once
#include "fmt/format.h"


#include <string>
#include <vector>
#include <span>

#include <fmt/format.h>

namespace TypeViewer
{
    enum class DiagnosticLevel
    {
        Error,
        Warning,
        Information,
        Hint,
    };

    struct DiagnosticEntry final
    {
        DiagnosticLevel Level{};
        std::string Message{};
    };

    class DiagnosticLog
    {
    private:
        std::vector<DiagnosticEntry> _log{};

    public:
        [[nodiscard]] std::span<DiagnosticEntry const> Enumerate() const
        {
            return this->_log;
        }

    public:
        void Add(DiagnosticLevel level, std::string message)
        {
            this->_log.push_back({level, std::move(message)});
        }

        void AddError(std::string message)
        {
            this->Add(DiagnosticLevel::Error, std::move(message));
        }

        void AddWarning(std::string message)
        {
            this->Add(DiagnosticLevel::Warning, std::move(message));
        }

        void AddInformation(std::string message)
        {
            this->Add(DiagnosticLevel::Information, std::move(message));
        }

        void AddHint(std::string message)
        {
            this->Add(DiagnosticLevel::Hint, std::move(message));
        }
    };
}

template <>
struct ::fmt::formatter<TypeViewer::DiagnosticLevel> : formatter<std::string_view>
{
    constexpr auto format(TypeViewer::DiagnosticLevel const& value, auto& context)
    {
        std::string_view result = "unknown";

        switch (value)
        {
        case TypeViewer::DiagnosticLevel::Error:
            result = "error";
            break;
        case TypeViewer::DiagnosticLevel::Warning:
            result = "warning";
            break;
        case TypeViewer::DiagnosticLevel::Information:
            result = "information";
            break;
        case TypeViewer::DiagnosticLevel::Hint:
            result = "hint";
            break;
        default:
            break;
        }

        return formatter<std::string_view>::format(result, context);
    }
};

template <>
struct ::fmt::formatter<TypeViewer::DiagnosticEntry>
{
    constexpr auto parse(auto& context)
    {
        return context.begin();
    }

    constexpr auto format(TypeViewer::DiagnosticEntry const& value, auto& context)
    {
        return fmt::format_to(context.out(), "[{}] {}", value.Level, value.Message);
    }
};
