/*
   Babe - tiny music player
   Copyright (C) 2017  Camilo Higuita
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

   */


#include "babetable.h"


BabeTable::BabeTable(QWidget *parent) : QTableWidget(parent) {

    /* connection = new CollectionDB();
  connection->openCollection("../player/collection.db");*/

    connect(this, SIGNAL(doubleClicked(QModelIndex)), this,
            SLOT(on_tableWidget_doubleClicked(QModelIndex)));
    //connect(this->model(),SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),this,SLOT(itemEdited(const QModelIndex&, const QModelIndex&)));
    this->setFrameShape(QFrame::NoFrame);
    this->setColumnCount(10);
    this->setHorizontalHeaderLabels({"Track", "Tile", "Artist", "Album", "Genre",
                                     "Location", "Stars", "Babe", "Art", "Played",
                                     "Playlist"});
    this->horizontalHeader()->setDefaultSectionSize(150);
    this->setMinimumSize(0, 0);
    this->verticalHeader()->setVisible(false);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setAlternatingRowColors(true);
    this->setSortingEnabled(true);
    this->horizontalHeader()->setHighlightSections(false);
    this->horizontalHeader()->setStretchLastSection(true);

    this->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);

    // this->setGridStyle(Qt::PenStyle);
    this->setShowGrid(false);

    this->setColumnWidth(TRACK, 20);
    this->setColumnWidth(PLAYED, 20);
    this->setColumnWidth(STARS, 80);
    this->hideColumn(LOCATION);
    this->hideColumn(STARS);
    this->hideColumn(BABE);
    this->hideColumn(GENRE);
    this->hideColumn(TRACK);
    this->hideColumn(PLAYED);
    this->hideColumn(ART);

    fav1 = new QToolButton();
    fav2 = new QToolButton();
    fav3 = new QToolButton();
    fav4 = new QToolButton();
    fav5 = new QToolButton();
    fav1->setAutoRaise(true);
    fav1->setMaximumSize(16, 16);
    fav2->setAutoRaise(true);
    fav2->setMaximumSize(16, 16);
    fav3->setAutoRaise(true);
    fav3->setMaximumSize(16, 16);
    fav4->setAutoRaise(true);
    fav4->setMaximumSize(16, 16);
    fav5->setAutoRaise(true);
    fav5->setMaximumSize(16, 16);
    fav1->setIcon(QIcon::fromTheme("rating-unrated"));
    fav2->setIcon(QIcon::fromTheme("rating-unrated"));
    fav3->setIcon(QIcon::fromTheme("rating-unrated"));
    fav4->setIcon(QIcon::fromTheme("rating-unrated"));
    fav5->setIcon(QIcon::fromTheme("rating-unrated"));

    // this->horizontalHeaderItem(0);
    // this->horizontalHeaderItem(0)->setResizeMode(1, QHeaderView::Interactive);
    // this->horizontalHeader()->setHighlightSections(true);
    contextMenu = new QMenu(this);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);

    auto babeIt = new QAction("Babe it \xe2\x99\xa1", contextMenu);
    this->addAction(babeIt);

    auto queueIt = new QAction("Queue", contextMenu);
    this->addAction(queueIt);

    /*auto sendIt = new QAction("Send to phone... ", contextMenu);
    this->addAction(sendIt);*/

    QAction *sendEntry = contextMenu->addAction("Send to phone...");
    this->addAction(sendEntry);
    sendToMenu = new QMenu("...");
    sendEntry->setMenu(sendToMenu);


    auto infoIt = new QAction("Info + ", contextMenu);
    this->addAction(infoIt);

    auto editIt = new QAction("Edit", contextMenu);
    this->addAction(editIt);

    auto saveIt = new QAction("Save to... ", contextMenu);
    this->addAction(saveIt);

    auto removeIt = new QAction("Remove", contextMenu);
    this->addAction(removeIt);

    QAction *addEntry = contextMenu->addAction("Add to...");
    this->addAction(addEntry);
    playlistsMenu = new QMenu("...");
    addEntry->setMenu(playlistsMenu);

    // auto moodIt = new QAction("Mood..", contextMenu);
    // this->addAction(moodIt);

    /*QAction *moodEntry = contextMenu->addAction("Mood...");
  this->addAction(moodEntry);
  moodMenu = new QMenu("...");
  moodEntry->setMenu(moodMenu);*/
    // passPlaylists();
    // playlistsMenu->addAction("hello rold");

    connect(this, SIGNAL(rightClicked(QPoint)), this,
            SLOT(setUpContextMenu(QPoint)));
    connect(playlistsMenu, SIGNAL(triggered(QAction *)), this,
            SLOT(addToPlaylist(QAction *)));
    connect(sendToMenu, SIGNAL(triggered(QAction *)), this,
            SLOT(sendIt_action(QAction *)));


    connect(babeIt, SIGNAL(triggered()), this, SLOT(babeIt_action()));
    connect(queueIt, SIGNAL(triggered()), this, SLOT(queueIt_action()));
    //connect(sendIt, SIGNAL(triggered()), this, SLOT(sendIt_action()));
    connect(infoIt, SIGNAL(triggered()), this, SLOT(infoIt_action()));
    connect(editIt, SIGNAL(triggered()), this, SLOT(editIt_action()));
    connect(removeIt, SIGNAL(triggered()), this, SLOT(removeIt_action()));



    QButtonGroup *bg = new QButtonGroup(contextMenu);
    bg->addButton(fav1, 1);
    bg->addButton(fav2, 2);
    bg->addButton(fav3, 3);
    bg->addButton(fav4, 4);
    bg->addButton(fav5, 5);
    // connect(fav1,SIGNAL(enterEvent(QEvent)),this,hoverEvent());
    connect(bg, SIGNAL(buttonClicked(int)), this, SLOT(rateGroup(int)));
    auto gr = new QWidget();
    auto ty = new QHBoxLayout();
    ty->addWidget(fav1);
    ty->addWidget(fav2);
    ty->addWidget(fav3);
    ty->addWidget(fav4);
    ty->addWidget(fav5);

    gr->setLayout(ty);

    QWidgetAction *chkBoxAction = new QWidgetAction(contextMenu);
    chkBoxAction->setDefaultWidget(gr);

    this->addAction(chkBoxAction);

    auto moods = new QWidget();
    auto moodsLayout = new QHBoxLayout();
    QButtonGroup *moodGroup = new QButtonGroup(contextMenu);
    connect(moodGroup, SIGNAL(buttonClicked(int)), this, SLOT(moodTrack(int)));
    for(int i=0; i<5; i++)
    {
        auto  *colorTag = new QToolButton();
        //colorTag->setIconSize(QSize(10,10));
        colorTag->setFixedSize(15,15);
        // colorTag->setAutoRaise(true);
        colorTag->setStyleSheet(QString("QToolButton { background-color: %1;}").arg(colors.at(i)));
        moodGroup->addButton(colorTag,i);
        moodsLayout->addWidget(colorTag);
    }
    moods->setLayout(moodsLayout);

    QWidgetAction *moodsAction = new QWidgetAction(contextMenu);
    moodsAction->setDefaultWidget(moods);

    this->addAction(moodsAction);
}

