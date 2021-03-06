#include <Models/ModelTypeDescriptor.hxx>
#include <LibPtv/TypeMemberInherited.hxx>
#include <LibPtv/TypeMemberField.hxx>
#include <LibPtv/TypeMemberPadding.hxx>
#include <QColor>
#include <QTranslator>

namespace ptvapp::models
{
    TypeDescriptorElement::TypeDescriptorElement(
        LibPdb::BaseTypeMember* member,
        TypeDescriptorElement* parent
    ) noexcept
        : m_Children{}
        , m_Parent{ parent }
        , m_Member{ member }
    {
    }

    TypeDescriptorElement::~TypeDescriptorElement() noexcept
    {
        qDeleteAll(this->m_Children);
    }

    void TypeDescriptorElement::AppendChild(
        TypeDescriptorElement* child
    ) noexcept
    {
        this->m_Children.append(child);
    }

    TypeDescriptorElement* TypeDescriptorElement::GetChild(
        int index
    ) const noexcept
    {
        return this->m_Children.value(index);
    }

    int TypeDescriptorElement::GetChildrenCount() const noexcept
    {
        return this->m_Children.size();
    }

    QVariant TypeDescriptorElement::data(
        TypeDescriptorElementRole role
    ) const noexcept
    {
        if (this->m_Member != nullptr)
        {
            auto const member_type = this->m_Member->GetMemberType();

            switch (role)
            {
            case TypeDescriptorElementRole::Type:
                {
                    if (member_type == LibPdb::MemberType::Field)
                    {
                        auto const* field = static_cast<const LibPdb::TypeMemberField*>(this->m_Member);

                        return QString::fromStdWString(
                            std::wstring{
                                field->GetTypeName()
                            }
                        );
                    }
                    else if (member_type == LibPdb::MemberType::Inherited)
                    {
                        auto const* inherited = static_cast<const LibPdb::TypeMemberInherited*>(this->m_Member);

                        return QString::fromStdWString(
                            std::wstring{
                                inherited->GetName()
                            }
                        );
                    }

                    break;
                }

            case TypeDescriptorElementRole::Name:
                {
                    if (member_type == LibPdb::MemberType::Field)
                    {
                        auto const* field = static_cast<const LibPdb::TypeMemberField*>(this->m_Member);

                        return QString::fromStdWString(
                            std::wstring{
                                field->GetName()
                            }
                        );
                    }
                    else if (member_type == LibPdb::MemberType::Padding)
                    {
                        auto const* padding = static_cast<const LibPdb::TypeMemberPadding*>(this->m_Member);

                        if (padding->IsSpurious())
                        {
                            return "<<spurious-padding>>";
                        }
                        else
                        {
                            return "<<padding>>";
                        }
                    }

                    break;
                }

            case TypeDescriptorElementRole::Bits:
                {
                    if (member_type == LibPdb::MemberType::Field)
                    {
                        auto const* field = static_cast<const LibPdb::TypeMemberField*>(this->m_Member);

                        if (auto const bits = field->GetBits(); bits.has_value())
                        {
                            auto const value = bits.value();

                            return QString{ "<%1:%2>" }
                                .arg(value.first)
                                .arg(value.second);
                        }
                    }

                    break;
                }

            case TypeDescriptorElementRole::Size:
                {
                    return this->m_Member->GetSize();
                }

            case TypeDescriptorElementRole::Offset:
                {
                    return this->m_Member->GetOffset();
                }

            case TypeDescriptorElementRole::IsPadding:
                {
                    return member_type == LibPdb::MemberType::Padding;
                }

            case TypeDescriptorElementRole::IsSpurious:
                {
                    if (member_type == LibPdb::MemberType::Padding)
                    {
                        auto const* padding = static_cast<const LibPdb::TypeMemberPadding*>(this->m_Member);
                        return padding->IsSpurious();
                    }

                    break;
                }
            case TypeDescriptorElementRole::Padding:
                {
                    if (member_type == LibPdb::MemberType::Inherited)
                    {
                        auto const* padding = static_cast<const LibPdb::TypeMemberInherited*>(this->m_Member);
                        return padding->GetPadding();
                    }

                    break;
                }
            case TypeDescriptorElementRole::Kind:
                {
                    if (member_type == LibPdb::MemberType::Field)
                    {
                        auto const& field = static_cast<const LibPdb::TypeMemberField&>(*this->m_Member);

                        switch (field.GetKind())
                        {
                        case LibPdb::MemberKind::VTable:
                            {
                                return QObject::tr("VTable Pointer");
                            }
                        default:
                            {
                                break;
                            }
                        }

                        return QObject::tr("Field");
                    }
                    else if (member_type == LibPdb::MemberType::Inherited)
                    {
                        return QObject::tr("Base Type");
                    }
                    else if (member_type == LibPdb::MemberType::Padding)
                    {
                        auto const* padding = static_cast<const LibPdb::TypeMemberPadding*>(this->m_Member);

                        if (padding->IsSpurious())
                        {
                            return QObject::tr("Spurious Padding");
                        }
                        else
                        {
                            return QObject::tr("Padding");
                        }
                    }

                    break;
                }
            case TypeDescriptorElementRole::CacheLine:
                {
                    static constexpr uint64_t CacheLineSize = 64; // TODO: Make this configurable from UI.

                    if (this->m_Member->GetSize() != 0)
                    {
                        auto const firstOffset = this->m_Member->GetOffset();
                        auto const nextOffset = this->m_Member->GetNextOffset();

                        auto const lower_cacheline = (firstOffset + CacheLineSize) / CacheLineSize;
                        auto const upper_cacheline = (nextOffset + CacheLineSize - 1) / CacheLineSize;

                        if (lower_cacheline != upper_cacheline)
                        {
                            return QString{ "%1 - %2" }.arg(lower_cacheline).arg(upper_cacheline);
                        }
                        else
                        {
                            return QString{ "%1" }.arg(lower_cacheline);
                        }
                    }

                    break;
                }
            }
        }

        return {};
    }

