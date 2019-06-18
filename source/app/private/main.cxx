#include <QApplication>
#include <QWindow>
#include <QAbstractItemModel>
#include <QTreeView>
#include <ptv.hxx>

#include <main.hxx>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Graphyte");
    QCoreApplication::setApplicationName("Pdb Type Viewer");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);



    auto session = ptv::create();
    session->load(LR"(D:\github\engine-concept\build\release-windows-x64\bin\Graphyte-Base.pdb)");

    auto descriptor = session->get_descriptor(
        session->get_types()[5897]
    );

    viewer::TreeModel model{descriptor};

    QTreeView view{};
    view.setModel(&model);
    view.setWindowTitle("model");
    view.show();

    return app.exec();
}
