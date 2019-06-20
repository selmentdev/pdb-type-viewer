#include <QApplication>
#include <QWindow>
#include <QAbstractItemModel>
#include <QTreeView>
#include <ptv.hxx>

#include <forms/form_main_window.hxx>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    //Q_INIT_RESOURCE(ptvapp);

    QCoreApplication::setOrganizationName("Graphyte");
    QCoreApplication::setApplicationName("Pdb Type Viewer");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    ptvapp::forms::main_window main_window{};
    main_window.show();

    return app.exec();
}
