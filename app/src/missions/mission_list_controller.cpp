#include "mission_list_controller.h"

#include <QDebug>

#include "locator.h"

using namespace md::domain;
using namespace md::presentation;

MissionListController::MissionListController(QObject* parent) :
    QObject(parent),
    m_missions(md::app::Locator::get<IMissionsService>()),
    m_vehicles(md::app::Locator::get<IVehiclesService>())
{
    Q_ASSERT(m_missions);
    Q_ASSERT(m_vehicles);

    connect(m_missions, &IMissionsService::missionTypesChanged, this,
            &MissionListController::missionTypesChanged);
    connect(m_missions, &IMissionsService::missionAdded, this,
            &MissionListController::onMissionAdded);
    connect(m_missions, &IMissionsService::missionRemoved, this,
            &MissionListController::onMissionRemoved);

    for (Mission* mission : m_missions->missions())
    {
        this->onMissionAdded(mission);
    }
}

QVariantList MissionListController::missionTypes() const
{
    QVariantList list;
    for (const MissionType* type : m_missions->missionTypes())
    {
        list.append(type->toVariantMap());
    }

    return list;
}

QJsonArray MissionListController::missions() const
{
    QJsonArray missions;
    for (Mission* mission : m_missions->missions())
    {
        missions += QJsonObject::fromVariantMap(mission->toVariantMap());
    }
    return missions;
}

QJsonObject MissionListController::mission(const QVariant& missionId) const
{
    Mission* mission = m_missions->mission(missionId);
    if (mission)
        return QJsonObject::fromVariantMap(mission->toVariantMap());

    return QJsonObject();
}

QString MissionListController::vehicleName(const QVariant& vehicleId) const
{
    Vehicle* vehicle = m_vehicles->vehicle(vehicleId);
    if (vehicle)
        return vehicle->name;

    return QString();
}

void MissionListController::addMission(const QString& typeId)
{
    const MissionType* type = m_missions->missionType(typeId);
    if (!type)
        return;

    QStringList missionNames;
    for (Mission* mission : m_missions->missions())
    {
        missionNames += mission->name;
    }

    auto mission = new Mission(type, utils::nameFromType(type->name, missionNames));
    m_missions->saveMission(mission);
}

void MissionListController::remove(const QVariant& missionId)
{
    Mission* mission = m_missions->mission(missionId);
    if (!mission)
        return;

    m_missions->removeMission(mission);
}

void MissionListController::rename(const QVariant& missionId, const QString& name)
{
    Mission* mission = m_missions->mission(missionId);
    if (!mission)
        return;

    mission->name.set(name);
    m_missions->saveMission(mission);
}

void MissionListController::onMissionAdded(Mission* mission)
{
    connect(mission, &Mission::changed, this, [this, mission]() {
        emit missionChanged(mission->id, mission->toVariantMap());
    });
    emit missionsChanged();
}

void MissionListController::onMissionRemoved(Mission* mission)
{
    disconnect(mission, nullptr, this, nullptr);
    emit missionsChanged();
}