BabeTable::~BabeTable() {  }

void BabeTable::moodTrack(int color)
{

    moodIt_action(colors.at(color));
}

void BabeTable::addToPlaylist(QAction *action) {

    QString playlist = action->text().replace("&", "");
    QString location =
            this->model()->data(this->model()->index(rRow, LOCATION)).toString();

    if (playlist.contains("Create new...")) {
        qDebug() << "trying to create a new playlist" << playlist;

        emit createPlaylist_clicked();
    } else {
        populatePlaylist({location}, playlist);
    }
}

void BabeTable::populatePlaylist(QStringList urls, QString playlist)  //this needs to get fixed
{

    for (auto location : urls)
    {

        // ui->fav_btn->setIcon(QIcon::fromTheme("face-in-love"));
        qDebug() << "Song to add: " << location << " to: " << playlist;

        QSqlQuery query = connection->getQuery(
                    "SELECT * FROM tracks WHERE location = \"" + location + "\"");

        QString list;
        while (query.next())
            list = query.value(PLAYLIST).toString();
        list += " " + playlist;
        // qDebug()<<played;

        if (connection->insertInto("tracks", "playlist", location, list))
        {
            // ui->fav_btn->setIcon(QIcon(":Data/data/love-amarok.svg"));
            qDebug() << list;
        }

    }
}



void BabeTable::passPlaylists() {}

