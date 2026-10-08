#include "totalcontroller.h"
#include <QCoreApplication>
TotalController::TotalController(QObject *parent)
    : QObject{parent}
{}

QString TotalController::DasePath(){
    return QCoreApplication::applicationDirPath() + "/../../data";
}
bool TotalController::loadRegions(const QString& path){
    if(!m_repo.loadSQLcountries(path.toStdString())) return false;
    return true;
}