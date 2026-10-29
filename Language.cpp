#include "Language.h"
#include <QCoreApplication>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

static QTranslator *translator = nullptr;
static QString current_language = "en";

QStringList availableLanguages()
{
    return {"en", "fr", "ru"};
}

QString languageName(QString code)
{
    //not translated: each language stays readable to those who speak it
    if (code == "fr")
        return "Français";
    if (code == "ru")
        return "Русский";
    return "English";
}

QString initialLanguage()
{
    QSettings settings;
    QString code = settings.value("language").toString();
    if (availableLanguages().contains(code))
        return code;

    //first launch: we try the system language
    for (const QString &system_locale : QLocale::system().uiLanguages())
    {
        code = system_locale.left(2);
        if (availableLanguages().contains(code))
            return code;
    }
    return "en";
}

void applyLanguage(QString code)
{
    if (!availableLanguages().contains(code))
        code = "en";

    if (!translator)
        translator = new QTranslator(qApp);
    QCoreApplication::removeTranslator(translator);

    //the .qm files are compiled from translations/*.ts and embedded in the resources
    if (code != "en" && translator->load(":/i18n/kanapp_"+code+".qm"))
        QCoreApplication::installTranslator(translator);

    current_language = code;
    QSettings().setValue("language", code);
}

QString currentLanguage()
{
    return current_language;
}
