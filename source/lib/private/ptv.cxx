#define PTV_BUILDING_LIB
#define NOMINMAX

#include <wrl/client.h>
#include <dia2.h>
#include <diacreate.h>
#include <optional>
#include <algorithm>
#include <map>
#include <set>

#include <ptv.hxx>
#include <ptv/pdb_abstract_type_member.hxx>
#include <ptv/pdb_member_ending.hxx>
#include <ptv/pdb_member_field.hxx>
#include <ptv/pdb_member_inherited.hxx>
#include <ptv/pdb_member_padding.hxx>

namespace ptv::dia
{
    Microsoft::WRL::ComPtr<IDiaSymbol> next(
        Microsoft::WRL::ComPtr<IDiaEnumSymbols> enumerator
    ) noexcept
    {
        Microsoft::WRL::ComPtr<IDiaSymbol> result{};
        ULONG fetched{};

        enumerator->Next(
            1,
            result.GetAddressOf(),
            &fetched
        );

        return result;
    }

    Microsoft::WRL::ComPtr<IDiaEnumSymbols> find_children(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol,
        enum SymTagEnum symtag
    ) noexcept
    {
        Microsoft::WRL::ComPtr<IDiaEnumSymbols> result{};

        symbol->findChildrenEx(
            symtag,
            nullptr,
            nsNone,
            result.GetAddressOf()
        );

        return result;
    }

    std::wstring_view name(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        BSTR result{};
        [[maybe_unused]] HRESULT hr = symbol->get_name(&result);

        if (result != nullptr)
        {
            return { result };
        }

        return {};
    }

    uint32_t rank(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        DWORD result{};
        symbol->get_rank(&result);

        return static_cast<uint32_t>(result);
    }

    uint32_t count(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        DWORD result{};
        symbol->get_count(&result);

        return static_cast<uint32_t>(result);
    }

    uint64_t length(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        ULONGLONG result{};
        HRESULT hr = symbol->get_length(&result);
        (void)hr;
        return static_cast<uint64_t>(result);
    }

    int64_t offset(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        LONG result{};
        symbol->get_offset(&result);

        return static_cast<int64_t>(result);
    }

    uint32_t bit_position(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        DWORD result{};
        symbol->get_bitPosition(&result);

        return static_cast<uint32_t>(result);
    }

    bool is_const(
        Microsoft::WRL::ComPtr<IDiaSymbol> type
    ) noexcept
    {
        BOOL result{};
        type->get_constType(&result);

        return result != FALSE;
    }

    bool is_volatile(
        Microsoft::WRL::ComPtr<IDiaSymbol> type
    ) noexcept
    {
        BOOL result{};
        type->get_volatileType(&result);

        return result != FALSE;
    }

    bool is_unaligned(
        Microsoft::WRL::ComPtr<IDiaSymbol> type
    ) noexcept
    {
        BOOL result{};
        type->get_unalignedType(&result);

        return result != FALSE;
    }

    bool is_reference(
        Microsoft::WRL::ComPtr<IDiaSymbol> type
    ) noexcept
    {
        BOOL result{};
        type->get_reference(&result);

        return result != FALSE;
    }

    UdtKind udt_kind(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        DWORD result{};
        symbol->get_udtKind(&result);

        return static_cast<UdtKind>(result);
    }

    BasicType get_base_type(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        DWORD result{};
        symbol->get_baseType(&result);

        return static_cast<BasicType>(result);
    }
    std::wstring get_base_type_name(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        auto type = dia::get_base_type(symbol);

        switch (type)
        {
        case btNoType:
            return L"<no-type>";
        case btVoid:
            return L"void";
        case btChar:
            return L"char";
        case btWChar:
            return L"wchar_t";
        case btInt:
        case btUInt:
            {
                std::wstring result = ((type == btUInt) ? L"unsigned " : L"");

                auto const length = dia::length(symbol);

                switch (length)
                {
                case 1:
                    result += L"char";
                    break;
                case 2:
                    result += L"short";
                    break;
                case 4:
                    result += L"int";
                    break;
                case 8:
                    result += L"long long";
                    break;
                }

                return result;
            }
        case btFloat:
            {
                auto const length = dia::length(symbol);

                if (length == 4)
                {
                    return L"float";
                }
                else if (length == 8)
                {
                    return L"double";
                }
                else if (length == 2)
                {
                    return L"half";
                }

                return L"<unknown-float>";
            }
        case btBCD:
            return L"<bcd>";
        case btBool:
            return L"bool";
        case btLong:
            return L"long";
        case btULong:
            return L"unsigned long";
        case btCurrency:
            return L"<currency>";
        case btDate:
            return L"<date>";
        case btVariant:
            return L"<variant>";
        case btComplex:
            return L"<complex>";
        case btBit:
            return L"<bit>";
        case btBSTR:
            return L"BSTR";
        case btHresult:
            return L"HRESULT";
        case btChar16:
            return L"char16_t";
        case btChar32:
            return L"char32_t";
#if _MSC_VER >= 1920
        case btChar8:
            return L"char8_t";
#endif
        default:
            return L"<unknown>";
        }
    }

