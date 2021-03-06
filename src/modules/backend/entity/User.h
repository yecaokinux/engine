/**
 * @file
 */

#pragma once

#include "network/MessageSender.h"
#include "Entity.h"
#include "core/Var.h"
#include "backend/poi/PoiProvider.h"

namespace backend {

class User : public Entity {
private:
	std::string _name;
	std::string _password;
	std::string _email;
	uint32_t _host;
	voxel::WorldPtr _world;
	PoiProviderPtr _poiProvider;
	network::MoveDirection _moveMask = network::MoveDirection::NONE;
	float _yaw = 0.0f;
	uint64_t _lastAction = 0u;
	uint64_t _time = 0u;
	core::VarPtr _userTimeout;
	flatbuffers::FlatBufferBuilder _entityUpdateFbb;

	bool isMove(network::MoveDirection dir) const;
	void addMove(network::MoveDirection dir);
	void removeMove(network::MoveDirection dir);

protected:
	void visibleAdd(const EntitySet& entities) override;
	void visibleRemove(const EntitySet& entities) override;

public:
	User(ENetPeer* peer, EntityId id, const std::string& name, const network::MessageSenderPtr& messageSender, const voxel::WorldPtr& world,
			const core::TimeProviderPtr& timeProvider, const attrib::ContainerProviderPtr& containerProvider, const cooldown::CooldownProviderPtr& cooldownProvider,
			const PoiProviderPtr& poiProvider);

	void setEntityId(EntityId id);

	void setPassword(const std::string& password);

	void setEmail(const std::string& mail);

	const std::string& name() const;

	const std::string& password() const;

	const std::string& email() const;

	void changeMovement(network::MoveDirection bitmask, float pitch, float yaw);

	void attack(EntityId id);

	/**
	 * @brief The client closed the connection - the user object itself will stay in the server until
	 * a logout cooldown was hit
	 */
	void disconnect();

	void reconnect();

	bool update(long dt) override;

	/**
	 * @brief Sets a new ENetPeer and returns the old one.
	 */
	ENetPeer* setPeer(ENetPeer* peer);

	uint32_t host() const {
		return _host;
	}

	/**
	 * @brief Informs the user that the login was successful
	 */
	void sendUserSpawn() const;
	void sendSeed(long seed) const;
};

inline bool User::isMove(network::MoveDirection dir) const {
	return (_moveMask & dir) != network::MoveDirection::NONE;
}

inline void User::addMove(network::MoveDirection dir) {
	_moveMask |= dir;
}

inline void User::removeMove(network::MoveDirection dir) {
	_moveMask &= ~dir;
}

inline void User::setEntityId(EntityId id) {
	_entityId = id;
}

inline void User::setPassword(const std::string& pw) {
	_password = pw;
}

inline void User::setEmail(const std::string& mail) {
	_email = mail;
}

inline const std::string& User::name() const {
	return _name;
}

inline const std::string& User::password() const {
	return _password;
}

inline const std::string& User::email() const {
	return _email;
}

inline void User::changeMovement(network::MoveDirection bitmask, float pitch, float yaw) {
	_moveMask = bitmask;
	_orientation = pitch;
	_yaw = yaw;
}

typedef std::shared_ptr<User> UserPtr;

}