void BabeTable::enterEvent(QEvent *event) {
    // qDebug()<<"entered the playlist";
    Q_UNUSED(event);
    // emit enteredTable();
}

void BabeTable::leaveEvent(QEvent *event) {
    // qDebug()<<"left the playlist";
    Q_UNUSED(event);
    //  emit leftTable();
}

void BabeTable::passStyle(QString style) { this->setStyleSheet(style); }

void BabeTable::addRow(QStringList list) {
    QString track,title,artist,album,genre,location,stars,babe,art,played,playlist;

    track= list.at(TRACK);
    title=list.at(TITLE);
    artist=list.at(ARTIST);
    album=list.at(ALBUM);
    genre=list.at(GENRE);
    location=list.at(LOCATION);
    stars=list.at(STARS);
    babe=list.at(BABE);
    art=list.at(ART);
    played=list.at(PLAYED);
    playlist=list.at(PLAYLIST);

    this->insertRow(this->rowCount());


    this->setItem(this->rowCount() - 1, TRACK, new QTableWidgetItem(track));
    this->setItem(this->rowCount() - 1, TITLE, new QTableWidgetItem(title));
    this->setItem(this->rowCount() - 1, ARTIST, new QTableWidgetItem(artist));
    this->setItem(this->rowCount() - 1, ALBUM, new QTableWidgetItem(album));
    this->setItem(this->rowCount() - 1, GENRE, new QTableWidgetItem(genre));
    this->setItem(this->rowCount() - 1, LOCATION, new QTableWidgetItem(location));
    this->setItem(this->rowCount() - 1, STARS, new QTableWidgetItem(stars));
    this->setItem(this->rowCount() - 1, BABE, new QTableWidgetItem(babe));
    this->setItem(this->rowCount() - 1, ART, new QTableWidgetItem(art));
    this->setItem(this->rowCount() - 1, PLAYED, new QTableWidgetItem(played));
    this->setItem(this->rowCount() - 1, PLAYLIST, new QTableWidgetItem(playlist));
    this->item(this->rowCount()-1,1)->setToolTip( "by "+artist);
}

