#include <Models/ModelTypeList.hxx>

namespace ptvapp::models
{
    TypeListModel::TypeListModel(
        QObject* parent
    ) noexcept
        : QAbstractItemModel{ parent }
        , m_Types{}
    {
    }

    TypeListModel::~TypeListModel() noexcept
    {
    }

    void TypeListModel::Setup(
        const std::vector<std::unique_ptr<LibPdb::Type>>& types
    ) noexcept
    {
        this->beginResetModel();

        m_Types.clear();

        for (auto const& type : types)
        {
            m_Types.push_back(TypeListElement{ type.get() });
        }

        this->endResetModel();
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
                auto const item = static_cast<const LibPdb::Type*>(index.internalPointer());

                if (item != nullptr)
                {
                    switch (index.column())
                    {
                    case 0:
                        {
                            return QString::fromStdWString(std::wstring{ item->GetName() });
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
            const_cast<LibPdb::Type*>(this->m_Types[row].GetType())
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
        return 2;
    }
}