    int TypeDescriptorElement::GetRowIndex() const noexcept
    {
        if (this->m_Parent != nullptr)
        {
            return this->m_Parent->m_Children.indexOf(
                const_cast<TypeDescriptorElement*>(this)
            );
        }

        return 0;
    }

    TypeDescriptorElement* TypeDescriptorElement::GetParent() const noexcept
    {
        return this->m_Parent;
    }

    void TypeDescriptorElement::Clear() noexcept
    {
        qDeleteAll(this->m_Children);
        this->m_Children.clear();
    }
}

namespace ptvapp::models
{
    TypeDescriptorModel::TypeDescriptorModel(
        QObject* parent
    ) noexcept
        : QAbstractItemModel{ parent }
        , m_Root{ new TypeDescriptorElement(nullptr, nullptr) }
    {
    }

    TypeDescriptorModel::~TypeDescriptorModel() noexcept
    {
        delete m_Root;
    }

    void TypeDescriptorModel::FromDescriptor(
        std::unique_ptr<LibPdb::TypeDescriptor>&& descriptor
    ) noexcept
    {
        this->m_CurrentDescriptor = std::move(descriptor);
        this->beginResetModel();

        delete this->m_Root;
        this->m_Root = nullptr;

        this->m_Root = new TypeDescriptorElement(nullptr, nullptr);
        this->SetupModelData(this->m_CurrentDescriptor->GetMembers(), m_Root);

        this->endResetModel();
    }

    static TypeDescriptorElementRole MapRole(int column) noexcept
    {
        switch (column)
        {
        case 0:
            return TypeDescriptorElementRole::Type;
        case 1:
            return TypeDescriptorElementRole::Name;
        case 2:
            return TypeDescriptorElementRole::Offset;
        case 3:
            return TypeDescriptorElementRole::Size;
        case 4:
            return TypeDescriptorElementRole::Bits;
        case 5:
            return TypeDescriptorElementRole::Padding;
        case 6:
            return TypeDescriptorElementRole::Kind;
        case 7:
            return TypeDescriptorElementRole::CacheLine;
        }

        return TypeDescriptorElementRole::Padding;
    }

