#include <QApplication>
#include <QWindow>

#include <ptv.hxx>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Graphyte");
    QCoreApplication::setApplicationName("Pdb Type Viewer");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QWindow window{};
    window.show();

    auto session = ptv::create();
    session->load(LR"(D:\github\engine-concept\build\release-windows-x64\bin\Graphyte-Base.pdb)");

    auto descriptor = session->get_descriptor(
        session->get_types().front()
    );

    (void)descriptor;

    return app.exec();
}
