#ifndef SONGLISTITEM_H
#define SONGLISTITEM_H
#include <QListWidgetItem>

class SongListItem : public QListWidgetItem{
    public:
        SongListItem();
        void setSongCode(QString);
        QString getSongCode();

    private:
        QString songCode;

};

#endif // SONGLISTITEM_H
