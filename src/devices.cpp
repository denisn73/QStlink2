/*
This file is part of QSTLink2.

    QSTLink2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QSTLink2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QSTLink2.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "devices.h"

DeviceInfo::DeviceInfo(QObject *parent) :
    QObject(parent)
{
    mType = "UNKNOWN";
    mLoaderFile = "UNKNOWN";
}

DeviceInfo::DeviceInfo(const DeviceInfo *device)
{
    mType = device->mType;
    mMap = device->mMap;
}

DeviceInfoList::DeviceInfoList(QObject *parent) :
    QObject(parent)
{
    mLoaded = false;
    qDebug("Loading device list.");
    mDoc = new QDomDocument("stlink");

    /* Try local path first */
    QFile file("devices.xml");

#ifndef WIN32
    /* Try default directory on Unix */
    if (!file.open(QIODevice::ReadOnly)) {
        file.setFileName("/usr/share/qstlink2/devices.xml");
    }
#endif

    /* Load from resource */
    if (!file.open(QIODevice::ReadOnly)) {
        qInfo("Could not open the devices.xml file. Using internal data.");

        file.setFileName(":/devices.xml");
    }
    if (!mDoc->setContent(&file)) {
        file.close();
        qCritical("Devices list failed to load.");
        return;
    }
    file.close();
    qInfo("Devices list loaded.");

    mDefaultDevice = new DeviceInfo(this);
    bool isInt;
    QDomElement docElem = mDoc->documentElement();
    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {

            if (e.tagName() == "devices_default") {
                QDomNodeList childs = e.childNodes();
                for (int i = 0;i < childs.count();i++) {
                    QDomElement el = childs.at(i).toElement();
                    qDebug("%s -> %s",  e.tagName().toStdString().c_str(), el.tagName().toStdString().c_str());
                    mDefaultDevice->insert(el.tagName(), el.text().toUInt(&isInt, 16));
                    if (!isInt)
                        qCritical("%s: Failed to parse number!", el.tagName().toStdString().c_str());
                }
            }
            else if (e.tagName() == "regs_default") {
                QDomNodeList regs = e.childNodes();
                for (int a = 0;a < regs.count(); a++) {
                    QDomElement el = regs.at(a).toElement();
                    qDebug("%s -> %X",  el.tagName().toStdString().c_str(), el.text().toUInt(0, 16));

                    mDefaultDevice->insert(el.tagName(), el.text().toUInt(&isInt, 16));
                    if (!isInt)
                        qCritical("%s: Failed to parse number!", el.tagName().toStdString().c_str());
                }
            }
        }
        n = n.nextSibling();
    }

    n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {

            if (e.tagName() == "devices") {
                QDomNodeList devices = e.childNodes();
                for (int a = 0;a < devices.count(); a++) {
                    mDevices.append(new DeviceInfo(mDefaultDevice)); // Copy from the default device.


                    QDomElement device = devices.at(a).toElement();
                    mDevices.last()->mType = device.attribute("type");

                    QDomNodeList childs = device.childNodes();
                    for (int i = 0;i < childs.count();i++) {
                        QDomElement el = childs.at(i).toElement();
                        qDebug("%s -> %s -> %s",
                               device.tagName().toStdString().c_str(),
                               device.attribute("type").toStdString().c_str(),
                               el.tagName().toStdString().c_str());
                        mDevices.last()->insert(el.tagName(), el.text().toUInt(&isInt, 16));
                        if (!isInt && el.tagName() != "loader")
                            qCritical("%s: Failed to parse number!", el.tagName().toStdString().c_str());

                        if (el.tagName() == "loader") {
                            mDevices.last()->mLoaderFile = el.text();
                        }
                    }
                }
            }

        }
        n = n.nextSibling();
    }

    mLoaded = true;
    return;
}

bool DeviceInfoList::IsLoaded() const {

    return mLoaded;
}

//if (mDevices->search(mStlink->mChipId))
bool DeviceInfoList::search(const quint32 chip_id) {
    qDebug("Looking for: 0x%03X", chip_id);
    for (int i=0; i < mDevices.count(); i++) {
        if (mDevices.at(i)->value("chip_id") == chip_id) {
            mCurDevice = mDevices.at(i);
            qDebug("Found chipID");
            //qDebug() << mCurDevice->repr();
            return true;
        }
    }
    qCritical("Did not find chipID!");
    return false;
}

quint16 DeviceInfoList::getDevicesCount() const {

    return mDevices.count();
}

QString DeviceInfo::repr(void) const {

    QMapIterator<QString, quint32> i(mMap);
    QString tmp("\r\n");
    tmp.append("Type: "+mType);
    while (i.hasNext()) {
        i.next();
        tmp.append("\r\n" + QString(i.key()) + ": 0x" + QString::number(i.value(), 16));
    }
    tmp.append("\r\nLoader: " + mLoaderFile);
    return tmp;
}

