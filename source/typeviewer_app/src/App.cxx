#include <QApplication>
#include <QWindow>
#include <QAbstractItemModel>
#include <QTreeView>
#include <QFileDialog>

#include "typeviewer_engine/Model.hxx"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(App);

    QCoreApplication::setOrganizationName("");
    QCoreApplication::setApplicationName("");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QString path = QFileDialog::getSaveFileName();


    TypeViewer::DiagnosticLog log{};

    {
        if (auto ret = TypeViewer::Model::CreateDatabase(path.toStdString().c_str(), log); ret != nullptr)
        {
            (void)ret->QueryTypes();
        }
        else
        {
            for (auto const& e : log.Enumerate())
            {
                fmt::println("> {}", e);
            }
        }
    }

    return app.exec();
}
