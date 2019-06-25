#include <models/ModelValidationList.hxx>

namespace ptvapp::models
{
    ValidationModel::ValidationModel(
        QObject* parent
    ) noexcept
        : QAbstractItemModel{ parent }
        , m_Issues{}
    {
    }

    ValidationModel::~ValidationModel() noexcept
    {
    }

    void ValidationModel::Add(const QString& value) noexcept
    {
        this->beginInsertRows({}, m_Issues.count(), m_Issues.count());
        this->Add(value, ValidationIssueKind::Information);
        this->endInsertRows();
    }

    void ValidationModel::Add(const QString& value, ValidationIssueKind kind) noexcept
    {
        this->m_Issues.push_back({ value, kind });
    }

    int ValidationModel::rowCount(
        const QModelIndex& parent
    ) const
    {
        if (parent.column() <= 0)
        {
            if (parent.isValid())
            {
                //
                // List elements doesn't have any children!
                //

                return 0;
            }

            return this->m_Issues.count();
        }

        return 0;
    }

    QVariant ValidationModel::data(
        const QModelIndex& index,
        int role
    ) const
    {
        if (index.isValid())
        {
            if (role == Qt::DisplayRole)
            {
                auto const row = index.row();

                auto const& item = this->m_Issues[row];

                switch (index.column())
                {
                case 0:
                    {
                        switch (item.Kind)
                        {
                        case ValidationIssueKind::Error:
                            {
                                return tr("Error");
                            }
                        case ValidationIssueKind::Information:
                            {
                                return tr("Information");
                            }
                        case ValidationIssueKind::Warning:
                            {
                                return tr("Warning");
                            }
                        }
                    }
                case 1:
                    {
                        return item.Value;
                    }
                }

            }
        }

        return {};
    }

    Qt::ItemFlags ValidationModel::flags(
        const QModelIndex& index
    ) const
    {
        if (index.isValid())
        {
            return QAbstractItemModel::flags(index);
        }

        return {};
    }

    QVariant ValidationModel::headerData(
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
                return tr("Kind");
            case 1:
                return tr("Message");
            }
        }

        return {};
    }

    QModelIndex ValidationModel::index(
        int row,
        int column,
        const QModelIndex& parent
    ) const
    {
        (void)parent;

        return createIndex(row, column);
    }

    QModelIndex ValidationModel::parent(
        const QModelIndex& child
    ) const
    {
        (void)child;
        return {};
    }

    int ValidationModel::columnCount(
        const QModelIndex& parent
    ) const
    {
        (void)parent;
        return 2;
    }
}
