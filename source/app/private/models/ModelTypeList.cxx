#include <models/model_type_list.hxx>

namespace ptvapp::models
{
    TypeListModel::TypeListModel(
        QObject* parent
    ) noexcept
        : QAbstractItemModel{ parent }
        , m_Session{}
    {
    }

    TypeListModel::~TypeListModel() noexcept
    {
    }

    void TypeListModel::SetSession(const LibPdb::Session* session) noexcept
    {
        this->beginResetModel();
        this->m_Session = session;
        this->endResetModel();
    }

    int TypeListModel::rowCount(
        const QModelIndex& parent
    ) const
    {
        if (parent.column() <= 0)
        {
            if (this->m_Session != nullptr)
            {
                return static_cast<int>(this->m_Session->GetTypes().size());
            }
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
            auto const row = index.row();

            if (role != Qt::DisplayRole)
            {
                return {};
            }

            auto const item = static_cast<const LibPdb::Type*>(index.internalPointer());

            if (item != nullptr)
            {
                return QString::fromStdWString(std::wstring{ item->GetName() });
            }
        }

        return {};
    }
    
    QModelIndex TypeListModel::index(int row, int column, const QModelIndex& parent) const
    {
        (void)parent;

        if (this->m_Session != nullptr)
        {
            auto* type = this->m_Session->GetTypes()[row].get();
            return createIndex(row, column, type);
        }

        return {};
    }

    QModelIndex TypeListModel::parent(const QModelIndex& child) const
    {
        (void)child;
        return QModelIndex();
    }
    
    int TypeListModel::columnCount(const QModelIndex& parent) const
    {
        (void)parent;
        return 1;
    }
}
