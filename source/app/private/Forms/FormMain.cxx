#include <Forms/FormMain.hxx>
#include <Forms/FormAboutBox.hxx>
#include <Models/ModelTypeDescriptor.hxx>
#include <Models/ModelTypeList.hxx>
#include <Models/ModelValidationList.hxx>
#include <QtWidgets>

namespace ptvapp::forms
{
    MainWindow::MainWindow() noexcept
    {
        this->setWindowTitle(tr("PDB Type Viewer"));
        this->setUnifiedTitleAndToolBarOnMac(true);

        this->CreateControls();
        this->CreateStatusBar();
        this->CreateActions();
    }

    void MainWindow::ShowAbout() noexcept
    {
        auto* about = new forms::AboutBox{ this };
        about->setModal(true);
        about->show();
    }

    void MainWindow::LoadPdb() noexcept
    {
        if (auto path = QFileDialog::getOpenFileName(this, {}, {}, tr("PDB Files (*.pdb)")); !path.isEmpty())
        {
            this->LoadFromPath(path);
        }
    }

    void MainWindow::LoadFromPath(QStringView path) noexcept
    {
        QProgressDialog dialog{ this };
        dialog.setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
        dialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        dialog.setWindowFlag(Qt::WindowCloseButtonHint, false);
        dialog.setCancelButton(nullptr);
        dialog.setAutoClose(true);
        dialog.setModal(true);

        auto label = new QLabel(&dialog);
        label->setMaximumWidth(560);
        label->setMinimumWidth(560);
        label->setWordWrap(true);

        dialog.setLabel(label);

        dialog.show();

        qApp->processEvents();

        auto pdbfile = LibPdb::CreateSession();

        if (pdbfile->Load(path.toString().toStdWString(), [&](int32_t current, int32_t total, std::wstring_view name)
        {
            dialog.setMaximum(total);
            dialog.setValue(current);

            auto text = QString::fromStdWString(
                std::wstring{
                    name
                }
            );

            QFontMetrics metrics(label->font());
            auto elidedText = metrics.elidedText(text, Qt::ElideRight, label->width());
            
            dialog.setLabelText(elidedText);

            qApp->processEvents();
        }))
        {
            m_PdbFile = std::move(pdbfile);
            m_TypeListModel->Setup(m_PdbFile->GetTypes());

            this->setWindowTitle(
                tr("PDB Type Viewer") + QString{ " - %1" }.arg(path)
            );
        }
        else
        {
            dialog.close();

            QMessageBox::warning(
                this,
                "Loading PDB failure",
                QString{ "Cannot load %1 file" }.arg(path)
            );
        }
    }

    void MainWindow::CreateActions() noexcept
    {
        if (auto* menuFile = menuBar()->addMenu(tr("&File")))
        {
            auto* actionMenuFileOpen = menuFile->addAction(tr("&Open"), this, &MainWindow::LoadPdb);
            actionMenuFileOpen->setShortcuts(QKeySequence::Open);

            auto* actionMenuFileClose = menuFile->addAction(tr("&Close"), this, &QWidget::close);
            actionMenuFileClose->setShortcuts(QKeySequence::Close);
        }

        if (auto* menuView = menuBar()->addMenu(tr("&View")))
        {
            [[maybe_unused]]
            auto* actionMenuViewTypeList = menuView->addAction(tr("&Type List"), this->m_DockTypeList, &QDockWidget::show);

            [[maybe_unused]]
            auto* actionMenuViewIssues = menuView->addAction(tr("&Issues"), this->m_DockValidationIssues, &QDockWidget::show);
        }

        if (auto* menuHelp = menuBar()->addMenu(tr("&Help")))
        {
            [[maybe_unused]] auto* actionMenuHelpAbout = menuHelp->addAction(tr("&About"), this, &MainWindow::ShowAbout);
        }
    }

    void MainWindow::CreateStatusBar() noexcept
    {
        statusBar()->showMessage(tr("Ready"));
    }

    void MainWindow::CreateControls() noexcept
    {
        this->m_DockValidationIssues = new QDockWidget(tr("Issues"), this);
        {
            //auto* container = new QWidget();
            this->m_ValidationIssuesView = new QTreeView(this);
            this->m_ValidationIssuesModel = new models::ValidationModel(this);
            this->m_ValidationIssuesView->setModel(this->m_ValidationIssuesModel);
            this->m_ValidationIssuesView->header()->setStretchLastSection(true);
            this->m_DockValidationIssues->setWidget(this->m_ValidationIssuesView);

            this->addDockWidget(Qt::BottomDockWidgetArea, this->m_DockValidationIssues);

            this->m_ValidationIssuesModel->Add("Ready");
        }

        this->m_DockTypeList = new QDockWidget(tr("Type List"), this);
        this->m_TypeListModel = new ptvapp::models::TypeListModel(this);

        this->m_TypeListModelProxy = new QSortFilterProxyModel(this);
        this->m_TypeListModelProxy->setSourceModel(this->m_TypeListModel);

        auto* container = new QWidget();

        this->m_TypeListView = new QTreeView(container);
        this->m_TypeListView->setModel(this->m_TypeListModelProxy);
        this->m_TypeListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        auto* filter = new QLineEdit(container);

        auto* vbox = new QVBoxLayout();
        vbox->addWidget(filter, 0);
        vbox->addWidget(this->m_TypeListView, 1);
        vbox->setContentsMargins({});

        container->setLayout(vbox);

        this->m_DockTypeList->setWidget(container);

        this->addDockWidget(Qt::LeftDockWidgetArea, this->m_DockTypeList);

        this->m_TypeModel = new ptvapp::models::TypeDescriptorModel(this);
        this->m_TypeView = new QTreeView(this);
        this->m_TypeView->setModel(this->m_TypeModel);
        this->m_TypeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

        this->setCentralWidget(this->m_TypeView);

        connect(
            filter,
            &QLineEdit::textChanged,
            [&](const QString& value)
            {
                this->m_TypeListModelProxy->setFilterRegExp(
                    QRegExp(
                        value,
                        Qt::CaseInsensitive,
                        QRegExp::RegExp
                    )
                );
            }
        );

        connect(
            this->m_TypeListView->selectionModel(),
            &QItemSelectionModel::currentChanged,
            [&](const QModelIndex& current, [[maybe_unused]] const QModelIndex& previous)
            {
                (void)previous;

                if (current.isValid())
                {
                    if (auto const underlying = this->m_TypeListModelProxy->mapToSource(current); underlying.isValid())
                    {
                        if (auto const* type = static_cast<const LibPdb::Type*>(underlying.internalPointer()); type != nullptr)
                        {
                            if (auto descriptor = this->m_PdbFile->GetDescriptor(*type); descriptor != nullptr)
                            {
                                this->m_TypeModel->FromDescriptor(
                                    std::move(descriptor)
                                );
                            }
                        }
                    }
                }
            }
        );

        connect(
            this->m_TypeModel,
            &QAbstractItemModel::modelReset,
            [&]()
            {
                this->m_TypeView->expandAll();
            }
        );
    }
}