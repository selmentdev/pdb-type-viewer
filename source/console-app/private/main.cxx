#include <ptv.hxx>
#include <ptv/pdb_member_inherited.hxx>
#include <ptv/pdb_member_field.hxx>
#include <ptv/pdb_member_padding.hxx>
#include <iostream>

std::size_t compute_padding(const ptv::pdb_abstract_type_member& member) noexcept
{
    std::size_t result{};

    switch (member.get_member_type())
    {
    case ptv::pdb_member_type::padding:
        {
            result += member.get_size();
            break;
        }
    case ptv::pdb_member_type::inherited:
        {
            auto const& inherited = static_cast<const ptv::pdb_member_inherited&>(member);

            for (auto const& m : inherited.get_members())
            {
                result += compute_padding(*m);
            }

            break;
        }
    }

    return result;
}

std::size_t compute_padding(const ptv::pdb_type_descriptor& descriptor) noexcept
{
    std::size_t result{};

    for (auto const& m : descriptor.get_members())
    {
        result += compute_padding(*m);
    }

    return result;
}

void print_member(uint32_t level, const ptv::pdb_abstract_type_member& member) noexcept
{
    for (uint32_t i = 0; i < (level * 4); ++i)
    {
        std::putwchar(L' ');
    }

    switch (member.get_member_type())
    {
    case ptv::pdb_member_type::padding:
        {
            auto const& padding = static_cast<const ptv::pdb_member_padding&>(member);

            std::wprintf(L"<<padding>> offset: %zu size: %zu", member.get_offset(), member.get_size());

            if (padding.is_spurious())
            {
                std::fputws(L" spurious", stdout);
            }

            std::putwchar(L'\n');
            break;
        }
    case ptv::pdb_member_type::inherited:
        {
            auto const& inherited = static_cast<const ptv::pdb_member_inherited&>(member);

            std::wprintf(
                L"%s [offset: %zu, size: %zu]\n",
                std::wstring{ inherited.get_name() }.c_str(),
                inherited.get_offset(),
                inherited.get_size()
            );

            for (auto const& m : inherited.get_members())
            {
                print_member(level + 1, *m);
            }

            break;
        }
    case ptv::pdb_member_type::field:
        {
            auto const& field = static_cast<const ptv::pdb_member_field&>(member);

            std::wprintf(
                L"%s %s",
                std::wstring{ field.get_type_name() }.c_str(),
                std::wstring{ field.get_name() }.c_str()
            );

            if (auto bits = field.get_bits(); bits.has_value())
            {
                std::wprintf(
                    L" <%u:%u>",
                    static_cast<uint32_t>(bits.value().first),
                    static_cast<uint32_t>(bits.value().second)
                );
            }

            std::wprintf(
                L" [offset: %zu, size: %zu]\n",
                field.get_offset(),
                field.get_size()
            );

            break;
        }
    }
}

void print_type(const ptv::pdb_type& type, const ptv::pdb_type_descriptor& descriptor) noexcept
{
    auto const padding = compute_padding(descriptor);

    if (padding != 0)
    {
        std::wprintf(
            L"Type: %s, Padding: %zu\n",
            std::wstring{ type.get_name() }.c_str(),
            padding
        );

        for (auto const& m : descriptor.get_members())
        {
            print_member(1, *m);
        }

        std::wprintf(L"\n\n");
    }
}

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        auto session = ptv::create();
        auto path = ptv::helpers::convert(argv[1]);

        if (session->load(path, {}))
        {
            for (auto const& type : session->get_types())
            {
                if (auto descriptor = session->get_descriptor(*type); descriptor != nullptr)
                {
                    print_type(*type, *descriptor);
                }
            }
        }
        else
        {
            std::fwprintf(stderr, L"Cannot load file: %s\n", path.c_str());
        }
    }

    return 0;
}
