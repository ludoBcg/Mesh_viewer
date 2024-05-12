/*********************************************************************************************************************
 *
 * main.h
 *
 * Main function 
 *
 * Mesh_viewer
 * Ludovic Blache
 *
 *********************************************************************************************************************/


#include "window.h"

#include <QApplication>
#include <QSurfaceFormat>

#include <stdio.h>
#include <stdlib.h>


//Installs a Qt message handler
// https://doc.qt.io/qt-6/qtlogging.html#qInstallMessageHandler

QtMessageHandler originalHandler = nullptr;


void logToFile(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString message = qFormatLogMessage(type, context, msg);
    static FILE* f = fopen("Mesh_viewer_log.txt", "w");
    fprintf(f, "%s\n", qPrintable(message));
    fflush(f);

    if (originalHandler)
        originalHandler(type, context, msg);
}


// based on Qt OpenGL Examples
// https://doc.qt.io/qt-6/examples-widgets-opengl.html

int main(int argc, char* argv[])
{
    originalHandler = qInstallMessageHandler(logToFile);


    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    Window window;
    window.show();
    return app.exec();
}
