#ifndef MISSION_CONTROLLER_H
#define MISSION_CONTROLLER_H

#include "i_missions_service.h"

namespace md::presentation
{
class MissionController
    : public QObject // TODO: MissionRouteController & MissionController with sync
{
    Q_OBJECT

    Q_PROPERTY(QVariant vehicleId READ vehicleId WRITE setVehicleId NOTIFY missionChanged)
    Q_PROPERTY(QJsonObject mission READ mission NOTIFY missionChanged)
    Q_PROPERTY(QJsonObject home READ home NOTIFY homeChanged)
    Q_PROPERTY(QJsonObject operation READ operation NOTIFY operationChanged)
    Q_PROPERTY(QStringList items READ items NOTIFY itemsChanged)
    Q_PROPERTY(int currentItem READ currentItem NOTIFY currentItemChanged)

public:
    explicit MissionController(QObject* parent = nullptr);

    QVariant vehicleId() const;
    QJsonObject mission() const;
    QJsonObject home() const;
    QJsonObject operation() const;
    QStringList items() const;
    int currentItem() const;

public slots:
    void setVehicleId(const QVariant& vehicleId);
    void setMission(domain::Mission* mission);

    void save(const QJsonObject& data);
    void remove();
    void upload();
    void download();
    void clear();
    void cancel();
    void switchItem(int index);

signals:
    void missionChanged();
    void operationChanged();
    void itemsChanged();
    void currentItemChanged();
    void homeChanged(QJsonObject home);

private:
    void setOperation(domain::MissionOperation* operation);

    domain::IMissionsService* const m_missionsService;
    domain::Mission* m_mission = nullptr;
    domain::MissionOperation* m_operation = nullptr;
};
} // namespace md::presentation

#endif // MISSION_CONTROLLER_H