void BabeTable::populateTableView(QString indication, bool descriptiveTitle) {
    // this->clearContents();
    this->setSortingEnabled(false);
    QSqlQuery query = connection->getQuery(indication);
    bool missingDialog = false;
    QStringList missingFiles;
    qDebug() << "ON POPULATE:"<<indication;

    if(connection->checkQuery(indication))
    {

        while (query.next()) {

            if (!QFileInfo(query.value(LOCATION).toString()).exists()) {
                qDebug() << "That file doesn't exists anymore: "
                         << query.value(LOCATION).toString();
                missingFiles << query.value(LOCATION).toString();
                missingDialog = true;
            } else {
                this->insertRow(this->rowCount());

                auto *track = new QTableWidgetItem(query.value(TRACK).toString());
                this->setItem(this->rowCount() - 1, TRACK, track);

                auto *title = new QTableWidgetItem(query.value(TITLE).toString());
                this->setItem(this->rowCount() - 1, TITLE, title);

                if(descriptiveTitle) title->setToolTip("by "+query.value(ARTIST).toString());


                // title->setFlags(title->flags() & ~Qt::ItemIsEditable);


                auto *artist = new QTableWidgetItem(query.value(ARTIST).toString());
                this->setItem(this->rowCount() - 1, ARTIST, artist);

                // qDebug()<<query.value(2).toString();
                auto *album = new QTableWidgetItem(query.value(ALBUM).toString());
                this->setItem(this->rowCount() - 1, ALBUM, album);

                auto *genre = new QTableWidgetItem(query.value(GENRE).toString());
                this->setItem(this->rowCount() - 1, GENRE, genre);

                auto *location = new QTableWidgetItem(query.value(LOCATION).toString());
                this->setItem(this->rowCount() - 1, LOCATION, location);

                QString rating;
                switch (query.value((STARS)).toInt()) {
                case 0:
                    rating = " ";
                    break;
                case 1:
                    rating = "\xe2\x98\x86 ";
                    break;
                case 2:
                    rating = "\xe2\x98\x86 \xe2\x98\x86 ";
                    break;
                case 3:
                    rating = "\xe2\x98\x86 \xe2\x98\x86 \xe2\x98\x86 ";
                    break;
                case 4:
                    rating = "\xe2\x98\x86 \xe2\x98\x86 \xe2\x98\x86 \xe2\x98\x86 ";
                    break;
                case 5:
                    rating =
                            "\xe2\x98\x86 \xe2\x98\x86 \xe2\x98\x86 \xe2\x98\x86 \xe2\x98\x86 ";
                    break;
                }

                if (query.value(BABE).toInt() == 1)
                    rating = "\xe2\x99\xa1 ";

                auto *stars = new QTableWidgetItem(rating);
                this->setItem(this->rowCount() - 1, STARS, stars);

                QString bb;
                switch (query.value((BABE)).toInt()) {
                case 0:
                    bb = " ";
                    break;
                case 1:
                    bb = "\xe2\x99\xa1 ";
                    break;
                }

                auto *babe = new QTableWidgetItem(bb);
                this->setItem(this->rowCount() - 1, BABE, babe);

                auto *art = new QTableWidgetItem(query.value(ART).toString());
                this->setItem(this->rowCount() - 1, ART, art);

                auto *played = new QTableWidgetItem(query.value(PLAYED).toString());
                this->setItem(this->rowCount() - 1, PLAYED, played);
            }
        }
        // this->sortByColumn(1);

        if (missingDialog) {
            QString parentDir;
            // QMessageBox::about(this,"Removing missing
            // files",missingFiles.join("\n"));

            Notify nof;
            nof.notifyUrgent("Removing missing files...",missingFiles.join("\n"));

            for (auto file_r : missingFiles)
            {
                parentDir=QFileInfo(QFileInfo(file_r)).dir().path();
                if (!QFileInfo(parentDir).exists())
                {
                    connection->removePath(parentDir);
                    qDebug()<<"the parent file doesn't exists"<<parentDir;
                }else
                {

                    connection->removePath(file_r);

                    qDebug() << "deleted from db: " << file_r;
                }



            }
            connection->setCollectionLists();
            connection->cleanCollectionLists();


        }

        this->setSortingEnabled(true);
        emit finishedPopulating();
    }else
    {
        qDebug()<<"Error: the query didn't pass"<<indication;
    }
    //
    // this->sortByColumn(1,Qt::AscendingOrder);
    /*for (Track track : collection.getTracks() )
  {
   this->insertRow(this->rowCount());
   auto *title= new QTableWidgetItem( QString::fromStdString(track.getTitle()));
   //title->setFlags(title->flags() & ~Qt::ItemIsEditable);

   this->setItem(this->rowCount()-1, TITLE, title);

   auto *artist= new QTableWidgetItem(
  QString::fromStdString(track.getArtist()));
   this->setItem(this->rowCount()-1, ARTIST, artist);

   auto *album= new QTableWidgetItem( QString::fromStdString(track.getAlbum()));
   this->setItem(this->rowCount()-1, ALBUM, album);

   auto *location= new QTableWidgetItem(
  QString::fromStdString(track.getLocation()));
   this->setItem(this->rowCount()-1, LOCATION, location);

  }*/
}

void BabeTable::setRating(int rate) {
    switch (rate) {
    case 0:
        fav1->setIcon(QIcon::fromTheme("rating-unrated"));
        fav2->setIcon(QIcon::fromTheme("rating-unrated"));
        fav3->setIcon(QIcon::fromTheme("rating-unrated"));
        fav4->setIcon(QIcon::fromTheme("rating-unrated"));
        fav5->setIcon(QIcon::fromTheme("rating-unrated"));
        break;
    case 1:
        fav1->setIcon(QIcon::fromTheme("rating"));
        fav2->setIcon(QIcon::fromTheme("rating-unrated"));
        fav3->setIcon(QIcon::fromTheme("rating-unrated"));
        fav4->setIcon(QIcon::fromTheme("rating-unrated"));
        fav5->setIcon(QIcon::fromTheme("rating-unrated"));
        break;
    case 2:
        fav1->setIcon(QIcon::fromTheme("rating"));
        fav2->setIcon(QIcon::fromTheme("rating"));
        fav3->setIcon(QIcon::fromTheme("rating-unrated"));
        fav4->setIcon(QIcon::fromTheme("rating-unrated"));
        fav5->setIcon(QIcon::fromTheme("rating-unrated"));
        break;
    case 3:
        fav1->setIcon(QIcon::fromTheme("rating"));
        fav2->setIcon(QIcon::fromTheme("rating"));
        fav3->setIcon(QIcon::fromTheme("rating"));
        fav4->setIcon(QIcon::fromTheme("rating-unrated"));
        fav5->setIcon(QIcon::fromTheme("rating-unrated"));

        break;
    case 4:
        fav1->setIcon(QIcon::fromTheme("rating"));
        fav2->setIcon(QIcon::fromTheme("rating"));
        fav3->setIcon(QIcon::fromTheme("rating"));
        fav4->setIcon(QIcon::fromTheme("rating"));
        fav5->setIcon(QIcon::fromTheme("rating-unrated"));
        break;
    case 5:
        fav1->setIcon(QIcon::fromTheme("rating"));
        fav2->setIcon(QIcon::fromTheme("rating"));
        fav3->setIcon(QIcon::fromTheme("rating"));
        fav4->setIcon(QIcon::fromTheme("rating"));
        fav5->setIcon(QIcon::fromTheme("rating"));
        break;
    }
}

