#pragma once
#include <QAbstractItemModel>

namespace ptvapp::models
{
    enum class ValidationIssueKind
    {
        Information,
        Warning,
        Error,
    };

    struct ValidationIssue
    {
        QString Value;
        ValidationIssueKind Kind;
    };
}

namespace ptvapp::models
{
    class ValidationModel : public QAbstractItemModel
    {
        Q_OBJECT
    private:
        QList<ValidationIssue> m_Issues;

    public:
        explicit ValidationModel(
            QObject* parent = nullptr
        ) noexcept;

        virtual ~ValidationModel() noexcept;

    public:
        void Add(const QString& value) noexcept;
        void Add(const QString& value, ValidationIssueKind kind) noexcept;

    public:
        virtual int rowCount(
            const QModelIndex& parent = {}
        ) const override;

        virtual QVariant data(
            const QModelIndex& index,
            int role = Qt::DisplayRole
        ) const override;

        virtual Qt::ItemFlags flags(
            const QModelIndex& index
        ) const override;

        virtual QVariant headerData(
            int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole
        ) const override;

        virtual QModelIndex index(
            int row,
            int column,
            const QModelIndex& parent = {}
        ) const override;

        virtual QModelIndex parent(
            const QModelIndex& child
        ) const override;

        virtual int columnCount(
            const QModelIndex& parent = {}
        ) const override;
    };
}
