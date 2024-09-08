#include "filemanager.h"

FileManager::FileManager(QObject *parent)
    : QObject{parent}, m_file(new QFile(this)), m_worker(new FileWorker(this))
{
    QObject::connect(m_worker, &FileWorker::topWordsChanged,
                     this, &FileManager::sendTopWords);
    QObject::connect(m_worker, &FileWorker::progressChanged,
                     this, &FileManager::progressChanged);
    QObject::connect(m_worker, &QThread::finished,
                     this, &FileManager::finished);
}

void FileManager::start()
{
    m_worker->start();
}

void FileManager::stop()
{
    m_worker->pauseOrResume();
}

void FileManager::cancel()
{
    m_worker->cancel();
    emit topWordsChanged();
    emit progressChanged(0.0);
}

bool FileManager::acceptFile(const QString& fileName)
{
    m_file->setFileName(fileName.sliced(8));
    if (!m_file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    m_worker->setFile(m_file);
    return true;
}

void FileManager::sendTopWords(const QVector<WordData> &topWordsList)
{
    m_topWords.clear();
    for (const auto& word : topWordsList) {
        QVariantMap map;
        map["word"] = word.word;
        map["count"] = word.count;
        m_topWords.push_back(map);
    }

    emit topWordsChanged();
}

QVariantList FileManager::topWords() const
{
    return m_topWords;
}
