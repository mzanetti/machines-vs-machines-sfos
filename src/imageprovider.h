#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider
{
public:
    explicit ImageProvider(const QString &type);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

private:
    QString m_type;
};

#endif // IMAGEPROVIDER_H
