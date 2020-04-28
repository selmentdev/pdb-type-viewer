#pragma once
#include <QMainWindow>

class QAction;
class QListWidget;
class QMenu;
class QStringListModel;
class QTreeView;
class QDockWidget;
class QListView;
class QSortFilterProxyModel;
class QTableView;

namespace DbgDb::App::Forms
{
    class FormMain
        : public QMainWindow
    {
        Q_OBJECT;
    public:
        FormMain() noexcept;

    private slots:
        void ShowAbout() noexcept;

    private:
        void CreateComponents() noexcept;
        void CreateStatusBar() noexcept;
        void CreateActions() noexcept;
    };
}
