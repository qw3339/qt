#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QStyle>
#include <QTime>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //Button sstyle

    ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButtonPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->pushButtonStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pushButtonPrev->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->pushButtonNext->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->pushButtonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
// Init player
    m_player = new QMediaPlayer();
    m_player->setVolume(15);
    ui->labelVolum->setText(QString("Volume: ").append(QString::number(m_player->volume())));
    ui->horizontalSliderVolum->setValue(m_player->volume());


    connect(ui->pushButtonPlay, &QPushButton::clicked,this->m_player, &QMediaPlayer::play);
    connect(ui->pushButtonPause, &QPushButton::clicked,this->m_player, &QMediaPlayer::pause);
    connect(ui->pushButtonStop, &QPushButton::clicked,this->m_player, &QMediaPlayer::stop);

    connect(m_player, &QMediaPlayer::durationChanged, this, &Widget::on_durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &Widget::om_position_change);

    // Iniy playlist
    m_playlist_model = new QStandardItemModel(this);
    ui->tableView->setModel(m_playlist_model);
    m_playlist_model->setHorizontalHeaderLabels(QStringList() << "Audio track" << "file");
    ui->tableView->hideColumn(1);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);




    m_playlist = new QMediaPlaylist(m_player);
    m_player->setPlaylist(m_playlist);

    connect(ui->pushButtonPrev, &QPushButton::clicked,m_playlist, &QMediaPlaylist::previous);
    connect(ui->pushButtonNext, &QPushButton::clicked,m_playlist, &QMediaPlaylist::next);

    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &Widget::on_current_index_chenges);


}

Widget::~Widget()
{
    delete m_playlist_model;
    delete m_playlist;
    delete m_player;
    delete ui;
}


void Widget::on_pushButtonOpen_clicked()
{
//    QString file = QFileDialog::getOpenFileName
//            (this,
//             "Open file: ",
//             NULL,
//             "Audio files (*mp3 *.flac)"
//             );

//    ui->labelFile->setText(file);
//    m_player->setMedia(QUrl::fromLocalFile(file));
//    m_player->play();
//    m_player->media(); "C:\Users\79297\Music"
//    this->setWindowTitle(QString("MediaPlayer - ").append(file.split('/').last()));
    QStringList files = QFileDialog::getOpenFileNames
            (
                this,
                "Open files",
                 "C:\\Users\\79297\\Music\\m",
                "Audio files (*mp3 *flac);;mp3 (*mp3);;FLAC (*flac)"
             );
    for(QString file:files)
    {
        QList<QStandardItem*> items;
        items.append(new QStandardItem(QDir(file).dirName()));
        items.append(new QStandardItem(file));
        m_playlist_model->appendRow(items);
        m_playlist->addMedia(QUrl(file));



    }


}


void Widget::on_horizontalSliderVolum_valueChanged(int value)
{
    m_player->setVolume(value);
    ui->labelVolum->setText(QString("Volume: ").append(QString::number(m_player->volume())));

}

void Widget::on_durationChanged(qint64 duration)
{

    ui->horizontalSliderProgres->setMaximum(duration);
    QTime qt_duration = QTime::fromMSecsSinceStartOfDay(duration);
    ui->labelDuration->setText(QString("Duration: ").append(qt_duration.toString("hh:mm:ss")));

}


void Widget::on_pushButtonMute_clicked()
{
    m_player->setMuted(!m_player->isMuted());
    ui->pushButtonMute->setIcon(style()->standardIcon(m_player->isMuted()? QStyle::SP_MediaVolumeMuted : QStyle::SP_MediaVolume));
}

void Widget::om_position_change(qint64 position)
{
    ui->horizontalSliderProgres->setValue(position);
    QTime qt_position = QTime::fromMSecsSinceStartOfDay(position);
    ui->labelProgres->setText(QString("Progress: ").append(qt_position.toString("mm:ss")));


}


void Widget::on_horizontalSliderProgres_valueChanged(int value)
{
    if(ui->horizontalSliderProgres->isSliderDown())
        m_player->setPosition(value);


}

void Widget::on_current_index_chenges(int position)
{
    ui->tableView->selectRow(position);
    QStandardItem* song = m_playlist_model->item(position, 0);
    this->setWindowTitle(QString("MediaPlayer: ").append(song->text()));

    QStandardItem* file = m_playlist_model->item(position, 0);
    ui->labelFile->setText(QString("Song file: ").append(file->text()));


}

