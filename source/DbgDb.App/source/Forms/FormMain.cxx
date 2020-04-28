#include <Forms/FormMain.hxx>

namespace DbgDb::App::Forms
{
    FormMain::FormMain() noexcept
    {
        this->setWindowTitle(tr("PDB Type Viewer"));
        this->setUnifiedTitleAndToolBarOnMac(true);
    }

    void FormMain::ShowAbout() noexcept
    {
    }
}
