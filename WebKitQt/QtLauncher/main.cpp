/*
 * Copyright (C) 2006 George Staikos <staikos@kde.org>
 * Copyright (C) 2006 Dirk Mueller <mueller@kde.org>
 * Copyright (C) 2006 Zack Rusin <zack@kde.org>
 * Copyright (C) 2006 Simon Hausmann <hausmann@kde.org>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <qwebpage.h>
#include <qwebframe.h>

#include <QtGui>
#include <QDebug>



class InfoWidget :public QProgressBar {
    Q_OBJECT
public:
    InfoWidget(QWidget *parent)
        : QProgressBar(parent), m_progress(0)
    {
        setMinimum(0);
        setMaximum(100);
    }
    QSize sizeHint() const
    {
        QSize size(100, 20);
        return size;
    }
public slots:
    void startLoad()
    {
        setValue(int(m_progress*100));
        show();
    }
    void changeLoad(double change)
    {
        m_progress = change;
        setValue(int(change*100));
    }
    void endLoad()
    {
        QTimer::singleShot(1000, this, SLOT(hide()));
        m_progress = 0;
    }

protected:
    qreal m_progress;
};

class SearchEdit;

class ClearButton : public QPushButton {
    Q_OBJECT
public:
    ClearButton(QWidget *w)
        : QPushButton(w)
    {
        setMinimumSize(24, 24);
        setFixedSize(24, 24);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        
    }
    void paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);
        QPainter painter(this);
        int height = parentWidget()->geometry().height();
        int width = height; //parentWidget()->geometry().width();
        
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::lightGray);
        painter.setBrush(isDown() ?
                         QColor(140, 140, 190) :
                         underMouse() ? QColor(220, 220, 255) : QColor(200, 200, 230)
            );
        painter.drawEllipse(4, 4, width - 8, height - 8);
        painter.setPen(Qt::white);
        int border = 8;
        painter.drawLine(border, border, width - border, height - border);
        painter.drawLine(border, height - border, width - border, border);
    }
};

class SearchEdit : public QLineEdit
{
    Q_OBJECT
public:
    SearchEdit(const QString &str, QWidget *parent = 0)
        : QLineEdit(str, parent)
    {
        setMinimumSize(QSize(300, 24));
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        
        setStyleSheet(":enabled { padding-right: 27px }");
        clearButton = new ClearButton(this);
        clearButton->setGeometry(QRect(geometry().right() - 27,
                                       geometry().top() - 2,
                                       geometry().right(), geometry().bottom()));
        clearButton->setVisible(true);
        clearButton->setCursor(Qt::ArrowCursor);
        connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));
        clearButton->setToolTip("Clear");
        
    }
    ~SearchEdit() { }
protected:
    virtual void paintEvent(QPaintEvent *e) {
        QLineEdit::paintEvent(e);
        if(text().isEmpty())
            clearButton->setVisible(false);
        else
            clearButton->setVisible(true);
    }
    virtual void resizeEvent(QResizeEvent *e) {
        clearButton->setParent(this);
        qDebug()<<"geometry = "<<geometry();
        clearButton->setGeometry(QRect(width()-27,
                                       0,
                                       24, 24));
    }
    virtual void moveEvent(QMoveEvent *) {
        clearButton->setParent(this);
        qDebug()<<"moveevent = "<<geometry();
        clearButton->setGeometry(QRect(width()-27, 1,
                                       24, 24));
    }
        
    QPushButton *clearButton;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(const QUrl &url)
    {
        page = new QWebPage(this);
        InfoWidget *info = new InfoWidget(page);
        info->setGeometry(20, 20, info->sizeHint().width(),
                          info->sizeHint().height());
        connect(page, SIGNAL(loadStarted(QWebFrame*)),
                info, SLOT(startLoad()));
        connect(page, SIGNAL(loadProgressChanged(double)),
                info, SLOT(changeLoad(double)));
        connect(page, SIGNAL(loadFinished(QWebFrame*)),
                info, SLOT(endLoad()));
        connect(page, SIGNAL(loadFinished(QWebFrame*)),
                this, SLOT(loadFinished()));
        connect(page, SIGNAL(titleChanged(const QString&)),
                this, SLOT(setWindowTitle(const QString&)));
    
        setCentralWidget(page);

        QToolBar *bar = addToolBar("Navigation");
        urlEdit = new SearchEdit(url.toString());
        connect(urlEdit, SIGNAL(returnPressed()),
                SLOT(changeLocation()));
        bar->addAction("Go back", page, SLOT(goBack()));
        bar->addAction("Stop", page, SLOT(stop()));
        bar->addAction("Go forward", page, SLOT(goForward()));
        bar->addWidget(urlEdit);
        

        page->open(url);

        info->raise();
    }
protected slots:
    void changeLocation()
    {
        QUrl url(urlEdit->text());
        page->open(url);
    }
    void loadFinished()
    {
        urlEdit->setText(page->url().toString());
    }
private:
    QWebPage *page;
    QLineEdit *urlEdit;
};

#include "main.moc"

int main(int argc, char **argv)
{
    QString url = QString("%1/%2").arg(QDir::homePath()).arg(QLatin1String("index.html"));
    QApplication app(argc, argv);

    const QStringList args = app.arguments();
    if (args.count() > 1)
        url = args.at(1);

    MainWindow window(url);
    window.show();

    return app.exec();
}
