#pragma once
#include <QAbstractItemModel>
#include <ptv/pdb_abstract_type_member.hxx>
#include <ptv/pdb_type_descriptor.hxx>

namespace ptvapp::models
{
    enum class type_descriptor_item_role
    {
        type,
        name,
        size,
        offset,
        kind,
        bits,
        padding,
        is_padding,
        is_spurious,
    };

    class type_descriptor_item
    {
    private:
        QList<type_descriptor_item*> m_children;
        type_descriptor_item* m_parent;
        ptv::pdb_abstract_type_member* m_member;

    public:
        explicit type_descriptor_item(
            ptv::pdb_abstract_type_member* member,
            type_descriptor_item* parent = nullptr
        ) noexcept;

        ~type_descriptor_item() noexcept;

    public:
        void append_child(
            type_descriptor_item* child
        ) noexcept;

        type_descriptor_item* get_child(
            int index
        ) const noexcept;

        int children_count() const noexcept;

        QVariant data(
            type_descriptor_item_role role
        ) const noexcept;

        int get_row_index() const noexcept;

        type_descriptor_item* get_parent() const noexcept;

        void clear() noexcept;

    public:
        ptv::pdb_abstract_type_member* get_member() const noexcept
        {
            return m_member;
        }
    };
}

namespace ptvapp::models
{
    class type_descriptor_model : public QAbstractItemModel
    {
        Q_OBJECT
    private:
        type_descriptor_item* m_Root;
        std::unique_ptr<ptv::pdb_type_descriptor> m_CurrentDescriptor;

    public:
        explicit type_descriptor_model(
            QObject* parent = nullptr
        ) noexcept;

        ~type_descriptor_model() noexcept;

    public:
        void from_type_descriptor(
            std::unique_ptr<ptv::pdb_type_descriptor>&& descriptor
        ) noexcept;

    public: // interface QAbstractItemModel
        QVariant data(
            const QModelIndex& index,
            int role
        ) const override;

        Qt::ItemFlags flags(
            const QModelIndex& index
        ) const override;

        QVariant headerData(
            int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole
        ) const override;

        QModelIndex index(
            int row,
            int column,
            const QModelIndex& parent = {}
        ) const override;

        QModelIndex parent(
            const QModelIndex& index
        ) const override;

        int rowCount(
            const QModelIndex& parent = {}
        ) const override;

        int columnCount(
            const QModelIndex& parent = {}
        ) const override;

    private:
        void SetupModelData(
            const std::vector<std::unique_ptr<ptv::pdb_abstract_type_member>>& members,
            type_descriptor_item* parent
        ) noexcept;
    };
}
