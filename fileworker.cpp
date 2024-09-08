#include "fileworker.h"

FileWorker::FileWorker(QObject *parent)
    : QThread{parent}, m_pause(false), m_cancel(false)
{}

void FileWorker::setFile(QFile *file)
{
    m_textFile = file;
}

void FileWorker::pauseOrResume()
{
    QMutexLocker locker(&m_mutex);
    m_pause = !m_pause;
    if (!m_pause) {
        m_wc.wakeAll();
    }
}

void FileWorker::cancel()
{
    QMutexLocker locker(&m_mutex);
    m_cancel = true;
    m_wc.wakeAll();
}

void FileWorker::run()
{
    m_topWords.clear();
    m_globalHash.clear();
    m_pause = false;
    m_cancel = false;

    QTextStream in(m_textFile);
    QString line;
    qint64 bytesSum = 0;
    double progress = 0.0;
    while(in.readLineInto(&line) && !m_cancel){
        {
            QMutexLocker locker(&m_mutex);
            if (m_pause) {
                m_wc.wait(&m_mutex);
                if (m_cancel) {
                    m_topWords.clear();
                    m_globalHash.clear();
                    m_topWords.clear();
                    progress = 0;

                    emit topWordsChanged(m_topWords);
                    emit progressChanged(progress);
                    break;
                }
            }
        }
        line = line.simplified().toLower();
        QStringList tokens;
        tokens = line.split(' ', Qt::SkipEmptyParts);
        QHash<QString, int> miniHash;
        foreach(QString word, tokens) {
            miniHash[word]++;
            m_globalHash[word]++;
        }
        mergeToTop(miniHash);
        miniHash.clear();

        progress = 100.0 - (static_cast<double>(m_textFile->bytesAvailable()) / m_textFile->size())*100;
        emit topWordsChanged(m_topWords);
        emit progressChanged(progress);
    }

    if (m_cancel) {
        m_topWords.clear();
        m_globalHash.clear();
        progress = 0;

        emit topWordsChanged(m_topWords);
        emit progressChanged(progress);
    }

    m_textFile->close();
}

void FileWorker::mergeToTop(const QHash<QString, int> &minihash)
{
    foreach(const QString& key, minihash.keys()) {
        auto it = std::find_if(m_topWords.begin(), m_topWords.end(),
                               [&](const WordData& data) { return data.word == key; });

        if (it != m_topWords.end()) {
            it->count = m_globalHash[key];
        } else {
            m_topWords.push_back({key, m_globalHash[key]});
        }
    }

    std::sort(
        m_topWords.begin(), m_topWords.end(),
        [](const WordData& a, const WordData& b) { return a.count > b.count; });

    if (m_topWords.size() > 15) {
        m_topWords = QVector<WordData>(m_topWords.begin(), m_topWords.begin() + 15);
    }

}
