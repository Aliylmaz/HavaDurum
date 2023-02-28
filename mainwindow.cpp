#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateWeatherData(QJsonObject dataObj)
{
    // Tüm hava durumu kodlarını bir QVector'e ekleyin
    QVector<QString> weatherCodes;
    QJsonArray weatherArray = dataObj.value("weather").toArray();
    for (int i = 0; i < weatherArray.size(); i++) {
        QJsonObject weatherObj = weatherArray.at(i).toObject();
        QString weatherCode = weatherObj.value("icon").toString();
        weatherCodes.append(weatherCode);
    }

    // İlk 8 hava durumu kodu için setWeatherIcon() fonksiyonunu çağırın
    int numIconsToSet = qMin(weatherCodes.size(), 40); // en fazla 8 ikon ayarla
    for (int i = 0; i < numIconsToSet; i++) {
        setWeatherIcon(weatherCodes.at(i));
    }
}




/*
 *     // Hava durumuna göre resimleri tutan QMap'i oluşturun
    QMap<QString, QPixmap> weatherIcons;
    weatherIcons.insert("01d", QPixmap(":/images/images/01d.png"));
    weatherIcons.insert("04d", QPixmap(":/images/images/04d.png"));
    weatherIcons.insert("09d", QPixmap(":/images/images/09d.png"));
    */

void MainWindow::setWeatherIcon(QString weatherCode)
{
    QMap<QString, QPixmap> weatherIcons;
    weatherIcons.insert("01d", QPixmap(":/images/images/01d.png"));
    weatherIcons.insert("02d", QPixmap(":/images/images/02d.png"));
    weatherIcons.insert("03d", QPixmap(":/images/images/03d.png"));
    weatherIcons.insert("04d", QPixmap(":/images/images/04d.png"));
    weatherIcons.insert("09d", QPixmap(":/images/images/09d.png"));
    weatherIcons.insert("10d", QPixmap(":/images/images/10d.png"));
    weatherIcons.insert("11d", QPixmap(":/images/images/11d.png"));
    weatherIcons.insert("13d", QPixmap(":/images/images/13d.png"));


    // weatherCode değişkeni QMap içinde var mı kontrol etmek için contains() fonksiyonu kullanılıyor
    // contains() fonksiyonu QMap içinde verilen anahtarı (key) içerip içermediğini kontrol eder
    if (weatherIcons.contains(weatherCode)) {
        QPixmap pixmap = weatherIcons.value(weatherCode);
        int labelWidth = ui->currentWeatherIcon->width();
        int labelHeight = ui->currentWeatherIcon->height();
        pixmap = pixmap.scaled(labelWidth, labelHeight, Qt::KeepAspectRatio);
        ui->currentWeatherIcon->setPixmap(pixmap);
        ui->image_first->setPixmap(pixmap);
        ui->image_first_2->setPixmap(pixmap);
        ui->image_first_3->setPixmap(pixmap);
        ui->image_first_4->setPixmap(pixmap);
        ui->image_first_5->setPixmap(pixmap);
         ui->image_first_6->setPixmap(pixmap);
          ui->image_first_7->setPixmap(pixmap);
           ui->image_first_8->setPixmap(pixmap);
           ui->image_pzt->setPixmap(pixmap);
            ui->image_sali->setPixmap(pixmap);
             ui->image_crs->setPixmap(pixmap);
              ui->image_prs->setPixmap(pixmap);
               ui->image_cuma->setPixmap(pixmap);
    } else {
        // tanımlanmayan hava durumu durumunda varsayılan resmi gösterin
        QPixmap pixmap = weatherIcons.value("01d");
        int labelWidth = ui->currentWeatherIcon->width();
        int labelHeight = ui->currentWeatherIcon->height();
        pixmap = pixmap.scaled(labelWidth, labelHeight, Qt::KeepAspectRatio);
        ui->currentWeatherIcon->setPixmap(pixmap);
        ui->image_first->setPixmap(pixmap);
        ui->image_first_2->setPixmap(pixmap);
        ui->image_first_3->setPixmap(pixmap);
        ui->image_first_4->setPixmap(pixmap);
         ui->image_first_5->setPixmap(pixmap);
          ui->image_first_6->setPixmap(pixmap);
           ui->image_first_7->setPixmap(pixmap);
            ui->image_first_8->setPixmap(pixmap);

    }
}





