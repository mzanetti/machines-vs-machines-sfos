#include "imageprovider.h"

#include <QDebug>
#include <QFileInfo>
#include <QCoreApplication>

ImageProvider::ImageProvider(const QString &type) :
    QQuickImageProvider(QQuickImageProvider::Image),
    m_type(type)
{
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << "image requested:" << id << requestedSize;
    if (id.split('/').count() < 2) {
        return QImage();
    }

    QString levelpack = id.split('/').first();
    QString filebasename = id.split('/').at(1);
    QImage image;
    QString subdir = m_type;
    QString fileName = filebasename;
    if (m_type == "boards") {
        qDebug() << "filebakse" << filebasename;
        subdir = filebasename;
        fileName = "board.png";
        if (filebasename.endsWith("-preview")) {
            subdir.remove(QRegExp("-preview$"));
            fileName += "_preview.png";
        }
    } else if (m_type == "packdata") {
        subdir = "";
    }
    qDebug() << "loading image:" << QCoreApplication::applicationDirPath() + "/data/lpbuild/" + levelpack + '/' + subdir + "/" + fileName;

    image.load(QCoreApplication::applicationDirPath() + "/data/lpbuild/" + levelpack + '/' + subdir + "/" + fileName);
    if (requestedSize.isValid()) {
        image.scaled(requestedSize);
    }
    if (size) {
        size->setWidth(image.width());
        size->setHeight(image.height());
    }
    return image;
}
