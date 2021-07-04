#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_

#include <QObject>
#include <QFont>

class AppConfig : public QObject
{
    Q_OBJECT

    QFont m_editorFont;
    bool m_isAutoWrap;
    bool m_isToolBarVisible;
    bool m_isStatusBarVisible;
    bool m_isValid;

    bool restore();
public:
    explicit AppConfig(QObject *parent = nullptr);
    explicit AppConfig(QFont font, bool isWrap, bool tbVisible, bool sbVisible, QObject *parent = nullptr);
    bool store();
    QFont editorFont();
    bool isAutoWrap();
    bool isToolBarVisible();
    bool isStatusBarVisible();
    operator bool();
};

#endif // _APPCONFIG_H_
