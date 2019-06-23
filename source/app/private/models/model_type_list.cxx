#include <models/model_type_list.hxx>

namespace ptvapp::models
{
    type_list_model::type_list_model(
        QObject* parent
    ) noexcept
        : QAbstractItemModel{ parent }
        , m_file{}
    {
    }

    type_list_model::~type_list_model() noexcept
    {
    }

    void type_list_model::set_pdb_file(const ptv::pdb_file* file) noexcept
    {
        this->beginResetModel();
        this->m_file = file;
        this->endResetModel();
    }

    int type_list_model::rowCount(
        const QModelIndex& parent
    ) const
    {
        if (parent.column() <= 0)
        {
            if (this->m_file != nullptr)
            {
                return static_cast<int>(this->m_file->get_types().size());
            }
        }

        return 0;
    }

    QVariant type_list_model::data(
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
    
    QModelIndex type_list_model::index(int row, int column, const QModelIndex& parent) const
    {
        (void)parent;

        if (this->m_file != nullptr)
        {
            auto* type = this->m_file->get_types()[row].get();
            return createIndex(row, column, type);
        }

        return {};
    }

    QModelIndex type_list_model::parent(const QModelIndex& child) const
    {
        (void)child;
        return QModelIndex();
    }
    
    int type_list_model::columnCount(const QModelIndex& parent) const
    {
        (void)parent;
        return 1;
    }
}
