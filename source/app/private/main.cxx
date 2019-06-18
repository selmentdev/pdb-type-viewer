#include <QApplication>
#include <QWindow>
#include <QAbstractItemModel>
#include <QTreeView>
#include <ptv.hxx>


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
        explicit TreeModel(const QString& data, QObject* parent = nullptr)
            : QAbstractItemModel(parent)
        {
            QList<QVariant> rootData;
            rootData << "Title" << "Summary";
            rootItem = new TreeItem(rootData);
            setupModelData(data.split(QString("\n")), rootItem);
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
        void setupModelData(const QStringList& lines, TreeItem* parent);

        TreeItem* rootItem;
    };
}






int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Graphyte");
    QCoreApplication::setApplicationName("Pdb Type Viewer");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);


    viewer::TreeModel model{""};

    QTreeView view{};
    view.setModel(&model);
    view.setWindowTitle("model");
    view.show();

    auto session = ptv::create();
    session->load(LR"(D:\github\engine-concept\build\release-windows-x64\bin\Graphyte-Base.pdb)");

    auto descriptor = session->get_descriptor(
        session->get_types().front()
    );

    (void)descriptor;

    return app.exec();
}
