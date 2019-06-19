#pragma once
#include <QAbstractItemModel>
#include <QTreeView>
#include <ptv.hxx>
#include <ptv/pdb_member_inherited.hxx>
#include <ptv/pdb_member_padding.hxx>
#include <ptv/pdb_member_ending.hxx>
#include <ptv/pdb_member_field.hxx>

namespace viewer
{
    class TreeItem
    {
    public:
        explicit TreeItem(const QList<QVariant>& data, TreeItem* parent = nullptr) noexcept
        {
            m_ParentItem = parent;
            m_ItemData = data;
        }

        ~TreeItem() noexcept
        {
            qDeleteAll(m_ChildItems);
        }

    public:
        void appendChild(TreeItem* child)
        {
            m_ChildItems.append(child);
        }

        TreeItem* child(int row)
        {
            return m_ChildItems.value(row);
        }

        int childCount() const
        {
            return m_ChildItems.count();
        }

        int columnCount() const
        {
            return m_ItemData.count();
        }

        QVariant data(int column) const
        {
            return m_ItemData.value(column);
        }

        int row() const
        {
            if (m_ParentItem != nullptr)
            {
                return m_ParentItem->m_ChildItems.indexOf(const_cast<TreeItem*>(this));
            }

            return 0;
        }

        TreeItem* parentItem()
        {
            return m_ParentItem;
        }

    private:
        QList<TreeItem*> m_ChildItems;
        QList<QVariant> m_ItemData;
        TreeItem* m_ParentItem;
    };

    class TreeModel : public QAbstractItemModel
    {
        Q_OBJECT

    public:
        explicit TreeModel(const std::unique_ptr<ptv::pdb_type_descriptor>& descriptor, QObject* parent = nullptr)
            : QAbstractItemModel(parent)
        {
            QList<QVariant> rootData;
            rootData << "Name" << "Size" << "Offset";
            rootItem = new TreeItem(rootData);
            setupModelData(descriptor->get_members(), rootItem);
        }

        ~TreeModel()
        {
            delete rootItem;
        }

        QVariant data(
            const QModelIndex& index,
            int role
        ) const override
        {
            if (!index.isValid())
                return QVariant();

            if (role != Qt::DisplayRole)
                return QVariant();

            TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

            return item->data(index.column());
        }

        Qt::ItemFlags flags(const QModelIndex& index) const override
        {
            if (!index.isValid())
                return 0;

            return QAbstractItemModel::flags(index);
        }

        QVariant headerData(
            int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole
        ) const override
        {
            if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
                return rootItem->data(section);

            return QVariant();
        }

        QModelIndex index(
            int row,
            int column,
            const QModelIndex& parent = QModelIndex()
        ) const override
        {
            if (!hasIndex(row, column, parent))
                return QModelIndex();

            TreeItem* parentItem;

            if (!parent.isValid())
                parentItem = rootItem;
            else
                parentItem = static_cast<TreeItem*>(parent.internalPointer());

            TreeItem* childItem = parentItem->child(row);
            if (childItem)
                return createIndex(row, column, childItem);
            else
                return QModelIndex();
        }

        QModelIndex parent(const QModelIndex& index) const override
        {
            if (!index.isValid())
                return QModelIndex();

            TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
            TreeItem* parentItem = childItem->parentItem();

            if (parentItem == rootItem)
                return QModelIndex();

            return createIndex(parentItem->row(), 0, parentItem);
        }

        int rowCount(const QModelIndex& parent = QModelIndex()) const override
        {
            TreeItem* parentItem;
            if (parent.column() > 0)
                return 0;

            if (!parent.isValid())
                parentItem = rootItem;
            else
                parentItem = static_cast<TreeItem*>(parent.internalPointer());

            return parentItem->childCount();
        }

        int columnCount(const QModelIndex& parent = QModelIndex()) const override
        {
            if (parent.isValid())
                return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
            else
                return rootItem->columnCount();
        }

    private:
        void setupModelData(const std::vector<std::unique_ptr<ptv::pdb_abstract_type_member>>& members, TreeItem* parent)
        {
            for (auto const& member : members)
            {
                QList<QVariant> data{};

                auto const type = member->get_member_type();

                switch (type)
                {
                case ptv::pdb_member_type::inherited:
                    {
                        if (auto detailed = static_cast<ptv::pdb_member_inherited*>(member.get()); detailed != nullptr)
                        {
                            auto name = detailed->get_name();

                            data << QString::fromWCharArray(name.data(), static_cast<int>(name.size())) << detailed->get_size() << detailed->get_offset();
                        }

                        break;
                    }
                case ptv::pdb_member_type::field:
                    {
                        if (auto detailed = static_cast<ptv::pdb_member_field*>(member.get()); detailed != nullptr)
                        {
                            auto name = detailed->get_name();

                            data << QString::fromWCharArray(name.data(), static_cast<int>(name.size())) << detailed->get_size() << detailed->get_offset();
                        }

                        break;
                    }
                case ptv::pdb_member_type::padding:
                    {
                        data << "<<padding>>" << member->get_size() << member->get_offset();
                        break;
                    }
                case ptv::pdb_member_type::ending:
                    {
                        data << "<<ending>>" << member->get_size() << member->get_offset();
                        break;
                    }
                }

                auto item = new TreeItem(data, parent);

                if (type == ptv::pdb_member_type::inherited)
                {
                    auto detailed = static_cast<ptv::pdb_member_inherited*>(member.get());
                    setupModelData(
                        detailed->get_members(),
                        item
                    );
                }

                parent->appendChild(item);
            }
        }

        TreeItem* rootItem;
    };
}

