#include <forms/form_about_box.hxx>
#include <QtWidgets>

namespace ptvapp::forms
{
    about_box::about_box(QWidget* parent) noexcept
        :QDialog(parent)
    {
        this->setWindowTitle(tr("About PDB Type Viewer"));

        this->CreateControls();
    }
 
    about_box::~about_box() noexcept
    {
    }

    void about_box::CreateControls() noexcept
    {
        if (auto* layout = new QVBoxLayout())
        {
            if (auto* title = new QLabel(this->windowTitle(), this))
            {
                auto font = title->font();
                font.setPointSizeF(18.0F);
                font.setBold(true);
                title->setFont(font);
                layout->addWidget(title, 0);
            }

            if (auto* copyright = new QLabel(tr(u8"Copyright \u00a9 Karol Grzybowski")))
            {
                layout->addWidget(copyright, 0, Qt::AlignRight);
            }

            if (auto* version = new QLabel(QString{ "Qt Version: %1" }.arg(qVersion())))
            {
                layout->addWidget(version);
            }

            if (auto* buttonPane = new QWidget(this))
            {
                auto* okButton = new QPushButton(tr("&OK"), buttonPane);
                connect(okButton, &QPushButton::clicked, this, &about_box::close);

                if (auto* buttonPaneLayout = new QHBoxLayout())
                {
                    buttonPaneLayout->addStretch();
                    buttonPaneLayout->addWidget(okButton);
                    buttonPaneLayout->setSpacing({});
                    buttonPaneLayout->setContentsMargins({});

                    buttonPane->setLayout(buttonPaneLayout);
                }

                layout->addWidget(buttonPane);
            }

            this->setLayout(layout);
        }


        this->setBackgroundRole(QPalette::Window);
        this->setSizeGripEnabled(false);
        this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
        this->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    }
}
