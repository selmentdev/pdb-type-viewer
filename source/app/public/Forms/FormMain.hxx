#pragma once
#include <QMainWindow>

#include <LibPtv.hxx>

class QAction;
class QListWidget;
class QMenu;
class QStringListModel;
class QTreeView;
class QDockWidget;
class QListView;
class QSortFilterProxyModel;
class QTableView;
class QLineEdit;
class QPushButton;

namespace ptvapp::models
{
    class TypeDescriptorModel;
    class TypeListModel;
    class ValidationModel;
}

namespace ptvapp::forms
{
    class MainWindow
        : public QMainWindow
    {
        Q_OBJECT;

    public:
        MainWindow() noexcept;

    public:
        void LoadFromPath(QStringView path) noexcept;

    private slots:
        void ShowAbout() noexcept;
        void LoadPdb() noexcept;
        void DoAnalyze() noexcept;

    private:
        void CreateActions() noexcept;
        void CreateStatusBar() noexcept;
        void CreateControls() noexcept;

    private:
        std::unique_ptr<LibPdb::Session> m_PdbFile;

    private:
        ptvapp::models::TypeListModel* m_TypeListModel;
        QSortFilterProxyModel* m_TypeListModelProxy;
        QTreeView* m_TypeListView;
        QLineEdit* m_TypeListViewFilter;
        QDockWidget* m_DockTypeList;
        QPushButton* m_TypeListAnalyzeButton;

    private:
        ptvapp::models::TypeDescriptorModel* m_TypeModel;
        QTreeView* m_TypeView;

    private:
        QDockWidget* m_DockValidationIssues;
        QTreeView* m_ValidationIssuesView;
        ptvapp::models::ValidationModel* m_ValidationIssuesModel;
    };
}
