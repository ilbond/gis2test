#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QVariant>

#include "fileworker.h"

class FileManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(
        QVariantList topWords READ topWords NOTIFY topWordsChanged)
public:
    explicit FileManager(QObject *parent = nullptr);

    QVariantList topWords() const;

signals:
    void topWordsChanged();
    void progressChanged(double);
    void finished();

public slots:
    void start();
    void stop();
    void cancel();
    bool acceptFile(const QString& fileName);

    void sendTopWords(const QVector<WordData>& topWordsList);

private:
    FileWorker * m_worker;
    QFile* m_file;

    QVariantList m_topWords;
};

#endif // FILEMANAGER_H
