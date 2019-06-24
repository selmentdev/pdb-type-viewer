#define PTV_BUILDING_LIB
#define NOMINMAX

#include <wrl/client.h>
#include <dia2.h>
#include <diacreate.h>
#include <optional>
#include <algorithm>
#include <map>
#include <set>

#include <LibPtv.hxx>
#include <LibPtv/BaseTypeMember.hxx>
#include <LibPtv/TypeMemberField.hxx>
#include <LibPtv/TypeMemberInherited.hxx>
#include <LibPtv/TypeMemberPadding.hxx>

namespace LibPdb::MsDia
{
    Microsoft::WRL::ComPtr<IDiaSymbol> Next(
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

    Microsoft::WRL::ComPtr<IDiaEnumSymbols> FindChildren(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol,
        enum SymTagEnum symtag
    ) noexcept
    {
        Microsoft::WRL::ComPtr<IDiaEnumSymbols> result{};

        HRESULT hr = symbol->findChildrenEx(
            symtag,
            nullptr,
            nsNone,
            result.GetAddressOf()
        );

        (void)hr;

        return result;
    }

    int32_t GetCount(
        Microsoft::WRL::ComPtr<IDiaEnumSymbols> enumerator
    ) noexcept
    {
        LONG result{};
        enumerator->get_Count(&result);
        return static_cast<int32_t>(result);
    }

    std::wstring_view GetName(
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

    uint32_t GetRank(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        DWORD result{};
        symbol->get_rank(&result);

        return static_cast<uint32_t>(result);
    }

    uint32_t GetCount(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        DWORD result{};
        symbol->get_count(&result);

        return static_cast<uint32_t>(result);
    }

    uint64_t GetLength(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        ULONGLONG result{};
        HRESULT hr = symbol->get_length(&result);
        (void)hr;
        return static_cast<uint64_t>(result);
    }

    int64_t GetOffset(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        LONG result{};
        symbol->get_offset(&result);

        return static_cast<int64_t>(result);
    }

    uint32_t GetBitPosition(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        DWORD result{};
        symbol->get_bitPosition(&result);

        return static_cast<uint32_t>(result);
    }

    bool IsConst(
        Microsoft::WRL::ComPtr<IDiaSymbol> type
    ) noexcept
    {
        BOOL result{};
        type->get_constType(&result);

        return result != FALSE;
    }

    bool IsVolatile(
        Microsoft::WRL::ComPtr<IDiaSymbol> type
    ) noexcept
    {
        BOOL result{};
        type->get_volatileType(&result);

        return result != FALSE;
    }

    bool IsUnaligned(
        Microsoft::WRL::ComPtr<IDiaSymbol> type
    ) noexcept
    {
        BOOL result{};
        type->get_unalignedType(&result);

        return result != FALSE;
    }

    bool IsReference(
        Microsoft::WRL::ComPtr<IDiaSymbol> type
    ) noexcept
    {
        BOOL result{};
        type->get_reference(&result);

        return result != FALSE;
    }

    UdtKind GetUdtKind(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        DWORD result{};
        symbol->get_udtKind(&result);

        return static_cast<UdtKind>(result);
    }

    BasicType GetBaseType(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        DWORD result{};
        symbol->get_baseType(&result);

        return static_cast<BasicType>(result);
    }
    std::wstring GetBaseTypeName(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        auto type = MsDia::GetBaseType(symbol);

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
                std::wstring result = ((type == btUInt) ? L"u" : L"");

                auto const length = MsDia::GetLength(symbol);

                switch (length)
                {
                case 1:
                    result += L"int8";
                    break;
                case 2:
                    result += L"int16";
                    break;
                case 4:
                    result += L"int32";
                    break;
                case 8:
                    result += L"int64";
                    break;
                }

                return result;
            }
        case btFloat:
            {
                auto const length = MsDia::GetLength(symbol);

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

    Microsoft::WRL::ComPtr<IDiaSymbol> GetType(
        Microsoft::WRL::ComPtr<IDiaSymbol> symbol
    ) noexcept
    {
        Microsoft::WRL::ComPtr<IDiaSymbol> result{};

        symbol->get_type(result.GetAddressOf());

        return result;
    }

    std::optional<enum SymTagEnum> GetSymTag(
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

    std::optional<bool> IsStatic(
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

    std::optional<LocationType> GetLocation(
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

namespace LibPdb::Helpers
{
    std::wstring Convert(
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

    std::string Convert(
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

namespace LibPdb::Detail
{
    class TypeImpl : public Type
    {
    private:
        std::wstring m_Name;
        Microsoft::WRL::ComPtr<IDiaSymbol> m_symbol;

    public:
        TypeImpl(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
            : m_Name{ MsDia::GetName(symbol) }
            , m_symbol{ symbol }
        {
        }

        virtual ~TypeImpl() noexcept = default;

        virtual std::wstring_view GetName() const noexcept override
        {
            return m_Name;
        }

        const Microsoft::WRL::ComPtr<IDiaSymbol>& GetSymbol() const noexcept
        {
            return m_symbol;
        }

        virtual std::unique_ptr<Type> Clone() const noexcept override
        {
            return std::make_unique<TypeImpl>(
                this->m_symbol
            );
        }
    };
}

namespace LibPdb::Detail
{
    class SessionImpl : public Session
    {
    private:
        Microsoft::WRL::ComPtr<IDiaSession> m_Session;
        Microsoft::WRL::ComPtr<IDiaDataSource> m_Source;
        Microsoft::WRL::ComPtr<IDiaSymbol> m_GlobalScope;
        std::vector<std::unique_ptr<Type>> m_Types;

    public:
        SessionImpl() noexcept = default;
        virtual ~SessionImpl() noexcept = default;

        virtual bool Load(
            std::wstring_view path,
            std::function<void(int32_t current, int32_t total)> progress
        ) noexcept override
        {
            Microsoft::WRL::ComPtr<IDiaDataSource> source{};

            if (FAILED(NoOleCoCreate(
                CLSID_DiaSourceAlt,
                IID_IDiaDataSource,
                (void**)source.GetAddressOf()
            )))
            {
                //
                // Failed to load msdia from registry.

                if (FAILED(NoRegCoCreate(
                    L"msdia140.dll",
                    CLSID_DiaSourceAlt,
                    IID_IDiaDataSource,
                    (void**)source.GetAddressOf()
                )))
                {
                    return false;
                }
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


            std::map<std::wstring_view, Microsoft::WRL::ComPtr<IDiaSymbol>> unique_types{};

            if (auto enum_types = MsDia::FindChildren(global_scope, SymTagUDT); enum_types != nullptr)
            {
                auto total = MsDia::GetCount(enum_types);
                int32_t current = 0;

                for (auto child = MsDia::Next(enum_types); child != nullptr; child = MsDia::Next(enum_types))
                {
                    ++current;

                    if (progress)
                    {
                        progress(current, total);
                    }

                    auto name = MsDia::GetName(child);
                    unique_types.insert(
                        std::make_pair(
                            name,
                            child
                        )
                    );
                }
            }

            std::vector<std::unique_ptr<LibPdb::Type>> types{};
            types.reserve(unique_types.size());

            for (auto const& unique : unique_types)
            {
                types.push_back(
                    std::move(
                        std::make_unique<LibPdb::Detail::TypeImpl>(
                            unique.second
                        )
                    )
                );
            }

            m_Types = std::move(types);
            m_Session = std::move(session);
            m_Source = std::move(source);
            m_GlobalScope = std::move(global_scope);

            return true;
        }


        virtual const std::vector<std::unique_ptr<Type>>& GetTypes() const noexcept override
        {
            return m_Types;
        }

        static std::wstring GetFunctionTypeName(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            std::wstring result{};

            Microsoft::WRL::ComPtr<IDiaSymbol> type{};

            symbol->get_type(type.GetAddressOf());

            if (type != nullptr)
            {
                result += GetTypeName(type);
            }

            result += MsDia::GetName(symbol);

            std::vector<std::wstring_view> names{};

            auto children = MsDia::FindChildren(symbol, SymTagNull);

            result += L"(";

            if (children != nullptr)
            {
                auto child = MsDia::Next(children);

                if (child != nullptr)
                {
                    result += GetTypeName(child);

                    child = MsDia::Next(children);

                    while (child != nullptr)
                    {
                        result += L", ";

                        result += GetTypeName(child);

                        child = MsDia::Next(children);
                    }
                }
            }

            result += L")";

            return result;
        }

        static std::wstring GetBound(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            return L"<bound>";
        }

        static std::wstring GetArrayName(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            std::wstring result{};

            if (symbol != nullptr)
            {
                auto type = MsDia::GetType(symbol);

                result += GetTypeName(type);

                auto done = false;
                if (auto rank = MsDia::GetRank(symbol); rank != 0)
                {
                    if (auto bounds = MsDia::FindChildren(symbol, SymTagDimension); bounds != nullptr)
                    {
                        for (auto bound = MsDia::Next(bounds); bound != nullptr; bound = MsDia::Next(bounds))
                        {
                            done = true;

                            Microsoft::WRL::ComPtr<IDiaSymbol> lower{};
                            Microsoft::WRL::ComPtr<IDiaSymbol> upper{};

                            bound->get_lowerBound(lower.GetAddressOf());
                            bound->get_upperBound(upper.GetAddressOf());

                            result += L"[";
                            result += GetBound(lower);
                            result += L"..";
                            result += GetBound(upper);
                            result += L"]";
                        }
                    }
                }

                if (!done)
                {
                    if (auto bounds = MsDia::FindChildren(symbol, SymTagCustomType); bounds != nullptr)
                    {
                        for (auto bound = MsDia::Next(bounds); bound != nullptr; bound = MsDia::Next(bounds))
                        {
                            done = true;

                            result += L"[";
                            result += GetTypeName(bound);
                            result += L"]";
                        }
                    }
                }

                if (!done)
                {
                    if (auto const count = MsDia::GetCount(symbol); count != 0)
                    {
                        done = true;
                        result += L"[";
                        result += std::to_wstring(count);
                        result += L"]";
                    }
                }

                if (!done)
                {
                    auto const array_length = MsDia::GetLength(symbol);
                    auto const type_length = MsDia::GetLength(type);

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

        static std::wstring GetTypeName(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            std::wstring result{};

            if (symbol != nullptr)
            {
                auto tag = MsDia::GetSymTag(symbol);

                switch (tag.value_or(SymTagNull))
                {
                case SymTagFunctionArgType:
                    {
                        result = GetTypeName(MsDia::GetType(symbol));
                        break;
                    }
                case SymTagFunctionType:
                    {
                        result = GetFunctionTypeName(symbol);
                        break;
                    }
                case SymTagPointerType:
                    {
                        result = GetTypeName(MsDia::GetType(symbol));

                        if (MsDia::IsConst(symbol))
                        {
                            result += L" const";
                        }

                        if (MsDia::IsVolatile(symbol))
                        {
                            result += L" volatile";
                        }

                        if (MsDia::IsUnaligned(symbol))
                        {
                            result += L" __unaligned";
                        }

                        if (MsDia::IsReference(symbol))
                        {
                            result += L" &";
                        }
                        else
                        {
                            result += L" *";
                        }

                        break;
                    }
                case SymTagArrayType:
                    {
                        result = GetArrayName(symbol);
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
                        auto kind = MsDia::GetUdtKind(symbol);

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

                        result += MsDia::GetName(symbol);

                        break;
                    }
                case SymTagEnum:
                    {
                        result = L"enum ";
                        result += MsDia::GetName(symbol);
                        break;
                    }
                case SymTagBaseType:
                    {
                        result = MsDia::GetBaseTypeName(symbol);
                        break;
                    }
                }
            }

            return result;
        }

        static std::optional<bool> IsValidMember(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            auto symtag = MsDia::GetSymTag(symbol);

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

            auto is_static = MsDia::IsStatic(symbol);

            if (!is_static.has_value())
            {
                return std::nullopt;
            }

            if (is_static.value())
            {
                return false;
            }

            auto location = MsDia::GetLocation(symbol);

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

        static std::unique_ptr<TypeMemberInherited> CreateMemberInherited(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            //
            // Get basic type info.
            //

            auto const type_name = MsDia::GetName(symbol);
            auto const offset = MsDia::GetOffset(symbol);
            auto const size = MsDia::GetLength(symbol);


            //
            // Enumerate native children symbols.
            //

            std::vector<Microsoft::WRL::ComPtr<IDiaSymbol>> native_symbols{};

            if (auto children = MsDia::FindChildren(symbol, SymTagNull); children != nullptr)
            {
                for (auto child = MsDia::Next(children); child != nullptr; child = MsDia::Next(children))
                {
                    native_symbols.push_back(child);
                }
            }


            //
            // Create wrapper for applicable symbols.
            //

            std::vector<std::unique_ptr<BaseTypeMember>> symbols{};

            for (auto const& current : native_symbols)
            {
                if (auto item = Create(current); item != nullptr)
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

                highest_ending = std::max(highest_ending, current->GetNextOffset());

                //
                // Get symbols before current one.
                //

                std::vector<LibPdb::BaseTypeMember*> lesser{};

                for (auto const& before : symbols)
                {
                    if (before->GetNextOffset() <= current->GetOffset())
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
                    [&](const LibPdb::BaseTypeMember* lhs, const LibPdb::BaseTypeMember* rhs)
                    {
                        return (current->GetOffset() - lhs->GetNextOffset()) < (current->GetOffset() - rhs->GetNextOffset());
                    }
                );

                if (it != std::end(lesser))
                {
                    //
                    // Compute offset and padding between this symbol and adjacent.
                    //

                    auto const final_offset = (*it)->GetNextOffset();
                    auto const final_padding = current->GetOffset() - final_offset;

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
                symbols.push_back(std::make_unique<TypeMemberPadding>(padding.first, padding.second));
            }


            //
            // Sort symbols by offset, then size.
            //

            std::sort(
                symbols.begin(),
                symbols.end(),
                [](const std::unique_ptr<BaseTypeMember>& lhs, const std::unique_ptr<BaseTypeMember>& rhs) noexcept
                {
                    if (lhs->GetOffset() == rhs->GetOffset())
                    {
                        return lhs->GetSize() > rhs->GetSize();
                    }

                    return lhs->GetOffset() < rhs->GetOffset();
                }
            );


            //
            // Check if whole type has leading padding value.
            //

            auto const type_size = MsDia::GetLength(symbol);
            if (auto const final_padding = type_size - highest_ending; final_padding != 0)
            {
                if (!symbols.empty())
                {
                    //
                    // If type is not empty, this is real padding. Empty types must have at least
                    // one byte, but we don't want to report it as memory-wasting padding.
                    //

                    symbols.push_back(std::make_unique<TypeMemberPadding>(final_padding, highest_ending));
                }
            }


            //
            //  Because DIA SDK reports sizeof 0 for some type members, we need to mark them as
            //  missing detailed size information.
            //
            //  This requires us to find sequence:
            //
            //      <symbol, offset:x, size:0>
            //      <padding, offset:x, size:not 0>
            //
            //  and replace them with marker.
            //

            if (symbols.size() > 1)
            {
                //
                // We need at least one element in this set of symbols.
                //

                for (size_t i = 0, count = symbols.size() - 1; i < count; ++i)
                {
                    auto const& first = symbols[i];
                    if (first->GetMemberType() == MemberType::Field)
                    {
                        auto const& field = static_cast<TypeMemberField&>(*first);

                        auto& second = symbols[i + 1];

                        if (second->GetMemberType() == MemberType::Padding)
                        {
                            auto& padding = static_cast<LibPdb::TypeMemberPadding&>(*second);

                            if (field.GetOffset() == padding.GetOffset() &&
                                field.GetSize() == 0 &&
                                field.GetKind() != LibPdb::MemberKind::Array)
                            {
                                //
                                // Type may contain array of zero elements!
                                //

                                padding.SetSpurious(true);
                            }
                        }
                    }
                }
            }


            //
            // Compute total padding.
            //

            uint64_t total_padding{};

            for (auto const& it : symbols)
            {
                switch (it->GetMemberType())
                {
                case LibPdb::MemberType::Padding:
                    {
                        auto const& padding = static_cast<LibPdb::TypeMemberPadding const&>(*it);
                        total_padding += padding.GetSize();
                        break;
                    }
                case LibPdb::MemberType::Inherited:
                    {
                        auto const& inherited= static_cast<LibPdb::TypeMemberInherited const&>(*it);
                        total_padding += inherited.GetPadding();
                        break;
                    }
                default:
                    {
                        break;
                    }
                }
            }

            return std::make_unique<TypeMemberInherited>(
                MsDia::GetLength(symbol),
                MsDia::GetOffset(symbol),
                total_padding,
                type_name,
                std::move(symbols)
                );
        }

        static MemberKind GetFieldKind(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            auto const symtag = MsDia::GetSymTag(symbol);

            switch (symtag.value_or(SymTagNull))
            {
            case SymTagNull:
            case SymTagExe:
            case SymTagCompiland:
            case SymTagCompilandDetails:
            case SymTagCompilandEnv:
            case SymTagFunction:
            case SymTagBlock:
            case SymTagData:
            case SymTagAnnotation:
            case SymTagLabel:
            case SymTagPublicSymbol:
            case SymTagFunctionType:
            case SymTagBaseType:
            case SymTagTypedef:
            case SymTagBaseClass:
            case SymTagFriend:
            case SymTagFunctionArgType:
            case SymTagFuncDebugStart:
            case SymTagFuncDebugEnd:
            case SymTagUsingNamespace:
            case SymTagCustom:
            case SymTagThunk:
            case SymTagCustomType:
            case SymTagManagedType:
            case SymTagDimension:
            case SymTagCallSite:
            case SymTagInlineSite:
            case SymTagBaseInterface:
            case SymTagVectorType:
            case SymTagMatrixType:
            case SymTagHLSLType:
            case SymTagCaller:
            case SymTagCallee:
            case SymTagExport:
            case SymTagHeapAllocationSite:
            case SymTagCoffGroup:
            case SymTagInlinee:
            case SymTagMax:
                return MemberKind::Unknown;
            case SymTagUDT:
            case SymTagEnum:
                return MemberKind::Value;
            case SymTagPointerType:
                if (MsDia::IsReference(symbol))
                {
                    return MemberKind::Reference;
                }
                else
                {
                    if (auto const type = MsDia::GetType(symbol); type != nullptr)
                    {
                        if (auto const type_symtag = MsDia::GetSymTag(type); type_symtag.has_value())
                        {
                            switch (*type_symtag)
                            {
                            case SymTagVTable:
                            case SymTagVTableShape:
                                {
                                    return MemberKind::VTable;
                                }
                            default:
                                {
                                    break;
                                }
                            }
                        }
                    }

                    return MemberKind::Pointer;
                }
            case SymTagArrayType:
                return MemberKind::Array;
            case SymTagVTableShape:
            case SymTagVTable:
                return MemberKind::VTable;
            }

            return MemberKind::Unknown;
        }

        static std::unique_ptr<TypeMemberField> CreateMemberField(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            auto type = MsDia::GetType(symbol);

            auto size = MsDia::GetLength(type);
            auto const offset = static_cast<uint64_t>(MsDia::GetOffset(symbol));
            auto const name = MsDia::GetName(symbol);
            auto const type_name = GetTypeName(type);
            auto const location = MsDia::GetLocation(symbol);
            auto const kind = GetFieldKind(type);

            std::optional<std::pair<uint64_t, uint64_t>> bits{};

            if (location.has_value() && location.value() == LocIsBitField)
            {
                bits = {
                    MsDia::GetBitPosition(symbol),
                    MsDia::GetLength(symbol),
                };
            }
            
            return std::make_unique<TypeMemberField>(
                size,
                offset,
                name,
                type_name,
                kind,
                bits
            );
        }

        static std::unique_ptr<BaseTypeMember> Create(
            Microsoft::WRL::ComPtr<IDiaSymbol> symbol
        ) noexcept
        {
            auto symtag = MsDia::GetSymTag(symbol);

            if (!symtag.has_value())
            {
                return nullptr;
            }

            if (symtag == SymTagBaseClass)
            {
                return CreateMemberInherited(symbol);
            }
            else if (auto valid = IsValidMember(symbol); valid.value_or(false))
            {
                return CreateMemberField(symbol);
            }

            return nullptr;
        }

        virtual std::unique_ptr<TypeDescriptor> GetDescriptor(
            const Type& type
        ) const noexcept override
        {
            auto const& typed = static_cast<LibPdb::Detail::TypeImpl const&>(type);

            std::vector<std::unique_ptr<BaseTypeMember>> members{};

            members.emplace_back(
                CreateMemberInherited(typed.GetSymbol())
            );

            return std::make_unique<TypeDescriptor>(std::move(members));
        }
    };
}

namespace LibPdb
{
    PTV_API std::unique_ptr<Session> CreateSession() noexcept
    {
        return std::make_unique<Detail::SessionImpl>();
    }
}
