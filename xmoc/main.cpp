#include <QByteArray>
#include <QProcess>
#include <QDebug>
#include <QFile>

void warnUsage(const char* message) {
    qWarning(message);
    qInfo("Usage: xmoc MOC_PATH MOC_OPTIONS INPUT_FILE -o OUTPUT_FILE");
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        warnUsage("Invalid arguments.");
        return 1;
    }

    QString command = QString::fromLocal8Bit(argv[1]);
    QStringList arguments;
    bool outputFileFollows = false;
    QString outputFile;
    for(int I = 2; I < argc; ++I) {
        QString argument = QString::fromLocal8Bit(argv[I]);
        arguments.append(argument);
        if(outputFileFollows) {
            outputFile = argument;
        }
        if(argument == QLatin1Literal("-o")) {
            outputFileFollows = true;
        }
    }

    if(outputFile.isEmpty()) {
        warnUsage("Invalid arguments. No output file specified.");
        return 1;
    }

    const int status = QProcess::execute(command, arguments);
    if(status != 0) {
        qWarning("Failed to execute moc.");
        return status;
    }

    // filter output file.
    QFile file(outputFile);
    if(! file.open(QIODevice::ReadOnly)) {
        qWarning("Failed to open output file for reading.");
        return 2;
    }

    const QByteArray data = file.readAll();
    if(data.size() != file.size()) {
        qWarning("Failed to read output file.");
        return 3;
    }

    file.close();

    const int begin = data.indexOf(QByteArrayLiteral("_t->xslot"));
    if(begin == -1) {
        return 0;
    }

    const int end = data.indexOf(';', begin);
    if(end == -1) {
        return 0;
    }

    if(! file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning("Failed to open output file for writing.");
        return 4;
    }

    const QByteArray replacement = QByteArrayLiteral("_t->xslot(_a);");

    if(file.write(data.left(begin)) != begin
       || file.write(replacement) != replacement.size()
       || file.write(data.right(data.size() - end - 1)) != data.size() - end - 1) {
        qWarning("Failed to write output file.");
        return 5;
    }

    file.close();

    return 0;
}
