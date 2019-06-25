#pragma once
#include <QAbstractItemModel>
#include <LibPtv/Session.hxx>
#include <LibPtv/Type.hxx>

namespace ptvapp::models
{
    class TypeListModel : public QAbstractItemModel
    {
        Q_OBJECT
    private:
        const LibPdb::Session* m_Session;

    public:
        explicit TypeListModel(
            QObject* parent = nullptr
        ) noexcept;

        virtual ~TypeListModel() noexcept;

        void SetSession(const LibPdb::Session* file) noexcept;

    public:
        virtual int rowCount(
            const QModelIndex& parent = {}
        ) const override;

        virtual QVariant data(
            const QModelIndex& index,
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
