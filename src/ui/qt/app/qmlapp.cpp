#include <QtGui/QGuiApplication>
#include <QtCore/QDir>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
int main(int argc, char *argv[])
{

	QGuiApplication app(argc, argv);

	QCommandLineParser parser;
	parser.setApplicationDescription("Executable for running qml applications");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("qmlfile", "Root qml file");
	parser.addOptions({
		QCommandLineOption({ "d", "dummy" }, "Dummy option"),
		QCommandLineOption(
			{ "I", "import-path" },
			"Import path"
			"Path to directory, where qml engine should search for imports",
			"."),
	});
	parser.process(app);

	auto positionalArguments = parser.positionalArguments();
	QQuickView viewer;

	const auto importPaths = parser.values("I");
	for (const QString &p : importPaths)
	{
		QDir dir(p);
		viewer.engine()->addImportPath(dir.absolutePath());
	}
	viewer.engine()->addImportPath(QGuiApplication::applicationDirPath() +
								   "/../lib/");

	if (positionalArguments.empty())
		return 1;

	QFileInfo file(positionalArguments[0]);
	viewer.setSource(file.absoluteFilePath());

	viewer.setTitle("Dream renderer window");
	viewer.setResizeMode(QQuickView::SizeRootObjectToView);
	viewer.setColor(QColor(0xf2, 0xf2, 0xf2));
	viewer.show();

	return app.exec();
}