    Microsoft::WRL::ComPtr<IDiaSymbol> type(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        Microsoft::WRL::ComPtr<IDiaSymbol> result{};

        symbol->get_type(result.GetAddressOf());

        return result;
    }

    std::optional<enum SymTagEnum> symtag(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        DWORD result{};

        if (FAILED(symbol->get_symTag(&result)))
        {
            return std::nullopt;
        }

        return static_cast<enum SymTagEnum>(result);
    }

    std::optional<bool> is_static(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        BOOL result{};

        if (FAILED(symbol->get_isStatic(&result)))
        {
            return std::nullopt;
        }

        return (result != FALSE);
    }

    std::optional<LocationType> location(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        DWORD result{};

        if (FAILED(symbol->get_locationType(&result)))
        {
            return std::nullopt;
        }

        return static_cast<LocationType>(result);
    }
}

namespace ptv::helpers
{
    std::wstring convert(
        std::string_view value
    ) noexcept
    {
        std::wstring result{};

        if (!value.empty())
        {
            auto const length = static_cast<int>(value.length()); // XXX: Check for overflow

            auto const converted1 = MultiByteToWideChar(CP_UTF8, 0, &value[0], length, nullptr, 0);

            result.resize(static_cast<size_t>(converted1), L'\0');

            [[maybe_unused]] auto const converted2 = MultiByteToWideChar(CP_UTF8, 0, &value[0], length, &result[0], converted1);
        }

        return result;
    }

    std::string convert(
        std::wstring_view value
    ) noexcept
    {
        std::string result{};

        if (!value.empty())
        {
            auto const length = static_cast<int>(value.length()); // XXX: Check for overflow

            auto const converted1 = WideCharToMultiByte(CP_UTF8, 0, &value[0], length, nullptr, 0, nullptr, nullptr);

            result.resize(static_cast<size_t>(converted1), '\0');

            [[maybe_unused]] auto const converted2 = WideCharToMultiByte(CP_UTF8, 0, &value[0], length, &result[0], converted1, nullptr, nullptr);
        }

        return result;
    }
}

namespace ptv::impl
{
    class pdb_file_impl : public pdb_file
    {
    private:
        Microsoft::WRL::ComPtr<IDiaSession> m_session;
        Microsoft::WRL::ComPtr<IDiaDataSource> m_source;
        Microsoft::WRL::ComPtr<IDiaSymbol> m_global_scope;
        std::vector<pdb_type> m_types;

    public:
        pdb_file_impl() noexcept = default;
        virtual ~pdb_file_impl() noexcept = default;

        virtual bool load(
            std::wstring_view path
        ) noexcept override
        {
            Microsoft::WRL::ComPtr<IDiaDataSource> source{};

            if (FAILED(NoRegCoCreate(
                L"msdia140.dll",
                CLSID_DiaSource,
                IID_IDiaDataSource,
                (void**)source.GetAddressOf()
            )))
            {
                return false;
            }

            if (FAILED(source->loadDataFromPdb(
                std::wstring{ path }.c_str()
            )))
            {
                return false;
            }

            Microsoft::WRL::ComPtr<IDiaSession> session{};

            if (FAILED(source->openSession(
                session.GetAddressOf()
            )))
            {
                return false;
            }

            Microsoft::WRL::ComPtr<IDiaSymbol> global_scope{};

            if (FAILED(session->get_globalScope(
                global_scope.GetAddressOf()
            )))
            {
                return false;
            }


            std::set<std::wstring_view> unique_types{};

            if (auto enum_types = dia::find_children(global_scope, SymTagUDT); enum_types != nullptr)
            {
                for (auto child = dia::next(enum_types); child != nullptr; child = dia::next(enum_types))
                {
                    auto name = dia::name(child);
                    unique_types.insert(name);
                }
            }

            std::vector<ptv::pdb_type> types{};
            types.reserve(unique_types.size());

            for (auto const& unique : unique_types)
            {
                types.push_back({ unique });
            }

            m_types = std::move(types);
            m_session = std::move(session);
            m_source = std::move(source);
            m_global_scope = std::move(global_scope);

            return true;
        }