void BabeTable::setTableOrder(int column, int order) {
    if (order == DESCENDING) {
        this->sortByColumn(column, Qt::DescendingOrder);
    } else if (order == ASCENDING) {
        this->sortByColumn(column, Qt::AscendingOrder);
    }
}

void BabeTable::setVisibleColumn(int column) {
    if (column == LOCATION) {
        this->showColumn(LOCATION);
    } else if (column == STARS) {
        this->showColumn(STARS);
    } else if (column == BABE) {
        this->showColumn(BABE);
    } else if (column == ALBUM) {
        this->showColumn(ALBUM);
    }
}


QMap<QString,QString> BabeTable::getKdeConnectDevices()
{
    qDebug()<<"getting the kdeconnect devices avaliable";
    QMap<QString,QString> _devices;
    QProcess process;
    process.start("kdeconnect-cli -a");
    process.waitForFinished();
    // auto output = process->readAllStandardOutput();

    process.setReadChannel(QProcess::StandardOutput);

    while (process.canReadLine()) {
        QString line = QString::fromLocal8Bit(process.readLine());
        qDebug()<<"line:"<<line;
        if(line.contains("(paired and reachable)"))
        {
            QStringList items = line.split(" ");
            auto key=QString(items.at(2));
            auto name=QString(items.at(1)).replace(":","");

            qDebug()<<"Founded devices: "<<key<<":"<<name;
            _devices.insert(key,name);
        }
    }

    this->devices=_devices;
    return devices;

}

void BabeTable::setUpContextMenu(QPoint pos)

{
    qDebug() << "setUpContextMenu";
    playlistsMenu->clear();
    for (auto playlist : connection->getPlaylists()) {
        playlistsMenu->addAction(playlist);
    }

    sendToMenu->clear();


    QMapIterator<QString, QString> i(getKdeConnectDevices());
    while (i.hasNext()) {
        i.next();
        qDebug()<<i.key();
        sendToMenu->addAction(i.value());
    }
    // playlistsMenu->addAction("Create new...");
    int rate = 0;
    bool babe = false;
    rRow = this->indexAt(pos).row();
    rColumn= this->indexAt(pos).column();

    // row= this->currentIndex().row(), rate;

    QString url =
            this->model()->data(this->model()->index(rRow, LOCATION)).toString();
    //
    QSqlQuery query = connection->getQuery(
                "SELECT * FROM tracks WHERE location = \"" + url + "\"");

    while (query.next()) {
        rate = query.value(STARS).toInt();
        babe = query.value(BABE).toInt() == 1 ? true : false;

        qDebug() << "se llamó a menu contextual con url: " << url
                 << "rated: " << rate;
    }

    setRating(rate);
    if (babe)
        this->actions().at(0)->setText("Un-Babe it");
    else
        this->actions().at(0)->setText("Babe it");
}

QStringList BabeTable::getPlaylistMenus() {

    playlistsMenus.clear();
    for (auto playlist : connection->getPlaylists()) {
        playlistsMenus << playlist;
    }
    return playlistsMenus;
}

