#pragma once
#include <QDialog>

namespace ptvapp::forms
{
    class about_box
        : public QDialog
    {
        Q_OBJECT;
    public:
        explicit about_box(QWidget* parent) noexcept;
        ~about_box() noexcept;

    private:
        void CreateControls() noexcept;
    };
}
