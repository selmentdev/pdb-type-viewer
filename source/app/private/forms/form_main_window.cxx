#include <forms/form_main_window.hxx>
#include <QtWidgets>
#include <models/model_type_descriptor.hxx>
#include <models/model_type_list.hxx>

namespace ptvapp::forms
{
    main_window::main_window() noexcept
    {
        this->setWindowTitle(tr("PDB Type Viewer"));
        this->setUnifiedTitleAndToolBarOnMac(true);

        this->create_actions();
        this->create_controls();
        this->create_status_bar();
    }

    void main_window::about() noexcept
    {
        QMessageBox::about(this, tr("About pdb-type-viewer"), tr("PDB Type Viewer"));
    }

    void main_window::load() noexcept
    {
        if (auto path = QFileDialog::getOpenFileName(this, {}, {}, tr("PDB Files (*.pdb)")); !path.isEmpty())
        {
            this->load_from_path(path);
        }
    }

    void main_window::load_from_path(QStringView path) noexcept
    {
        m_pdb_file = ptv::create();
        if (m_pdb_file->load(path.toString().toStdWString()))
        {
            m_type_list_model->set_pdb_file(m_pdb_file.get());

            this->setWindowTitle(
                tr("PDB Type Viewer") + QString{ " - %1" }.arg(path)
            );
        }
    }

    void main_window::create_actions() noexcept
    {
        auto* menu_file = menuBar()->addMenu(tr("&File"));

        auto* action_menu_file_open = menu_file->addAction(tr("&Open"), this, &main_window::load);
        action_menu_file_open->setShortcuts(QKeySequence::Open);

        auto* action_menu_file_close = menu_file->addAction(tr("&Close"), this, &QWidget::close);
        action_menu_file_close->setShortcuts(QKeySequence::Close);

        auto* menu_help = menuBar()->addMenu(tr("&Help"));

        [[maybe_unused]] auto* action_menu_help_about = menu_help->addAction(tr("&About"), this, &main_window::about);
    }

    void main_window::create_status_bar() noexcept
    {
        statusBar()->showMessage(tr("Ready"));
    }

    void main_window::create_controls() noexcept
    {
        this->m_dock_pane_type_list = new QDockWidget(tr("Type List"), this);
        this->m_type_list_model = new ptvapp::models::type_list_model(this);

        this->m_type_list_model_proxy = new QSortFilterProxyModel(this);
        this->m_type_list_model_proxy->setSourceModel(this->m_type_list_model);

        auto* container = new QWidget();

        this->m_type_list_view = new QListView(container);
        this->m_type_list_view->setModel(this->m_type_list_model_proxy);
        this->m_type_list_view->setEditTriggers(QAbstractItemView::NoEditTriggers);

        auto* filter = new QLineEdit(container);

        auto* vbox = new QVBoxLayout();
        vbox->addWidget(filter, 0);
        vbox->addWidget(this->m_type_list_view, 1);

        container->setLayout(vbox);

        this->m_dock_pane_type_list->setWidget(container);


        this->addDockWidget(Qt::LeftDockWidgetArea, this->m_dock_pane_type_list);

        this->m_type_model = new ptvapp::models::type_descriptor_model(this);
        this->m_type_view = new QTreeView(this);
        this->m_type_view->setModel(this->m_type_model);
        this->m_type_view->setSelectionMode(QTreeView::MultiSelection);
        this->m_type_view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

        this->setCentralWidget(this->m_type_view);

        connect(
            filter,
            &QLineEdit::textChanged,
            [&](const QString& value)
            {
                this->m_type_list_model_proxy->setFilterRegExp(
                    QRegExp(
                        value,
                        Qt::CaseInsensitive,
                        QRegExp::RegExp
                    )
                );
            }
        );

        connect(
            this->m_type_list_view->selectionModel(),
            &QItemSelectionModel::currentChanged,
            [&](const QModelIndex& current, [[maybe_unused]] const QModelIndex& previous)
            {
                (void)previous;

                if (current.isValid())
                {
                    if (auto const underlying = this->m_type_list_model_proxy->mapToSource(current); underlying.isValid())
                    {
                        if (auto const* type = static_cast<const ptv::pdb_type*>(underlying.internalPointer()); type != nullptr)
                        {
                            if (auto descriptor = this->m_pdb_file->get_descriptor(*type); descriptor != nullptr)
                            {
                                this->m_type_model->from_type_descriptor(
                                    std::move(descriptor)
                                );
                            }
                        }
                    }
                }
            }
        );

        connect(
            this->m_type_model,
            &QAbstractItemModel::modelReset,
            [&]()
            {
                this->m_type_view->expandAll();
            }
        );
    }
}