    QVariant TypeDescriptorModel::data(
        const QModelIndex& index,
        int role
    ) const
    {
        if (index.isValid())
        {
            auto const* item = static_cast<TypeDescriptorElement*>(index.internalPointer());

            if (role == Qt::BackgroundRole)
            {
                if (auto const* member = item->GetMember(); member != nullptr)
                {
                    auto const member_type = member->GetMemberType();

                    if (member_type == LibPdb::MemberType::Padding)
                    {
                        auto const* padding = static_cast<const LibPdb::TypeMemberPadding*>(member);

                        if (padding->IsSpurious())
                        {
                            return QColor::fromRgb(255, 50, 50);
                        }
                        else
                        {
                            return QColor::fromRgb(224, 128, 128);
                        }
                    }
                }
            }

            if (role != Qt::DisplayRole)
            {
                return {};
            }

            auto const item_role = MapRole(index.column());

            return item->data(
                item_role
            );
        }

        return {};
    }

    Qt::ItemFlags TypeDescriptorModel::flags(
        const QModelIndex& index
    ) const
    {
        if (index.isValid())
        {
            return QAbstractItemModel::flags(index);
        }

        return {};
    }

    QVariant TypeDescriptorModel::headerData(
        int section,
        Qt::Orientation orientation,
        int role
    ) const
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
            switch (section)
            {
            case 0:
                return tr("Type");
            case 1:
                return tr("Name");
            case 2:
                return tr("Offset");
            case 3:
                return tr("Size");
            case 4:
                return tr("Bits");
            case 5:
                return tr("Padding");
            case 6:
                return tr("Kind");
            case 7:
                return tr("Cache Line");
            }
        }

        return {};
    }

    QModelIndex TypeDescriptorModel::index(
        int row,
        int column,
        const QModelIndex& parent
    ) const
    {
        if (this->hasIndex(row, column, parent))
        {
            TypeDescriptorElement* root{};

            if (parent.isValid())
            {
                root = static_cast<TypeDescriptorElement*>(parent.internalPointer());
            }
            else
            {
                root = this->m_Root;
            }

            if (auto* child = root->GetChild(row); child != nullptr)
            {
                return this->createIndex(row, column, child);
            }
        }

        return {};
    }

    QModelIndex TypeDescriptorModel::parent(
        const QModelIndex& index
    ) const
    {
        if (index.isValid())
        {
            auto* child = static_cast<TypeDescriptorElement*>(index.internalPointer());
            auto* parent = child->GetParent();

            if (parent != this->m_Root)
            {
                return this->createIndex(parent->GetRowIndex(), 0, parent);
            }
        }

        return {};
    }

    int TypeDescriptorModel::rowCount(
        const QModelIndex& parent
    ) const
    {
        if (parent.column() <= 0)
        {
            TypeDescriptorElement* root{};

            if (parent.isValid())
            {
                root = static_cast<TypeDescriptorElement*>(parent.internalPointer());
            }
            else
            {
                root = this->m_Root;
            }

            return root->GetChildrenCount();
        }

        return 0;
    }

    int TypeDescriptorModel::columnCount(
        const QModelIndex& parent
    ) const
    {
        (void)parent;
        return 8;
    }

    void TypeDescriptorModel::SetupModelData(
        const std::vector<std::unique_ptr<LibPdb::BaseTypeMember>>& members,
        TypeDescriptorElement* parent
    ) noexcept
    {
        for (auto const& member : members)
        {
            auto item = new TypeDescriptorElement(member.get(), parent);

            auto const member_type = member->GetMemberType();

            if (member_type == LibPdb::MemberType::Inherited)
            {
                auto* detailed = static_cast<LibPdb::TypeMemberInherited*>(member.get());

                SetupModelData(detailed->GetMembers(), item);
            }

            parent->AppendChild(item);
        }
    }
}
