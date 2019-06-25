#pragma once
#include <QDialog>

namespace ptvapp::forms
{
    class AboutBox
        : public QDialog
    {
        Q_OBJECT;
    public:
        explicit AboutBox(QWidget* parent) noexcept;
        ~AboutBox() noexcept;

    private:
        void CreateControls() noexcept;
    };
}
