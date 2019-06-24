#pragma once
#include <QMainWindow>

#include <LibPtv.hxx>

class QAction;
class QListWidget;
class QMenu;
class QStringListModel;
class QTreeView;
class QDockWidget;
class QListView;
class QSortFilterProxyModel;

namespace ptvapp::models
{
    class TypeDescriptorModel;
    class TypeListModel;
}

namespace ptvapp::forms
{
    class MainWindow
        : public QMainWindow
    {
        Q_OBJECT;

    public:
        MainWindow() noexcept;

    public:
        void LoadFromPath(QStringView path) noexcept;

    private slots:
        void ShowAbout() noexcept;
        void LoadPdb() noexcept;

    private:
        void CreateActions() noexcept;
        void CreateStatusBar() noexcept;
        void CreateControls() noexcept;

    private:
        std::unique_ptr<LibPdb::Session> m_PdbFile;

    private:
        ptvapp::models::TypeListModel* m_TypeListModel;
        QSortFilterProxyModel* m_TypeListModelProxy;
        QListView* m_TypeListView;
        QDockWidget* m_DockTypeList;

    private:
        ptvapp::models::TypeDescriptorModel* m_TypeModel;
        QTreeView* m_TypeView;
    };
}
