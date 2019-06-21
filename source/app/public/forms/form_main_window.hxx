#pragma once
#include <QMainWindow>

#include <ptv.hxx>

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
    class type_descriptor_model;
}

namespace ptvapp::forms
{
    class main_window : public QMainWindow
    {
        Q_OBJECT;

    public:
        main_window() noexcept;

    public:
        void load_from_path(QStringView path) noexcept;

    private slots:
        void about() noexcept;
        void load() noexcept;

    private:
        void create_actions() noexcept;
        void create_status_bar() noexcept;
        void create_controls() noexcept;

    private:
        std::unique_ptr<ptv::pdb_file> m_pdb_file;

    private:
        QMenu* m_main_menu;

    private:
        QStringListModel* m_type_list_model;
        QSortFilterProxyModel* m_type_list_model_proxy;
        QListView* m_type_list_view;
        QDockWidget* m_dock_pane_type_list;

    private:
        ptvapp::models::type_descriptor_model* m_type_model;
        QTreeView* m_type_view;
    };
}
