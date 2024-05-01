

#include "window.h"

#include <QApplication>
#include <QSurfaceFormat>

// based on Qt OpenGL Examples
// https://doc.qt.io/qt-6/examples-widgets-opengl.html

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    Window window;
    window.show();
    return app.exec();
}
