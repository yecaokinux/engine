/**
 * @file
 */

#include "voxel/polyvox/AStarPathfinder.h"
#include "Npc.h"
#include "EntityStorage.h"
#include "backend/poi/PoiProvider.h"
#include "voxel/World.h"

namespace backend {

std::atomic<EntityId> Npc::_nextNpcId(5000000);

Npc::Npc(network::EntityType type, const EntityStoragePtr& entityStorage, const ai::TreeNodePtr& behaviour, const voxel::WorldPtr& world, const network::MessageSenderPtr& messageSender,
		const core::TimeProviderPtr& timeProvider, const attrib::ContainerProviderPtr& containerProvider, const cooldown::CooldownProviderPtr& cooldownProvider, const PoiProviderPtr& poiProvider) :
		Entity(_nextNpcId++, messageSender, timeProvider, containerProvider, cooldownProvider), _world(world), _poiProvider(poiProvider) {
	_entityType = type;
	_ai = std::make_shared<ai::AI>(behaviour);
	_ai->setCharacter(std::make_shared<AICharacter>(_entityId, *this));
}

Npc::~Npc() {
	ai::Zone* zone = _ai->getZone();
	if (zone == nullptr) {
		return;
	}
	zone->destroyAI(id());
	_ai->setZone(nullptr);
}

void Npc::init() {
	Entity::init();
}

void Npc::init(const glm::ivec3* pos) {
	const glm::ivec3& randomPos = pos ? *pos : _world->randomPos();
	const voxel::VoxelType material = _world->getMaterial(randomPos.x, randomPos.y, randomPos.z);
	Log::info("spawn character %i with behaviour tree %s at position %i:%i:%i (material: %i)",
			ai()->getId(), ai()->getBehaviour()->getName().c_str(),
			randomPos.x, randomPos.y, randomPos.z, std::enum_value(material));
	setHomePosition(randomPos);
	_ai->getCharacter()->setPosition(glm::vec3(randomPos.x, randomPos.y, randomPos.z));
	init();
	_ai->getAggroMgr().setReduceByValue(0.1f);
}

std::string Npc::name() const {
	return network::EnumNameEntityType(_entityType);
}

void Npc::setPointOfInterest() {
	_poiProvider->addPointOfInterest(pos());
}

double Npc::applyDamage(Npc* attacker, double damage) {
	double health = _attribs.current(attrib::Type::HEALTH);
	if (health > 0.0) {
		health = std::max(0.0, health - damage);
		if (attacker != nullptr) {
			_ai->getAggroMgr().addAggro(attacker->id(), damage);
		}
		_attribs.setCurrent(attrib::Type::HEALTH, health);
		return damage;
	}
	return 0.0;
}

bool Npc::die() {
	return applyDamage(nullptr, _attribs.current(attrib::Type::HEALTH)) > 0.0;
}

bool Npc::attack(ai::CharacterId id) {
	const double strength = _attribs.current(attrib::Type::STRENGTH);
	if (strength <= 0.0) {
		return false;
	}
	return _ai->getZone()->executeAsync(id, [=] (const ai::AIPtr & targetAi) {
		AICharacter& targetChr = ai::character_cast<AICharacter>(targetAi->getCharacter());
		targetChr.getNpc().applyDamage(this, strength);
	});
}

bool Npc::update(long dt) {
	if (!Entity::update(dt)) {
		return false;
	}
	ai::ICharacterPtr character = _ai->getCharacter();
	character->setSpeed(_attribs.current(attrib::Type::SPEED));
	character->setOrientation(orientation());
	return !dead();
}

bool Npc::route(const glm::ivec3& target) {
	std::list<glm::ivec3> result;
	const glm::vec3& pos = _ai->getCharacter()->getPosition();
	const glm::ivec3 start(pos);
	const glm::ivec3 end(target.x, target.y, target.z);
	// TODO: use the route
	return _world->findPath(start, end, result);
}

void Npc::moveToGround() {
	glm::vec3 pos = _ai->getCharacter()->getPosition();
	pos.y = _world->findFloor(pos.x, pos.z, voxel::isFloor);
	_ai->getCharacter()->setPosition(pos);
}

}
