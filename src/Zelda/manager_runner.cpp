#include "manager_runner.h"
#include "input_manager.h"
#include "player_manager.h"
#include "physics_world_manager.h"
#include "velocity_manager.h"
#include "render_manager.h"
#include "draw_manager.h"

namespace te
{
	struct ManagerRunner::Impl
	{
		InputManager inputManager;
		PlayerManager playerManager;
		PhysicsWorldManager physicsManager;
		VelocityManager velocityManager;
		RenderManager<decltype(GameData::sprites)> spriteRenderManager;
		RenderManager<decltype(GameData::mapLayers)> layerRenderManager;
		DrawManager drawManager;

		Impl(GameData& data)
			: inputManager(data.directionInput)
			, playerManager(0, data.directionInput, data.velocities)
			, physicsManager(data.physicsWorld, data.rigidBodies, data.positions)
			, velocityManager(data.velocities, data.rigidBodies, data.positions)
			, spriteRenderManager(data.sprites, data.positions, data.sortingLayers, data.pendingDraws)
			, layerRenderManager(data.mapLayers, data.positions, data.sortingLayers, data.pendingDraws)
			, drawManager(data.pixelToWorldScale, data.pendingDraws, *data.pWindow)
		{}
	};

	ManagerRunner::ManagerRunner(GameData& data)
		: m_pImpl(std::make_unique<Impl>(data))
	{}

	ManagerRunner::ManagerRunner(ManagerRunner&&) noexcept = default;
	ManagerRunner& ManagerRunner::operator=(ManagerRunner&&) noexcept = default;
	ManagerRunner::~ManagerRunner() = default;

	void ManagerRunner::fixedUpdate(const sf::Time& dt)
	{
		m_pImpl->inputManager.update();
		m_pImpl->playerManager.update();
		m_pImpl->velocityManager.update(dt);
		m_pImpl->physicsManager.update(dt);
	}

	void ManagerRunner::renderUpdate()
	{
		m_pImpl->spriteRenderManager.update();
		m_pImpl->layerRenderManager.update();
		m_pImpl->drawManager.update();
	}
}
