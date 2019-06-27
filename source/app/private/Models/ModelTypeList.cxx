#include <Models/ModelTypeList.hxx>
#include <LibPtv/TypeMemberInherited.hxx>

namespace ptvapp::models
{
    TypeListModel::TypeListModel(
        QObject* parent
    ) noexcept
        : QAbstractItemModel{ parent }
        , m_Types{}
        , m_IsAnalyzed{ false }
    {
    }

    TypeListModel::~TypeListModel() noexcept
    {
        qDeleteAll(this->m_Types);
    }

    void TypeListModel::Setup(
        const std::vector<std::unique_ptr<LibPdb::Type>>& types
    ) noexcept
    {
        this->beginResetModel();

        m_Types.clear();

        for (auto const& type : types)
        {
            m_Types.push_back(new TypeListElement{ type.get() });
        }

        this->endResetModel();
    }

    void TypeListModel::DoAnalyze(
        LibPdb::Session& session,
        std::function<void(const QString& type, int32_t current, int32_t total)> callback
    ) noexcept
    {
        if (!this->m_IsAnalyzed)
        {
            this->m_IsAnalyzed = true;

            auto const count = this->m_Types.count();
            int current{};

            for (auto& type : this->m_Types)
            {
                callback(
                    type->GetName(),
                    current,
                    count
                );

                ++current;

                if (auto descriptor = session.GetDescriptor(*type->GetType()); descriptor != nullptr)
                {
                    if (auto const& members = descriptor->GetMembers(); !members.empty())
                    {
                        auto const& analyzedType = members.front();
                        if (analyzedType->GetMemberType() == LibPdb::MemberType::Inherited)
                        {
                            auto const& actualType = static_cast<const LibPdb::TypeMemberInherited&>(*analyzedType);

                            type->SetPadding(actualType.GetPadding());
                            type->SetSize(actualType.GetSize());
                        }
                    }
                }
            }
        }
    }

    int TypeListModel::rowCount(
        const QModelIndex& parent
    ) const
    {
        if (parent.column() <= 0)
        {
            if (parent.isValid())
            {
                return 0;
            }

            return this->m_Types.count();
        }

        return 0;
    }

    Qt::ItemFlags TypeListModel::flags(
        const QModelIndex& index
    ) const
    {
        if (index.isValid())
        {
            return QAbstractItemModel::flags(index);
        }

        return {};
    }

    QVariant TypeListModel::headerData(
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
                return tr("Size");
            case 2:
                return tr("Padding");
            }
        }

        return {};
    }

    QVariant TypeListModel::data(
        const QModelIndex& index,
        int role
    ) const
    {
        if (index.isValid())
        {
            if (role == Qt::DisplayRole)
            {
                auto const* item = static_cast<TypeListElement*>(index.internalPointer());

                if (item != nullptr)
                {
                    switch (index.column())
                    {
                    case 0:
                        {
                            return item->GetName();
                        }
                    case 1:
                        {
                            if (auto const& size = item->GetSize(); size.has_value())
                            {
                                return size.value();
                            }

                            break;
                        }
                    case 2:
                        {
                            if (auto const& padding = item->GetPadding(); padding.has_value())
                            {
                                return padding.value();
                            }

                            break;
                        }
                    default:
                        {
                            break;
                        }
                    }
                }
            }
        }

        return {};
    }
    
    QModelIndex TypeListModel::index(int row, int column, const QModelIndex& parent) const
    {
        (void)parent;

        return this->createIndex(
            row,
            column,
            this->m_Types[row]
        );
    }

    QModelIndex TypeListModel::parent(const QModelIndex& child) const
    {
        (void)child;
        return {};
    }
    
    int TypeListModel::columnCount(const QModelIndex& parent) const
    {
        (void)parent;
        return 3;
    }
}
