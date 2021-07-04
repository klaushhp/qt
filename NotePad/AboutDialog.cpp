#include "AboutDialog.h"
#include <QPixmap>
#include <QPalette>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , m_logoLbl(this)
    , m_qqLbl(this)
    , m_infoEdit(this)
    , m_closeBtn(this)
{
    QPixmap pm(":/res/pic/dt.png");

    pm = pm.scaled(150, 150, Qt::KeepAspectRatio);

    m_logoLbl.setPixmap(pm);
    m_logoLbl.move(20, 20);
    m_logoLbl.resize(150, 150);

    QPalette p = m_infoEdit.palette();

    p.setColor(QPalette::Active, QPalette::Base, palette().color(QPalette::Active, QPalette::Background));
    p.setColor(QPalette::Inactive, QPalette::Base, palette().color(QPalette::Inactive, QPalette::Background));

    m_infoEdit.move(200, 30);
    m_infoEdit.resize(180, 130);
    m_infoEdit.setPalette(p);
    m_infoEdit.setFrameStyle(QFrame::NoFrame);
    m_infoEdit.setReadOnly(true);
    m_infoEdit.insertPlainText("NotePad Project\n\nPlatform: Qt 4.7.4\n\nVersion: 1.0.0\n\nCopyright: D.T.Software");

    m_qqLbl.move(22, 184);
    m_qqLbl.setText("QQ Group：847852369");

    m_closeBtn.setText("close");
    m_closeBtn.move(282, 180);
    m_closeBtn.resize(100, 30);

    setFixedSize(390, 230);
    setWindowTitle("About NotePad");

    connect(&m_closeBtn, &QPushButton::clicked, this, &QDialog::close);
}
