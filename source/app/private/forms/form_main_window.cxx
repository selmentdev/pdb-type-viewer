#include <forms/form_main_window.hxx>
#include <QtWidgets>

namespace ptvapp::forms
{
    main_window::main_window() noexcept
    {
        this->setWindowTitle("pdb type viewer");
        this->setUnifiedTitleAndToolBarOnMac(true);

        this->create_actions();
        this->create_controls();
        this->create_status_bar();
    }

    void main_window::about() noexcept
    {
        QMessageBox::about(this, tr("About pdb-type-viewer"), tr("pdb-type-viewer"));
    }

    void main_window::create_actions() noexcept
    {
        QMenu* menu_file = menuBar()->addMenu(tr("&File"));

        QAction* action_menu_file_close = menu_file->addAction(tr("&Close"), this, &QWidget::close);
        action_menu_file_close->setShortcuts(QKeySequence::Close);

        QMenu* menu_help = menuBar()->addMenu(tr("&Help"));

        [[maybe_unused]] QAction* action_menu_help_about = menu_help->addAction(tr("&About"), this, &main_window::about);
    }

    void main_window::create_status_bar() noexcept
    {
        statusBar()->showMessage(tr("Ready"));
    }

    void main_window::create_controls() noexcept
    {
        QDockWidget* pane_view_type_list = new QDockWidget(tr("Type List"), this);
        this->addDockWidget(Qt::LeftDockWidgetArea, pane_view_type_list);

        this->setCentralWidget(new QTextEdit());
    }
}