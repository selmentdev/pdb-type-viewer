#pragma once
#include <QAbstractItemModel>
#include <ptv/pdb_file.hxx>
#include <ptv/pdb_type.hxx>

namespace ptvapp::models
{
    class type_list_model : public QAbstractItemModel
    {
        Q_OBJECT
    private:
        const ptv::pdb_file* m_file;

    public:
        explicit type_list_model(
            QObject* parent = nullptr
        ) noexcept;

        virtual ~type_list_model() noexcept;

        void set_pdb_file(const ptv::pdb_file* file) noexcept;

    public:
        int rowCount(const QModelIndex& parent = {}) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        virtual QModelIndex index(int row, int column, const QModelIndex& parent = {}) const override;
        virtual QModelIndex parent(const QModelIndex& child) const override;
        virtual int columnCount(const QModelIndex& parent = {}) const override;
    };
}
