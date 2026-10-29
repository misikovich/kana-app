#include "GestionLangue.h"
#include <QCoreApplication>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

static QTranslator *traducteur = nullptr;
static QString langue_actuelle = "fr";

QStringList languesDisponibles()
{
    return {"fr", "en", "ru"};
}

QString nomLangue(QString code)
{
    //non traduit : chaque langue reste lisible par ceux qui la parlent
    if (code == "en")
        return "English";
    if (code == "ru")
        return "Русский";
    return "Français";
}

QString langueInitiale()
{
    QSettings settings;
    QString code = settings.value("langue").toString();
    if (languesDisponibles().contains(code))
        return code;

    //premier lancement : on essaie la langue du système
    for (const QString &systeme : QLocale::system().uiLanguages())
    {
        code = systeme.left(2);
        if (languesDisponibles().contains(code))
            return code;
    }
    return "fr";
}

void appliqueLangue(QString code)
{
    if (!languesDisponibles().contains(code))
        code = "fr";

    if (!traducteur)
        traducteur = new QTranslator(qApp);
    QCoreApplication::removeTranslator(traducteur);

    //les fichiers .qm sont compilés depuis traductions/*.ts et intégrés dans les ressources
    if (code != "fr" && traducteur->load(":/i18n/kanapp_"+code+".qm"))
        QCoreApplication::installTranslator(traducteur);

    langue_actuelle = code;
    QSettings().setValue("langue", code);
}

QString langueActuelle()
{
    return langue_actuelle;
}
