#pragma once
#include <QDialog>

namespace DbgDb::App::Forms
{
    class FormAboutBox
        : public QDialog
    {
        Q_OBJECT;
    public:
        explicit FormAboutBox(QWidget* parent) noexcept;

    private:
        void CreateControls() noexcept;
    };
}