void BabeTable::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Return: {


        QList<QStringList> list;


        list<<getRowData(this->currentIndex().row());
        emit tableWidget_doubleClicked(list);

        break;
    }
    case Qt::Key_Up: {
        QModelIndex index = this->currentIndex();
        int row = index.row() - 1;
        int column = 1;
        QModelIndex newIndex = this->model()->index(row, column);
        this->selectionModel()->select(newIndex, QItemSelectionModel::Select);
        this->setCurrentIndex(newIndex);
        this->setFocus();

        qDebug()
                << this->model()->data(this->model()->index(row, LOCATION)).toString();

        break;
    }
    case Qt::Key_Down: {
        QModelIndex index = this->currentIndex();
        int row = index.row() + 1;
        int column = 1;
        QModelIndex newIndex = this->model()->index(row, column);
        this->selectionModel()->select(newIndex, QItemSelectionModel::Select);
        this->setCurrentIndex(newIndex);
        this->setFocus();

        qDebug()
                << this->model()->data(this->model()->index(row, LOCATION)).toString();
        break;
    }
    default: {
        QTableWidget::keyPressEvent(event);
        break;
    }
    }
}

void BabeTable::mousePressEvent(QMouseEvent *evt) {
    // QTableView::mouseReleaseEvent( event );

    if (evt->button() == Qt::RightButton) {
        qDebug() << "table right clicked";
        evt->accept();
        emit rightClicked(evt->pos());
    } else {
        QTableWidget::mousePressEvent(evt);
    }
}

void BabeTable::rateGroup(int id) {
    qDebug() << "rated with: " << id;
    // int row= this->currentIndex().row();
    QString location =
            this->model()->data(this->model()->index(rRow, LOCATION)).toString();

    QSqlQuery query = connection->getQuery(
                "SELECT * FROM tracks WHERE location = \"" + location + "\"");

    int rate = 0;

    while (query.next())
        rate = query.value(STARS).toInt();

    if (connection->check_existance("tracks", "location", location)) {
        if (connection->insertInto("tracks", "stars", location, id)) {
            setRating(id);

            // this->model()->data(this->model()->index(row,1)).
        }
        //qDebug() << "rating the song of rowffff: " << row;

        QString stars;
        for (int i = 0; i < id; i++) {
            stars += "\xe2\x98\x86 ";
        }
        this->item(rRow, STARS)->setText(stars);

        if (id > 0 && rate < 5) {
            QString title =
                    this->model()->data(this->model()->index(rRow, TITLE)).toString();
            QString artist =
                    this->model()->data(this->model()->index(rRow, ARTIST)).toString();
            QString album =
                    this->model()->data(this->model()->index(rRow, ALBUM)).toString();
            QString star =
                    this->model()->data(this->model()->index(rRow, STARS)).toString();
            QString babe =
                    this->model()->data(this->model()->index(rRow, BABE)).toString();

            qDebug() << "rated and trying to add to favs";
            emit songRated({title, artist, album, location, star, babe});
        } else {
            qDebug() << "rated and trying to add to favs failed";
        }
        // this->update();
    } else {
    }
}

QStringList BabeTable::getRowData(int row)
{
    QStringList file;

    for(auto i=0; i<this->columnCount()+1;i++)
        file<< this->model()->data(this->model()->index(row, i)).toString();


    return file;
}

void BabeTable::allowDrag()
{

}

void BabeTable::on_tableWidget_doubleClicked(const QModelIndex &index)
{

    QList<QStringList> list;
    qDebug()
            << "BabeTable doubleClicked item<<"
            << index.sibling(this->currentIndex().row(), LOCATION).data().toString();

    list<<getRowData(this->currentIndex().row());

    emit tableWidget_doubleClicked(list);

}

void BabeTable::babeIt_action()
{
    qDebug() << "right clicked!";
    // int row= this->currentIndex().row();
    qDebug()
            << this->model()->data(this->model()->index(rRow, LOCATION)).toString();
    QList<QStringList> list;
    list<<getRowData(rRow);
    emit babeIt_clicked(list);
}

