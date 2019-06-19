#pragma once
#include <QMainWindow>

class QAction;
class QListWidget;
class QMenu;

namespace ptvapp::forms
{
    class main_window : public QMainWindow
    {
        Q_OBJECT;

    public:
        main_window() noexcept;

    private slots:
        void about() noexcept;

    private:
        void create_actions() noexcept;
        void create_status_bar() noexcept;
        void create_controls() noexcept;

    private:
        QMenu* m_main_menu;
    };
}
