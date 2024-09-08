#ifndef FILEWORKER_H
#define FILEWORKER_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QMutex>
#include <QWaitCondition>
#include <QHash>

#include <QDebug>

struct WordData {
    QString word;
    int count;
};

class FileWorker : public QThread
{
    Q_OBJECT
public:
    explicit FileWorker(QObject *parent = nullptr);
    void setFile(QFile* file);
    void pauseOrResume();
    void cancel();

protected:
    void run() override;

private:
    void mergeToTop(const QHash<QString, int>& minihash);

signals:
    void topWordsChanged(QVector<WordData>);
    void progressChanged(double);

private:
    QFile* m_textFile;

    QMutex m_mutex;
    bool m_pause;
    bool m_cancel;
    QWaitCondition m_wc;

    QHash<QString, int> m_globalHash;
    QVector<WordData> m_topWords;
};

#endif // FILEWORKER_H
