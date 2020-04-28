#include <DbgDb.Lib.Session.hxx>

#include <sqlite3.h>

namespace DbgDb::Lib
{
    static Result CreateSchema(::sqlite3* connection) noexcept
    {
        ::sqlite3_stmt* query{};

        int const rc_prepare = ::sqlite3_prepare_v2(connection, R"sql(
-- Version information
DROP TABLE IF EXISTS typedb_version;
CREATE TABLE typedb_version (
    version_name TEXT NOT NULL,
    version_id INTEGER NOT NULL
);

INSERT INTO typedb_version (version_name, version_id) VALUES ('dbgdb 1.0', 0x00010000);

-- Files from which types were recovered
DROP TABLE IF EXISTS typedb_files;
CREATE TABLE typedb_files (
    -- unique ID within database
    file_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    -- name of file
    file_name TEXT NOT NULL,
    -- hash of file content
    file_hash TEXT NOT NULL
);

-- List of types
DROP TABLE IF EXISTS typedb_types;
CREATE TABLE typedb_types (
    -- unique ID of type
    type_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    -- name of type
    type_name TEXT NOT NULL,
    file_id INTEGER NOT NULL,
    FOREIGN KEY(file_id) REFERENCES typedb_files(file_id)
);

-- List of fields
DROP TABLE IF EXISTS [typedb_fields];
CREATE TABLE [typedb_fields] (
    -- unique ID of field
    [field_id] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    -- name of field
    [field_name] TEXT NOT NULL,
    [field_type] INTEGER NOT NULL,
    -- unknown, value, vpointer, pointer, reference, array
    [field_kind] INTEGER NOT NULL,
    [field_bits_start] INTEGER NOT NULL,
    [field_bits_count] INTEGER NOT NULL,
    [field_size] INTEGER NOT NULL,
    [field_offset] INTEGER NOT NULL,
    FOREIGN KEY(field_type) REFERENCES typedb_types(type_id)
);

INSERT INTO typedb_files
    (file_id, file_name, file_hash)
VALUES
    (0, "<<builtin>>", "");

INSERT INTO typedb_types
    (type_id, type_name, file_id)
VALUES
    (0, 'char', 0),
    (1, 'char8', 0),
    (2, 'char16', 0),
    (3, 'char32', 0),
    (4, 'wchar_t', 0),
    (5, 'signed char', 0),
    (6, 'unsigned char', 0),
    (7, 'signed short', 0),
    (8, 'unsigned short', 0),
    (9, 'signed int', 0),
    (10, 'unsigned int', 0),
    (11, 'signed long', 0),
    (12, 'unsigned long', 0),
    (13, 'signed long long', 0),
    (14, 'unsigned long long', 0);

)sql", -1, &query, nullptr);

        if (rc_prepare == SQLITE_OK)
        {
            int const rc_step = ::sqlite3_step(query);
            if (rc_step == SQLITE_DONE)
            {
                sqlite3_finalize(query);
                return Result::Success;
            }
        }

        return Result::FailedCreateSchema;
    }
}

namespace DbgDb::Lib
{
    class SqliteSession final : public ISession
    {
    private:
        ::sqlite3* m_Connection;

    public:
        SqliteSession(::sqlite3* connection) noexcept
            : m_Connection{ connection }
        {
        }

        virtual ~SqliteSession() noexcept override final
        {
            if (m_Connection != nullptr)
            {
                ::sqlite3_close(m_Connection);
            }
        }

        virtual Result RemoveType(const char* name) noexcept override final
        {
            (void)name;
            return Result::NotImplemented;
        }

        virtual Result RemoveProvider(const char* name) noexcept override final
        {
            (void)name;
            return Result::NotImplemented;
        }
    };


    DBGDBLIB_EXPORT std::unique_ptr<ISession> CreateSession(std::string path, Result& result) noexcept
    {
        ::sqlite3* connection{};
        ::sqlite3_open(path.c_str(), &connection);

        if (connection != nullptr)
        {
            result = DbgDb::Lib::CreateSchema(connection);
            if (result == Result::Success)
            {
                return std::make_unique<SqliteSession>(connection);
            }

            return nullptr;
        }

        result = Result::FailedCreateDatabase;
        return nullptr;
    }
}
