#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStandardItemModel>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButtonOpen_clicked();

    void on_horizontalSliderVolum_valueChanged(int value);

    void on_durationChanged(qint64 duration);



    void on_pushButtonMute_clicked();

    void om_position_change(qint64 position);


    void on_horizontalSliderProgres_valueChanged(int  value);

    void on_current_index_chenges(int position);

private:
    Ui::Widget *ui;

    QMediaPlayer* m_player;
    QMediaPlaylist* m_playlist;
    QStandardItemModel* m_playlist_model;


};
#endif // WIDGET_H
