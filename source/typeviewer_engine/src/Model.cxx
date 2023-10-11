#include "typeviewer_engine/Model.hxx"

#include <sqlite3.h>

namespace TypeViewer::Model
{
    Database::~Database() = default;
}

namespace TypeViewer::Model
{
    class SqliteDatabase : public Database
    {
    private:
        sqlite3* _handle{};

    public:
        SqliteDatabase(sqlite3* handle);

        ~SqliteDatabase() override;

    public:
        Type* QueryTypeByName(std::string_view name) const override;
        std::vector<Type*> QueryTypes() const override;
        std::vector<std::string> QueryDataSources() const override;
        void Import(Importer& importer, DiagnosticLog& log) override;
    };

    SqliteDatabase::SqliteDatabase(sqlite3* handle)
        : _handle{handle}
    {
    }

    SqliteDatabase::~SqliteDatabase()
    {
        sqlite3_close_v2(this->_handle);
    }

    Type* SqliteDatabase::QueryTypeByName(std::string_view name) const
    {
        (void)name;
        return nullptr;
    }

    std::vector<Type*> SqliteDatabase::QueryTypes() const
    {
        return {};
    }

    std::vector<std::string> SqliteDatabase::QueryDataSources() const
    {
        return {};
    }

    void SqliteDatabase::Import(Importer& importer, DiagnosticLog& log)
    {
        (void)importer;
        (void)log;
    }
}

namespace TypeViewer::Model
{
    std::unique_ptr<Database> CreateDatabase(const char* path, DiagnosticLog& log)
    {
        (void)log;
        sqlite3* handle{};

        if (int const rc = sqlite3_open_v2(path, &handle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_EXRESCODE, nullptr); rc != SQLITE_OK)
        {
            log.AddError(fmt::format("Cannot open or create database file '{}', {}", path, rc));
            return {};
        }

        return std::make_unique<SqliteDatabase>(handle);
    }

    std::unique_ptr<Database> OpenDatabase(const char* path, DiagnosticLog& log)
    {
        (void)log;
        sqlite3* handle{};

        if (int const rc = sqlite3_open_v2(path, &handle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_EXRESCODE, nullptr); rc != SQLITE_OK)
        {
            log.AddError(fmt::format("Cannot open database file '{}', {}", path, rc));
            return {};
        }

        return std::make_unique<SqliteDatabase>(handle);
    }

    std::unique_ptr<Database> CreateMemoryDatabase(DiagnosticLog& log)
    {
        (void)log;
        sqlite3* handle{};

        if (int const rc = sqlite3_open_v2(":memory:", &handle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY | SQLITE_OPEN_EXRESCODE, nullptr); rc != SQLITE_OK)
        {
            log.AddError(fmt::format("Cannot create database, {}", rc));
            return {};
        }

        return std::make_unique<SqliteDatabase>(handle);
    }
}
