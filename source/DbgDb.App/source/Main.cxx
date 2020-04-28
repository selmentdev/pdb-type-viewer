#include <QApplication>
#include <QWindow>
#include <QAbstractItemModel>
#include <QMessageBox>
#include <QTreeView>

#include <Forms/FormMain.hxx>

#include <DbgDb.Lib.SourceProvider.hxx>
#include <DbgDb.Lib.Session.hxx>
#include <QLibrary>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(app);

    QCoreApplication::setOrganizationName("Graphyte");
    QCoreApplication::setApplicationName("Pdb Type Viewer");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    std::vector<std::unique_ptr<QLibrary>> plugins{};

    plugins.push_back(std::make_unique<QLibrary>("DbgDb.Lib.Elf"));
    plugins.push_back(std::make_unique<QLibrary>("DbgDb.Lib.Pdb"));

    for (auto const& plugin : plugins)
    {
        using PluginFunction = void(void);
        PluginFunction* func = reinterpret_cast<PluginFunction*>(plugin->resolve("InitializePlugin"));
        func();
    }

    DbgDb::App::Forms::FormMain form_main{};

    form_main.show();

    return app.exec();
}
