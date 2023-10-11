#pragma once
#include <string_view>
#include <vector>
#include <optional>
#include <expected>

#include "typeviewer_engine/Importer.hxx"
#include "typeviewer_engine/Diagnostic.hxx"

namespace TypeViewer::Model
{
    class Type
    {
    public:
        virtual ~Type() = 0;
    };

    class Field
    {
    public:
        virtual ~Field() = 0;
    };

    class Database
    {
    public:
        virtual ~Database() = 0;

    public:
        virtual Type* QueryTypeByName(std::string_view name) const = 0;

        virtual std::vector<Type*> QueryTypes() const = 0;

        virtual std::vector<std::string> QueryDataSources() const = 0;

        virtual void Import(Importer& importer, DiagnosticLog& log) = 0;
    };

    std::unique_ptr<Database> CreateDatabase(const char* path, DiagnosticLog& log);

    std::unique_ptr<Database> OpenDatabase(const char* path, DiagnosticLog& log);

    std::unique_ptr<Database> CreateMemoryDatabase(DiagnosticLog& log);
}
