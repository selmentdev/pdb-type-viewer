#include <Forms/FormMain.hxx>
#include <Forms/FormAboutBox.hxx>
#include <QMdiArea>
#include <QPushButton>
#include <QtWidgets>

namespace DbgDb::App::Forms
{
    FormMain::FormMain() noexcept
    {
        this->setWindowTitle(tr("PDB Type Viewer"));
        this->setUnifiedTitleAndToolBarOnMac(true);
        this->CreateComponents();
        this->CreateStatusBar();
        this->CreateActions();
    }

    void FormMain::ShowAbout() noexcept
    {
        auto* about = new Forms::FormAboutBox(this);
        about->setModal(true);
        about->show();
    }

    void FormMain::CreateComponents() noexcept
    {
        QMdiArea* area = new QMdiArea();
        area->setTabShape(QTabWidget::Triangular);
        area->setViewMode(QMdiArea::TabbedView);
        area->addSubWindow(new QPushButton());
        this->setCentralWidget(area);
    }

    void FormMain::CreateStatusBar() noexcept
    {
        statusBar()->showMessage(tr("Ready"));
    }

    void FormMain::CreateActions() noexcept
    {
        {
            auto* menuHelp = this->menuBar()->addMenu(tr("&Help"));
            menuHelp->addAction(tr("&About"), this, &FormMain::ShowAbout);
        }
    }
}
