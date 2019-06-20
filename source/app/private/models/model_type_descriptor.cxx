#include <models/model_type_descriptor.hxx>
#include <ptv/pdb_member_inherited.hxx>
#include <ptv/pdb_member_field.hxx>
#include <ptv/pdb_member_padding.hxx>
#include <QColor>

namespace ptvapp::models
{
    type_descriptor_item::type_descriptor_item(
        ptv::pdb_abstract_type_member* member,
        type_descriptor_item* parent
    ) noexcept
        : m_children{}
        , m_parent{ parent }
        , m_member{ member }
    {
    }

    type_descriptor_item::~type_descriptor_item() noexcept
    {
        qDeleteAll(this->m_children);
    }

    void type_descriptor_item::append_child(
        type_descriptor_item* child
    ) noexcept
    {
        this->m_children.append(child);
    }

    type_descriptor_item* type_descriptor_item::get_child(
        int index
    ) const noexcept
    {
        return this->m_children.value(index);
    }

    int type_descriptor_item::children_count() const noexcept
    {
        return this->m_children.size();
    }

    QVariant type_descriptor_item::data(
        type_descriptor_item_role role
    ) const noexcept
    {
        if (this->m_member != nullptr)
        {
            auto const member_type = this->m_member->get_member_type();

            switch (role)
            {
            case type_descriptor_item_role::type:
                {
                    if (member_type == ptv::pdb_member_type::field)
                    {
                        auto const* field = static_cast<const ptv::pdb_member_field*>(this->m_member);

                        return QString::fromStdWString(
                            std::wstring{
                                field->get_type_name()
                            }
                        );
                    }
                    else if (member_type == ptv::pdb_member_type::inherited)
                    {
                        auto const* inherited = static_cast<const ptv::pdb_member_inherited*>(this->m_member);

                        return QString::fromStdWString(
                            std::wstring{
                                inherited->get_name()
                            }
                        );
                    }

                    break;
                }

            case type_descriptor_item_role::name:
                {
                    if (member_type == ptv::pdb_member_type::field)
                    {
                        auto const* field = static_cast<const ptv::pdb_member_field*>(this->m_member);

                        if (auto const bits = field->get_bits(); bits.has_value())
                        {
                            auto const& value = bits.value();

                            return QString::fromStdWString(
                                std::wstring{
                                    field->get_name()
                                }
                            ) + QString{ " <%1:%2>" }
                                .arg(value.first)
                                .arg(value.second);
                        }
                        else
                        {
                            return QString::fromStdWString(
                                std::wstring{
                                    field->get_name()
                                }
                            );
                        }
                    }
                    else if (member_type == ptv::pdb_member_type::padding)
                    {
                        auto const* padding = static_cast<const ptv::pdb_member_padding*>(this->m_member);

                        if (padding->is_spurious())
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

            case type_descriptor_item_role::size:
                {
                    return this->m_member->get_size();
                }

            case type_descriptor_item_role::offset:
                {
                    return this->m_member->get_offset();
                }

            case type_descriptor_item_role::is_padding:
                {
                    return member_type == ptv::pdb_member_type::padding;
                }

            case type_descriptor_item_role::is_spurious:
                {
                    if (member_type == ptv::pdb_member_type::padding)
                    {
                        auto const* padding = static_cast<const ptv::pdb_member_padding*>(this->m_member);
                        return padding->is_spurious();
                    }
                }
            }
        }

        return {};
    }

    int type_descriptor_item::get_row_index() const noexcept
    {
        if (this->m_parent != nullptr)
        {
            return this->m_parent->m_children.indexOf(
                const_cast<type_descriptor_item*>(this)
            );
        }

        return 0;
    }

    type_descriptor_item* type_descriptor_item::get_parent() const noexcept
    {
        return this->m_parent;
    }

    void type_descriptor_item::clear() noexcept
    {
        qDeleteAll(this->m_children);
        this->m_children.clear();
    }
}

namespace ptvapp::models
{
    type_descriptor_model::type_descriptor_model(
        QObject* parent
    ) noexcept
        : QAbstractItemModel{ parent }
        , m_Root{ new type_descriptor_item(nullptr, nullptr) }
    {
    }

    type_descriptor_model::~type_descriptor_model() noexcept
    {
        delete m_Root;
    }

    void type_descriptor_model::from_type_descriptor(
        std::unique_ptr<ptv::pdb_type_descriptor>&& descriptor
    ) noexcept
    {
        this->m_CurrentDescriptor = std::move(descriptor);
        this->beginResetModel();

        delete this->m_Root;
        this->m_Root = nullptr;

        this->m_Root = new type_descriptor_item(nullptr, nullptr);
        this->SetupModelData(this->m_CurrentDescriptor->get_members(), m_Root);

        this->endResetModel();
    }

    static type_descriptor_item_role MapRole(int column) noexcept
    {
        switch (column)
        {
        case 0:
            return type_descriptor_item_role::type;
        case 1:
            return type_descriptor_item_role::name;
        case 2:
            return type_descriptor_item_role::offset;
        case 3:
            return type_descriptor_item_role::size;
        }

        return type_descriptor_item_role::name;
    }

    QVariant type_descriptor_model::data(
        const QModelIndex& index,
        int role
    ) const
    {
        if (index.isValid())
        {
            auto const* item = static_cast<type_descriptor_item*>(index.internalPointer());

            if (role == Qt::BackgroundRole)
            {
                if (auto const* member = item->get_member(); member != nullptr)
                {
                    auto const member_type = member->get_member_type();

                    if (member_type == ptv::pdb_member_type::padding)
                    {
                        auto const* padding = static_cast<const ptv::pdb_member_padding*>(member);

                        if (padding->is_spurious())
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

    Qt::ItemFlags type_descriptor_model::flags(
        const QModelIndex& index
    ) const
    {
        if (index.isValid())
        {
            return QAbstractItemModel::flags(index);
        }

        return {};
    }

    QVariant type_descriptor_model::headerData(
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
            }
        }

        return {};
    }

    QModelIndex type_descriptor_model::index(
        int row,
        int column,
        const QModelIndex& parent
    ) const
    {
        if (this->hasIndex(row, column, parent))
        {
            type_descriptor_item* root{};

            if (parent.isValid())
            {
                root = static_cast<type_descriptor_item*>(parent.internalPointer());
            }
            else
            {
                root = this->m_Root;
            }

            if (auto* child = root->get_child(row); child != nullptr)
            {
                return this->createIndex(row, column, child);
            }
        }

        return {};
    }

    QModelIndex type_descriptor_model::parent(
        const QModelIndex& index
    ) const
    {
        if (index.isValid())
        {
            auto* child = static_cast<type_descriptor_item*>(index.internalPointer());
            auto* parent = child->get_parent();

            if (parent != this->m_Root)
            {
                return this->createIndex(parent->get_row_index(), 0, parent);
            }
        }

        return {};
    }

    int type_descriptor_model::rowCount(
        const QModelIndex& parent
    ) const
    {
        if (parent.column() <= 0)
        {
            type_descriptor_item* root{};

            if (parent.isValid())
            {
                root = static_cast<type_descriptor_item*>(parent.internalPointer());
            }
            else
            {
                root = this->m_Root;
            }

            return root->children_count();
        }

        return 0;
    }

    int type_descriptor_model::columnCount(
        const QModelIndex& parent
    ) const
    {
        (void)parent;
        return 4;
    }

    void type_descriptor_model::SetupModelData(
        const std::vector<std::unique_ptr<ptv::pdb_abstract_type_member>>& members,
        type_descriptor_item* parent
    ) noexcept
    {
        for (auto const& member : members)
        {
            auto item = new type_descriptor_item(member.get(), parent);

            auto const member_type = member->get_member_type();

            if (member_type == ptv::pdb_member_type::inherited)
            {
                auto* detailed = static_cast<ptv::pdb_member_inherited*>(member.get());

                SetupModelData(detailed->get_members(), item);
            }

            parent->append_child(item);
        }
    }
}