void MainWindow::on_Guncelle_clicked(){

    // Firebase bağlantı bilgileri
    const QString firebaseDatabaseUrl = "https://pythonfirebase-d6440-default-rtdb.firebaseio.com/";
    const QString firebaseApiKey = "AIzaSyAG5CQn9fyX_JIzBq41JWQ34jOoRTXQex4";

    // Firebase REST API için URL
    const QString firebaseUrl = firebaseDatabaseUrl + "weather.json?auth=" + firebaseApiKey;

    // HTTP isteği oluşturma
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(firebaseUrl));

    // İstek gönderme
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=](){

        // İstek tamamlandığında çalışacak kod
        if (reply->error() == QNetworkReply::NoError) {

            // Veri alma

            QByteArray response = reply->readAll();



            QString jsonData = QString::fromUtf8(response.data(), response.size());
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject obj = doc.object();


            // Tarih ve saat bilgilerini ayırın.
            QJsonArray dataList = obj.value("list").toArray();
            for (int i = dataList.size()-1; i>=0 ; i--) {
                QJsonObject dataObj = dataList[i].toObject();
                QDateTime currentDateTime = QDateTime::currentDateTime();
                 double temp = dataObj.value("main").toObject().value("temp").toDouble();


                 for(int i=0; i<5; i++){
                     QDateTime fiveDaysLaterDateTime = currentDateTime.addDays(i);
                     QString dayOfWeek = fiveDaysLaterDateTime.toString("dddd");
                     QString dayOfMonth = fiveDaysLaterDateTime.toString("dd");
                     QString month = fiveDaysLaterDateTime.toString("MMMM");
                     QString year = fiveDaysLaterDateTime.toString("yyyy");

                     QString labelText =dayOfMonth + " " + month ;
                     QString labeltext=dayOfWeek;
                     QLabel *label = nullptr;
                      QLabel *label1 = nullptr;

                     if (i == 0) {
                         label = ui->Datepzt;
                         label1=ui->Daypzt;
                     } else if (i == 1) {
                         label = ui->DateSali;
                         label1=ui->Daysali;
                     } else if (i == 2) {
                         label = ui->DateCrs;
                         label1=ui->Daycars;
                     } else if (i == 3) {
                         label = ui->DatePrs;
                         label1=ui->Daypers;
                     } else if (i == 4) {
                         label = ui->DateCuma;
                         label1=ui->Daycuma;
                     }

                     label->setText(labelText);
                     label1->setText(labeltext);
                 }
                 ui->minmax_pzt->setText((QString(" 11/17°C")));
                  ui->minmax_sali->setText((QString(" 9/13°C")));
                  ui->minmax_cars->setText((QString(" 12/15°C")));
                   ui->minmax_pers->setText((QString(" 10/13°C")));
                    ui->minmax_cuma->setText((QString(" 11/15°C")));





                for (int i = 0; i < dataList.size(); i++) {
                    //QJsonObject dataObj = dataList[i].toObject();
                    QString dateTimeStr = dataObj.value("dt_txt").toString();
                    QDateTime dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd hh:mm:ss");

                    if (dateTime > currentDateTime) {
                        // Bir sonraki 3 saatlik zaman dilimini bulun
                        QDateTime nextDateTime = dateTime.addSecs(3 * 3600 - dateTime.time().second() - dateTime.time().minute() * 60);


                        // 3 saatlik zaman dilimlerini işleyin
                        for (int j = 0; j < 8; j++) {
                            QDateTime timeToProcess = nextDateTime.addSecs(j * 3 * 3600);
                            QString timeStr = timeToProcess.toString("hh:mm");

                            qDebug() <<temp;


                            // burada sıcaklığı işleyin
                            if (j == 0) {
                                ui->temp_3->setText(QString(" %1°C").arg(temp-276));
                                ui->time_3->setText(timeStr);
                            } else if (j == 1) {
                                ui->temp_6->setText(QString(" %1°C").arg(temp-278));
                                ui->time_6->setText(timeStr);
                            } else if (j == 2) {
                                ui->temp_5->setText(QString(" %1°C").arg(temp-273.9));
                                ui->time_5->setText(timeStr);
                            } else if (j == 3) {
                                ui->temp_7->setText(QString(" %1°C").arg(temp-278.4));
                                ui->time_7->setText(timeStr);
                            } else if (j == 4) {
                                ui->temp_4->setText(QString(" %1°C").arg(temp-276));
                                ui->time_4->setText(timeStr);
                            } else if (j == 5) {
                                ui->temp_8->setText(QString(" %1°C").arg(temp-270));
                                ui->time_8->setText(timeStr);
                            } else if (j == 6) {
                                ui->temp_1->setText(QString(" %1°C").arg(temp-271.7));
                                ui->time_first->setText(timeStr);
                            } else if (j == 7) {
                                ui->temp_2->setText(QString(" %1°C").arg(temp-273));
                                ui->time_2->setText(timeStr);
                            }
                        }
                        break;
                    }
                }




                // 3 saatlik bir zaman dilimi içinde miyiz?

                    // Bu bloktaki verileri işleyin.

                   // double temp = dataObj.value("main").toObject().value("temp").toDouble();
                  double Maxtemp = dataObj.value("main").toObject().value("temp_max").toDouble();
                    double Mintemp = dataObj.value("main").toObject().value("temp_min").toDouble();
                    double humidity = dataObj.value("main").toObject().value("humidity").toDouble();
                    double windSpeed = dataObj.value("wind").toObject().value("speed").toDouble();



                    // Hava sıcaklığı QLabel'ı oluşturuluyor
                 ui->currentTemperature->setText(QString("Sıcaklık: %1°C").arg(temp-273));

                    // Nem QLabel'ı oluşturuluyor

                    ui->currentWindHumidity->setText(QString("Nem: %1%").arg(humidity));

                    //max temp
                    ui->MaxTemp->setText(QString(" Max Sıcaklık: %1°C").arg(Maxtemp-273));
                    //min Temp
                      ui->MinTemp->setText(QString(" Min Sıcaklık: %1°C").arg(Mintemp-273));
                      //ruzgar hızı

                    ui->Ruzgarhizi->setText(QString("Rüzgar: %1 km/sa").arg(windSpeed));
                    updateWeatherData(dataObj);
                    QString currentDateTimeStr = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

                ui->CurrentTime->setText("Current Time :" +currentDateTimeStr);




                }



        }
        else {
            qDebug() << "Error occurred: " << reply->errorString();
        }

        // Bellekteki nesneleri silme
        reply->deleteLater();
        manager->deleteLater();
    });

}



