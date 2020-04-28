#include <QApplication>
#include <QWindow>
#include <QAbstractItemModel>
#include <QMessageBox>
#include <QTreeView>

#include <Forms/FormMain.hxx>

#include <DbgDb.Lib.SourceProvider.hxx>
#include <DbgDb.Lib.Session.hxx>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(app);

    QCoreApplication::setOrganizationName("Graphyte");
    QCoreApplication::setApplicationName("Pdb Type Viewer");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    DbgDb::Lib::CreateProvider();

    DbgDb::App::Forms::FormMain form_main{};

    if (argc == 2)
    {
        QString path{ argv[1] };
        DbgDb::Lib::Result result{};
        auto session = DbgDb::Lib::CreateSession(argv[1], result);

        QMessageBox::information(nullptr, "Create Session", DbgDb::Lib::ToString(result), QMessageBox::Ok);
        //main_window.LoadFromPath(path);
    }

    form_main.show();

    return app.exec();
}