        virtual const std::vector<pdb_type>& get_types() const noexcept override
        {
            return m_types;
        }

        static std::wstring get_function_type_name(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            std::wstring result{};

            Microsoft::WRL::ComPtr<IDiaSymbol> type{};

            symbol->get_type(type.GetAddressOf());

            if (type != nullptr)
            {
                result += get_type_name(type);
            }

            result += dia::name(symbol);

            std::vector<std::wstring_view> names{};

            auto children = dia::find_children(symbol, SymTagNull);

            result += L"(";

            if (children != nullptr)
            {
                auto child = dia::next(children);

                if (child != nullptr)
                {
                    result += get_type_name(child);

                    child = dia::next(children);

                    while (child != nullptr)
                    {
                        result += L", ";

                        result += get_type_name(child);

                        child = dia::next(children);
                    }
                }
            }

            result += L")";

            return result;
        }

        static std::wstring get_bound(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            return L"<bound>";
        }

        static std::wstring get_array_name(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            std::wstring result{};

            if (symbol != nullptr)
            {
                auto type = dia::type(symbol);

                result += get_type_name(type);

                auto done = false;
                if (auto rank = dia::rank(symbol); rank != 0)
                {
                    if (auto bounds = dia::find_children(symbol, SymTagDimension); bounds != nullptr)
                    {
                        for (auto bound = dia::next(bounds); bound != nullptr; bound = dia::next(bounds))
                        {
                            done = true;

                            Microsoft::WRL::ComPtr<IDiaSymbol> lower{};
                            Microsoft::WRL::ComPtr<IDiaSymbol> upper{};

                            bound->get_lowerBound(lower.GetAddressOf());
                            bound->get_upperBound(upper.GetAddressOf());

                            result += L"[";
                            result += get_bound(lower);
                            result += L"..";
                            result += get_bound(upper);
                            result += L"]";
                        }
                    }
                }

                if (!done)
                {
                    if (auto bounds = dia::find_children(symbol, SymTagCustomType); bounds != nullptr)
                    {
                        for (auto bound = dia::next(bounds); bound != nullptr; bound = dia::next(bounds))
                        {
                            done = true;

                            result += L"[";
                            result += get_type_name(bound);
                            result += L"]";
                        }
                    }
                }

                if (!done)
                {
                    if (auto const count = dia::count(symbol); count != 0)
                    {
                        done = true;
                        result += L"[";
                        result += std::to_wstring(count);
                        result += L"]";
                    }
                }

                if (!done)
                {
                    auto const array_length = dia::length(symbol);
                    auto const type_length = dia::length(type);

                    done = true;

                    auto count = array_length;

                    if (type_length != 0)
                    {
                        count /= type_length;
                    }

                    result += L"[";
                    result += std::to_wstring(count);
                    result += L"]";
                }

                if (!done)
                {
                    result = L"<array>[]";
                }
            }

            return result;
        }

        static std::wstring get_type_name(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            std::wstring result{};

            if (symbol != nullptr)
            {
                auto tag = dia::symtag(symbol);

                switch (tag.value_or(SymTagNull))
                {
                case SymTagFunctionArgType:
                    {
                        result = get_type_name(dia::type(symbol));
                        break;
                    }
                case SymTagFunctionType:
                    {
                        result = get_function_type_name(symbol);
                        break;
                    }
                case SymTagPointerType:
                    {
                        result = get_type_name(dia::type(symbol));

                        if (dia::is_const(symbol))
                        {
                            result += L"const ";
                        }

                        if (dia::is_volatile(symbol))
                        {
                            result += L"volatile ";
                        }

                        if (dia::is_unaligned(symbol))
                        {
                            result += L"__unaligned ";
                        }

                        if (dia::is_reference(symbol))
                        {
                            result += L"& ";
                        }
                        else
                        {
                            result += L"* ";
                        }

                        break;
                    }
                case SymTagArrayType:
                    {
                        result = get_array_name(symbol);
                        break;
                    }
                case SymTagVTable:
                case SymTagVTableShape:
                    {
                        result = L"<vtable>";
                        break;
                    }
                case SymTagUDT:
                    {
                        auto kind = dia::udt_kind(symbol);

                        switch (kind)
                        {
                        case UdtStruct:
                            {
                                result += L"struct ";
                                break;
                            }
                        case UdtClass:
                            {
                                result += L"class ";
                                break;
                            }
                        case UdtUnion:
                            {
                                result += L"union ";
                                break;
                            }
                        case UdtInterface:
                            {
                                result += L"interface ";
                                break;
                            }
                        }

                        result += dia::name(symbol);

                        break;
                    }
                case SymTagEnum:
                    {
                        result = L"enum ";
                        result += dia::name(symbol);
                        break;
                    }
                case SymTagBaseType:
                    {
                        result = dia::get_base_type_name(symbol);
                        break;
                    }
                }
            }

            return result;
        }

