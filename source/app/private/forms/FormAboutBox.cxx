#include <Forms/FormAboutBox.hxx>
#include <QtWidgets>

namespace ptvapp::forms
{
    AboutBox::AboutBox(QWidget* parent) noexcept
        :QDialog(parent)
    {
        this->setWindowTitle(tr("About PDB Type Viewer"));

        this->CreateControls();
    }
 
    AboutBox::~AboutBox() noexcept
    {
    }

    void AboutBox::CreateControls() noexcept
    {
        if (auto* layout = new QVBoxLayout())
        {
            //
            // Add image header.
            //

            if (auto* header = new QLabel(this))
            {
                header->setBackgroundRole(QPalette::Base);

                QImage image(":/logo.png");
                header->setPixmap(QPixmap::fromImage(std::move(image)));
                layout->addWidget(header);
            }


            //
            // Add link to github repo.
            //

            if (auto * link = new QLabel(
                QString{
                    R"(<a href="https://github.com/selmentdev/pdb-type-viewer">https://github.com/selmentdev/pdb-type-viewer</a>)"
                }
            ))
            {
                //link->setTextFormat(Qt::RichText);
                link->setTextInteractionFlags(Qt::TextBrowserInteraction);
                link->setOpenExternalLinks(true);

                layout->addWidget(link);
            }


            //
            // Add Qt Version information.
            //

            if (auto* version = new QLabel(QString{ "Qt Version: %1" }.arg(qVersion())))
            {
                layout->addWidget(version);
            }


            //
            // Add license information.
            //

            if (auto* license = new QLabel())
            {
                QFile file{ ":/license.txt" };
                if (file.open(QIODevice::ReadOnly))
                {
                    license->setText(file.readAll());
                }

                layout->addWidget(license);
            }


            //
            // Add pane with OK button.
            //

            if (auto* buttonPane = new QWidget(this))
            {
                auto* okButton = new QPushButton(tr("&OK"), buttonPane);
                connect(okButton, &QPushButton::clicked, this, &AboutBox::close);

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
