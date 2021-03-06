#include <QApplication>
#include <QWindow>
#include <QAbstractItemModel>
#include <QTreeView>
#include <LibPtv.hxx>

#include <Forms/FormMain.hxx>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(app);

    QCoreApplication::setOrganizationName("Graphyte");
    QCoreApplication::setApplicationName("Pdb Type Viewer");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    ptvapp::forms::MainWindow main_window{};

    if (argc == 2)
    {
        QString path{ argv[1] };
        main_window.LoadFromPath(path);
    }

    main_window.show();

    return app.exec();
}
