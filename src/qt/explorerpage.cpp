//ROOTCOIN integrated blockexplorer

#include "explorerpage.h"
#include "ui_explorerpage.h"

#include "clientmodel.h"
#include "walletmodel.h"
#include "bitcoinunits.h"
#include "optionsmodel.h"
#include "transactiontablemodel.h"
#include "transactionfilterproxy.h"
#include "guiutil.h"
#include "guiconstants.h"

#include <QAbstractItemDelegate>
#include <QPainter>

#define DECORATION_SIZE 64

class ExplorerViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    ExplorerViewDelegate(): QAbstractItemDelegate(), unit(BitcoinUnits::BTC) { }

    inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index ) const
    {
        painter->save();

        QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        QRect mainRect = option.rect;
        QRect decorationRect(mainRect.topLeft(), QSize(DECORATION_SIZE, DECORATION_SIZE));
        icon.paint(painter, decorationRect);

        QColor foreground = option.palette.color(QPalette::Text);
        painter->setPen(foreground);
        painter->setPen(foreground);
        painter->setPen(option.palette.color(QPalette::Text));

        painter->restore();
    }

    inline QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        return QSize(DECORATION_SIZE, DECORATION_SIZE);
    }

    int unit;

};
#include "explorerpage.moc"

ExplorerPage::ExplorerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerPage),
    clientModel(0),
    walletModel(0),
    explorerdelegate(new ExplorerViewDelegate())
{
    ui->setupUi(this);
    nam = new QNetworkAccessManager(this);
    ui->textBrowser->setHidden(true);
    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
    connect(ui->submitButton,SIGNAL(clicked()),this,SLOT(DoHttpGet()));
}

ExplorerPage::~ExplorerPage()
{
    delete ui;
}

void ExplorerPage::setModel(ClientModel *model)
{
    this->clientModel = model;

}

void ExplorerPage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
}

void ExplorerPage::finished(QNetworkReply *reply) {
  ui->textBrowser->setHidden(false);
  if(reply->error() == QNetworkReply::NoError) {
    ui->textBrowser->setText(reply->readAll());
  } else {
    ui->textBrowser->setText(reply->errorString());
  }
}

void ExplorerPage::DoHttpGet() {
  QString url1 = "http://root.explorer.ssdpool.com:9083/tx/";
  QString url2 = "http://root.explorer.ssdpool.com:9083/api/getblockhash?index=";
  QString url3 = "http://root.explorer.ssdpool.com:9083/address/";
//  QString end = ".htm";
  QString final;
  QString data = ui->dataLine->text();
  int size = data.size();
  
if(size == 34)
{
final = url3 + data;
}
else if(size == 64)
{
final = url1 + data;
}
else if(size == 68)
{
QString datax = data.left(64);
final = url1 + datax;
}
else {
final = url2 + data;
}
  QByteArray postData;
  postData.append(data.toAscii());
  nam->get(QNetworkRequest(QUrl(final)));
}
