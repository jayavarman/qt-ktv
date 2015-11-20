#include "songlistitem.h"

SongListItem::SongListItem(){

}

void SongListItem::setSongCode(QString code){
    this->songCode = code;
}

QString SongListItem::getSongCode(){
    return this->songCode;
}
