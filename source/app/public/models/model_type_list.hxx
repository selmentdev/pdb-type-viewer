#pragma once
#include <QAbstractItemModel>
#include <ptv/pdb_file.hxx>
#include <ptv/pdb_type.hxx>

namespace ptvapp::models
{
    class TypeListModel : public QAbstractItemModel
    {
        Q_OBJECT
    private:
        const ptv::pdb_file* m_File;

    public:
        explicit TypeListModel(
            QObject* parent = nullptr
        ) noexcept;

        virtual ~TypeListModel() noexcept;

        void SetPdbFile(const ptv::pdb_file* file) noexcept;

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
