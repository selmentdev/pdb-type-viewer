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
            return this->m_Types.count();
        }

        return 0;
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
                    return QString::fromStdWString(std::wstring{ item->GetName() });
                }
            }
        }

        return {};
    }
    
    QModelIndex TypeListModel::index(int row, int column, const QModelIndex& parent) const
    {
        if (this->hasIndex(row, column, parent))
        {
            return this->createIndex(
                row,
                column,
                const_cast<LibPdb::Type*>(this->m_Types[row].GetType())
            );
        }

        return {};
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