        static std::optional<bool> is_valid_member(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            auto symtag = dia::symtag(symbol);

            if (!symtag.has_value())
            {
                return std::nullopt;
            }

            switch (symtag.value())
            {
            case SymTagFunction:
            case SymTagFriend:
            case SymTagEnum:
            case SymTagUDT:
            case SymTagBaseType:
            case SymTagTypedef:
                return false;
            }

            auto is_static = dia::is_static(symbol);

            if (!is_static.has_value())
            {
                return std::nullopt;
            }

            if (is_static.value())
            {
                return false;
            }

            auto location = dia::location(symbol);

            if (!location.has_value())
            {
                return std::nullopt;
            }

            if (location != LocIsThisRel && location != LocIsNull && location != LocIsBitField)
            {
                return false;
            }

            return true;
        }

        static std::unique_ptr<pdb_member_inherited> create_member_inherited(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            //
            // Get basic type info.
            //

            auto const type_name = dia::name(symbol);
            auto const offset = dia::offset(symbol);
            auto const size = dia::length(symbol);


            //
            // Enumerate native children symbols.
            //

            std::vector<Microsoft::WRL::ComPtr<IDiaSymbol>> native_symbols{};

            if (auto children = dia::find_children(symbol, SymTagNull); children != nullptr)
            {
                for (auto child = dia::next(children); child != nullptr; child = dia::next(children))
                {
                    native_symbols.push_back(child);
                }
            }

            //
            // Sort them by offset.
            //
            // NOTE: is this required?
            //
#if false
            std::sort(
                native_symbols.begin(),
                native_symbols.end(),
                [](const Microsoft::WRL::ComPtr<IDiaSymbol>& lhs, const Microsoft::WRL::ComPtr<IDiaSymbol>& rhs) noexcept
                {
                    return dia::offset(lhs) < dia::offset(rhs);
                }
            );
#endif


            //
            // Create wrapper for applicable symbols.
            //

            std::vector<std::unique_ptr<pdb_abstract_type_member>> symbols{};

            for (auto const& current : native_symbols)
            {
                if (auto item = create(current); item != nullptr)
                {
                    symbols.push_back(std::move(item));
                }
            }


            //
            // Try to find paddings
            //

            std::set<std::pair<uint64_t, uint64_t>> paddings{};

            uint64_t highest_ending{};

            for (auto const& current : symbols)
            {
                //
                // Compute highest padding value.

                highest_ending = std::max(highest_ending, current->get_next_offset());

                //
                // Get symbols before current one.
                //

                std::vector<ptv::pdb_abstract_type_member*> lesser{};

                for (auto const& before : symbols)
                {
                    if (before->get_next_offset() <= current->get_offset())
                    {
                        lesser.push_back(before.get());
                    }
                }


                //
                // Try find symbol with closest padding value.
                //

                auto it = std::min_element(
                    std::begin(lesser),
                    std::end(lesser),
                    [&](const ptv::pdb_abstract_type_member* lhs, const ptv::pdb_abstract_type_member* rhs)
                    {
                        return (current->get_offset() - lhs->get_next_offset()) < (current->get_offset() - rhs->get_next_offset());
                    }
                );

                if (it != std::end(lesser))
                {
                    //
                    // Compute offset and padding between this symbol and adjacent.
                    //

                    auto const final_offset = (*it)->get_next_offset();
                    auto const final_padding = current->get_offset() - final_offset;

                    if (final_padding != 0)
                    {
                        //
                        // Padding found. Remember it.
                        //

                        paddings.insert({ final_padding, final_offset });
                    }
                }
            }


            //
            // Insert additional paddings to symbols list.
            //

            for (auto const& padding : paddings)
            {
                symbols.push_back(std::make_unique<pdb_member_padding>(padding.first, padding.second));
            }


            //
            // Sort symbols by offset, then size.
            //

            std::sort(
                symbols.begin(),
                symbols.end(),
                [](const std::unique_ptr<pdb_abstract_type_member>& lhs, const std::unique_ptr<pdb_abstract_type_member>& rhs) noexcept
                {
                    if (lhs->get_offset() == rhs->get_offset())
                    {
                        return lhs->get_size() < rhs->get_size();
                    }

                    return lhs->get_offset() < rhs->get_offset();
                }
            );


            //
            // Check if whole type has leading padding value.
            //

            auto const type_size = dia::length(symbol);
            if (auto const final_padding = type_size - highest_ending; final_padding != 0)
            {
                if (!symbols.empty())
                {
                    //
                    // If type is not empty, this is real padding. Empty types must have at least
                    // one byte, but we don't want to report it as memory-wasting padding.
                    //

                    symbols.push_back(std::make_unique<pdb_member_ending>(final_padding, highest_ending));
                }
            }


            //
            // TODO:
            //      Because DIA SDK reports sizeof 0 for some type members, we need to mark them as
            //      missing detailed size information.
            //
            //      This requires us to find sequence:
            //
            //          <symbol, offset:x, size:0>
            //          <padding, offset:x, size:not 0>
            //
            //      and replace them with marker.
            //

            for (size_t i = 0, count = symbols.size() - 1; i < count; ++i)
            {
                auto const& first = symbols[i];
                if (first->get_member_type() == pdb_member_type::field)
                {
                    auto& second = symbols[i + 1];

                    if (second->get_member_type() == pdb_member_type::padding)
                    {
                        auto& typed = static_cast<ptv::pdb_member_padding&>(*second);
                        typed.set_spurious(true);
                    }
                    else if (second->get_member_type() == pdb_member_type::ending)
                    {
                        auto& typed = static_cast<ptv::pdb_member_ending&>(*second);
                        typed.set_spurious(true);
                    }
                }
            }

            return std::make_unique<pdb_member_inherited>(
                dia::length(symbol),
                dia::offset(symbol),
                type_name,
                std::move(symbols)
            );
        }

