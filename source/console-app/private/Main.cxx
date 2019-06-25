#include <LibPtv.hxx>
#include <LibPtv/TypeMemberInherited.hxx>
#include <LibPtv/TypeMemberField.hxx>
#include <LibPtv/TypeMemberPadding.hxx>
#include <iostream>

std::size_t compute_padding(const LibPdb::BaseTypeMember& member) noexcept
{
    std::size_t result{};

    switch (member.GetMemberType())
    {
    case LibPdb::MemberType::Padding:
        {
            result += member.GetSize();
            break;
        }
    case LibPdb::MemberType::Inherited:
        {
            auto const& inherited = static_cast<const LibPdb::TypeMemberInherited&>(member);

            for (auto const& m : inherited.GetMembers())
            {
                result += compute_padding(*m);
            }

            break;
        }
    }

    return result;
}

std::size_t compute_padding(const LibPdb::TypeDescriptor& descriptor) noexcept
{
    std::size_t result{};

    for (auto const& m : descriptor.GetMembers())
    {
        result += compute_padding(*m);
    }

    return result;
}

void print_member(uint32_t level, const LibPdb::BaseTypeMember& member) noexcept
{
    for (uint32_t i = 0; i < (level * 4); ++i)
    {
        std::putwchar(L' ');
    }

    switch (member.GetMemberType())
    {
    case LibPdb::MemberType::Padding:
        {
            auto const& padding = static_cast<const LibPdb::TypeMemberPadding&>(member);

            std::wprintf(L"<<padding>> offset: %zu size: %zu", member.GetOffset(), member.GetSize());

            if (padding.IsSpurious())
            {
                std::fputws(L" spurious", stdout);
            }

            std::putwchar(L'\n');
            break;
        }
    case LibPdb::MemberType::Inherited:
        {
            auto const& inherited = static_cast<const LibPdb::TypeMemberInherited&>(member);

            std::wprintf(
                L"%s [offset: %zu, size: %zu]\n",
                std::wstring{ inherited.GetName() }.c_str(),
                inherited.GetOffset(),
                inherited.GetSize()
            );

            for (auto const& m : inherited.GetMembers())
            {
                print_member(level + 1, *m);
            }

            break;
        }
    case LibPdb::MemberType::Field:
        {
            auto const& field = static_cast<const LibPdb::TypeMemberField&>(member);

            std::wprintf(
                L"%s %s",
                std::wstring{ field.GetTypeName() }.c_str(),
                std::wstring{ field.GetName() }.c_str()
            );

            if (auto bits = field.GetBits(); bits.has_value())
            {
                std::wprintf(
                    L" <%u:%u>",
                    static_cast<uint32_t>(bits.value().first),
                    static_cast<uint32_t>(bits.value().second)
                );
            }

            std::wprintf(
                L" [offset: %zu, size: %zu]\n",
                field.GetOffset(),
                field.GetSize()
            );

            break;
        }
    }
}

void print_type(const LibPdb::Type& type, const LibPdb::TypeDescriptor& descriptor) noexcept
{
    auto const padding = compute_padding(descriptor);

    if (padding != 0)
    {
        std::wprintf(
            L"Type: %s, Padding: %zu\n",
            std::wstring{ type.GetName() }.c_str(),
            padding
        );

        for (auto const& m : descriptor.GetMembers())
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
        auto session = LibPdb::CreateSession();
        auto path = LibPdb::Helpers::Convert(argv[1]);

        if (session->Load(path, {}))
        {
            for (auto const& type : session->GetTypes())
            {
                if (auto descriptor = session->GetDescriptor(*type); descriptor != nullptr)
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