void BabeTable::sendIt_action(QAction *device)
{
    QString url = this->model()->index(rRow,LOCATION).data().toString();
    QString title = this->model()->index(rRow,TITLE).data().toString();
    QString artist = this->model()->index(rRow,ARTIST).data().toString();

    QString deviceName= device->text().replace("&","");
    QString deviceKey=  devices.key(deviceName);

    qDebug()<<"trying to send "<< url << "to : "<< deviceName;
    auto process = new QProcess(this);

    connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus){

        qDebug()<<"processFinished_totally"<<exitCode<<exitStatus;
        Notify nof;
        nof.notifyUrgent("Song send to " + deviceName,title +" by "+ artist);

    });
    qDebug()<<"kdeconnect-cli -d " +deviceKey+ " --share " + url;
    process->start("kdeconnect-cli -d " +deviceKey+ " --share " +"\""+ url+"\"");

}

void BabeTable::editIt_action()
{
    //editing=true;
    // emit this->edit(this->model()->index(rRow,rColumn));

    auto infoForm = new Form(getRowData(rRow),this);
    connect(infoForm,SIGNAL(infoModified(QMap<int, QString>)),this,SLOT(itemEdited(QMap<int, QString>)));
    infoForm->show();

}

void BabeTable::itemEdited(QMap<int, QString> map)
{
    qDebug()<<"item changed: " << map[TITLE];
    this->item(rRow,TRACK)->setText(map[TRACK]);
    this->item(rRow,TITLE)->setText(map[TITLE]);
    this->item(rRow,ARTIST)->setText(map[ARTIST]);
    this->item(rRow,ALBUM)->setText(map[ALBUM]);
    this->item(rRow,GENRE)->setText(map[GENRE]);


    //     connection->insertInto("tracks",column,this->model()->index(newIndex.row(),LOCATION).data().toString(),newIndex.data().toString());

}

void BabeTable::infoIt_action()
{
    QString artist = this->model()->data(this->model()->index(rRow, ARTIST)).toString();
    QString album = this->model()->data(this->model()->index(rRow, ALBUM)).toString();
    QString title = this->model()->data(this->model()->index(rRow, TITLE)).toString();
    emit infoIt_clicked(title, artist, album);
}

void BabeTable::removeIt_action()
{
    qDebug() << "removeIt/right clicked!";
    // int row= this->currentIndex().row();
    qDebug()
            << this->model()->data(this->model()->index(rRow, LOCATION)).toString();
    this->removeRow(rRow);
    emit removeIt_clicked(rRow);
}

void BabeTable::moodIt_action(QString color)
{
    qDebug() << "right clicked!";
    // int row= this->currentIndex().row();
    qDebug()
            << this->model()->data(this->model()->index(rRow, LOCATION)).toString();

    // QColor color = QColorDialog::getColor(Qt::black, this, "Pick a Mood",  QColorDialog::DontUseNativeDialog);
    qDebug()<< color;

    if(!color.isEmpty())
    {
        QSqlQuery query;
        query.prepare("UPDATE tracks SET art = (:art) WHERE location = (:location)" );
        //query.prepare("SELECT * FROM "+tableName+" WHERE "+searchId+" = (:search)");
        query.bindValue(":art",  color);
        query.bindValue(":location", this->model()->data(this->model()->index(rRow, LOCATION)).toString());

        if(query.exec())
        {
            qDebug()<<"Art[color] inserted into DB"<< color;
            emit moodIt_clicked(color);

        }else
        {
            qDebug()<<"COULDN'T insert art[color] into DB";
        }
    }


    //emit moodIt_clicked(this->model()->data(this->model()->index(row, LOCATION)).toString(),color.name());
    /*emit babeIt_clicked(
      {this->model()->data(this->model()->index(row, LOCATION)).toString()});*/
}


void BabeTable::queueIt_action()
{
    qDebug() << "queueIt clicked!";
    // int row= this->currentIndex().row();
    QString url = this->model()->data(this->model()->index(rRow, LOCATION)).toString();
    qDebug()<<url;
    emit queueIt_clicked(url);

}

void BabeTable::flushTable()
{
    this->clearContents();
    this->setRowCount(0);
}

void BabeTable::passCollectionConnection(CollectionDB *con)
{
    connection = con;
}

QStringList BabeTable::getTableContent(int column)
{
    QStringList result;
    for (int i = 0; i < this->rowCount(); i++)
    {
        result << this->model()->data(this->model()->index(i, column)).toString();
    }

    return result;
}


QList<QStringList> BabeTable::getAllTableContent()
{
    QList<QStringList> result;


    for (int i = 0; i < this->rowCount(); i++)
    {

        result<<getRowData(i);
    }

    return result;
}
