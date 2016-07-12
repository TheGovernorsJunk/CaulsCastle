namespace te
{
	template <typename T>
	void UpdatableSystem<T>::update(const sf::Time& dt)
	{
		for (auto& component : m_Components) component.update(dt);
	}
}
