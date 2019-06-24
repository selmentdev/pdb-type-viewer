#include <models/model_type_list.hxx>

namespace ptvapp::models
{
    TypeListModel::TypeListModel(
        QObject* parent
    ) noexcept
        : QAbstractItemModel{ parent }
        , m_File{}
    {
    }

    TypeListModel::~TypeListModel() noexcept
    {
    }

    void TypeListModel::SetPdbFile(const ptv::pdb_file* file) noexcept
    {
        this->beginResetModel();
        this->m_File = file;
        this->endResetModel();
    }

    int TypeListModel::rowCount(
        const QModelIndex& parent
    ) const
    {
        if (parent.column() <= 0)
        {
            if (this->m_File != nullptr)
            {
                return static_cast<int>(this->m_File->get_types().size());
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

            auto const item = static_cast<const ptv::pdb_type*>(index.internalPointer());

            if (item != nullptr)
            {
                return QString::fromStdWString(std::wstring{ item->get_name() });
            }
        }

        return {};
    }
    
    QModelIndex TypeListModel::index(int row, int column, const QModelIndex& parent) const
    {
        (void)parent;

        if (this->m_File != nullptr)
        {
            auto* type = this->m_File->get_types()[row].get();
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
