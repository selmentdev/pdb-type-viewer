#pragma once
#include <QMainWindow>

#include <ptv.hxx>

class QAction;
class QListWidget;
class QMenu;
class QStringListModel;

namespace viewer
{
    class TreeModel;
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
        QMenu* m_main_menu;
        std::unique_ptr<ptv::pdb_file> m_pdb_file;

        QStringListModel* m_type_list_model;
        viewer::TreeModel* m_type_view_model;
    };
}