        static std::unique_ptr<pdb_member_field> create_member_field(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            auto type = dia::type(symbol);

            auto size = dia::length(type);
            auto const offset = static_cast<uint64_t>(dia::offset(symbol));
            auto const name = dia::name(symbol);
            auto const type_name = get_type_name(type);
            auto const location = dia::location(symbol);

            std::optional<std::pair<uint64_t, uint64_t>> bits{};

            if (location.has_value() && location.value() == LocIsBitField)
            {
                bits = {
                    dia::bit_position(symbol),
                    dia::length(symbol),
                };
            }
            
            return std::make_unique<pdb_member_field>(
                size,
                offset,
                name,
                type_name,
                bits
            );
        }

        static std::unique_ptr<pdb_abstract_type_member> create(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            auto symtag = dia::symtag(symbol);

            if (!symtag.has_value())
            {
                return nullptr;
            }

            if (symtag == SymTagBaseClass)
            {
                return create_member_inherited(symbol);
            }
            else if (auto valid = is_valid_member(symbol); valid.value_or(false))
            {
                return create_member_field(symbol);
            }

            return nullptr;
        }

        virtual std::unique_ptr<pdb_type_descriptor> get_descriptor(
            const pdb_type& type
        ) const noexcept override
        {
            Microsoft::WRL::ComPtr<IDiaEnumSymbols> enum_symbols{};

            if (FAILED(m_global_scope->findChildrenEx(
                SymTagNull,
                std::wstring{ type.name }.c_str(),
                nsNone,
                enum_symbols.GetAddressOf()
            )))
            {
                return {};
            }

            LONG count{};
            if (FAILED(enum_symbols->get_Count(&count)) || count == 0)
            {
                return {};
            }

            Microsoft::WRL::ComPtr<IDiaSymbol> symbol{};

            ULONG fetched{};
            if (FAILED(enum_symbols->Next(1, symbol.GetAddressOf(), &fetched)) || fetched == 0)
            {
                return {};
            }

            std::vector<std::unique_ptr<pdb_abstract_type_member>> members{};
            members.emplace_back(create_member_inherited(symbol));

            return std::make_unique<pdb_type_descriptor>(std::move(members));
        }
    };
}

namespace ptv
{
    PTV_API std::unique_ptr<pdb_file> create() noexcept
    {
        return std::make_unique<impl::pdb_file_impl>();
    }
}
