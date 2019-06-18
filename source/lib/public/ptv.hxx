#pragma once
#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <optional>

#if defined(PTV_BUILDING_LIB)
#define PTV_API __declspec(dllexport)
#else
#define PTV_API __declspec(dllimport)
#endif

namespace ptv
{
    enum class pdb_member_type
    {
        inherited,
        field,
        padding,
        ending,
    };

    class pdb_abstract_type_member
    {
    private:
        uint64_t m_size{};
        uint64_t m_offset{};

    public:
        pdb_abstract_type_member(uint64_t size, uint64_t offset) noexcept
            : m_size{ size }
            , m_offset{ offset }
        {
        }

        virtual ~pdb_abstract_type_member() noexcept = default;

    public:
        virtual pdb_member_type get_member_type() const noexcept = 0;

        uint64_t get_size() const noexcept
        {
            return m_size;
        }

        uint64_t get_offset() const noexcept
        {
            return m_offset;
        }

        uint64_t get_next_offset() const noexcept
        {
            return m_offset + m_size;
        }
    };

    class pdb_member_inherited : public pdb_abstract_type_member
    {
    private:
        std::wstring m_name;
        std::vector<std::unique_ptr<pdb_abstract_type_member>> m_members;

    public:
        pdb_member_inherited(uint64_t size, uint64_t offset, std::wstring_view name, std::vector<std::unique_ptr<pdb_abstract_type_member>>&& members) noexcept
            : pdb_abstract_type_member{ size, offset }
            , m_name{ name }
            , m_members{ std::move(members) }
        {
        }

        virtual ~pdb_member_inherited() noexcept = default;

    public:
        virtual pdb_member_type get_member_type() const noexcept override
        {
            return pdb_member_type::inherited;
        }

        virtual const std::vector<std::unique_ptr<pdb_abstract_type_member>>& get_members() const noexcept
        {
            return m_members;
        }

        std::wstring_view get_name() const noexcept
        {
            return m_name;
        }
    };

    class pdb_member_padding : public pdb_abstract_type_member
    {
    public:
        pdb_member_padding(uint64_t size, uint64_t offset) noexcept
            : pdb_abstract_type_member{ size, offset }
        {
        }

        virtual ~pdb_member_padding() noexcept = default;

    public:
        virtual pdb_member_type get_member_type() const noexcept override
        {
            return pdb_member_type::padding;
        }
    };

    class pdb_member_ending : public pdb_abstract_type_member
    {
    public:
        pdb_member_ending(uint64_t size, uint64_t offset) noexcept
            : pdb_abstract_type_member{ size, offset }
        {
        }

        virtual ~pdb_member_ending() noexcept = default;

    public:
        virtual pdb_member_type get_member_type() const noexcept override
        {
            return pdb_member_type::ending;
        }
    };

    class pdb_member_field : public pdb_abstract_type_member
    {
    private:
        std::wstring_view m_name;
        std::wstring m_type_name;
        std::optional<std::pair<uint64_t, uint64_t>> m_bits;

    public:
        pdb_member_field(
            uint64_t size,
            uint64_t offset,
            std::wstring_view name,
            std::wstring_view type_name,
            std::optional<std::pair<uint64_t, uint64_t>> bits
        ) noexcept
            : pdb_abstract_type_member{ size, offset }
            , m_name{ name }
            , m_type_name{ type_name }
            , m_bits{ bits }
        {
        }

        virtual ~pdb_member_field() noexcept = default;

    public:
        virtual pdb_member_type get_member_type() const noexcept override
        {
            return pdb_member_type::field;
        }

        std::wstring_view get_name() const noexcept
        {
            return { m_name };
        }

        std::wstring_view get_type_name() const noexcept
        {
            return m_type_name;
        }

        std::optional<std::pair<uint64_t, uint64_t>> get_bit_start() const noexcept
        {
            return m_bits;
        }
    };
}

namespace ptv
{
    struct pdb_type final
    {
        std::wstring_view name;
    };

    class pdb_type_descriptor
    {
    private:
        std::vector<std::unique_ptr<pdb_abstract_type_member>> m_members;

    public:
        pdb_type_descriptor(std::vector<std::unique_ptr<pdb_abstract_type_member>>&& members) noexcept
            : m_members{ std::move(members) }
        {
        }

        const std::vector<std::unique_ptr<pdb_abstract_type_member>>& get_members() const noexcept
        {
            return m_members;
        }
    };

    class pdb_file
    {
    public:
        virtual ~pdb_file() noexcept = default;

        virtual bool load(std::wstring_view path) noexcept = 0;

    public:
        virtual const std::vector<pdb_type>& get_types() const noexcept = 0;

        virtual std::unique_ptr<pdb_type_descriptor> get_descriptor(const pdb_type& type) const noexcept = 0;
    };

    PTV_API std::unique_ptr<pdb_file> create() noexcept;
}
