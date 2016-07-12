namespace te
{
	template <typename T>
	SystemImpl<T>::SystemImpl()
		: m_Components{}
	{}

	template <typename T>
	void SystemImpl<T>::update(const sf::Time& dt)
	{
		for (auto& component : m_Components) component.update(dt);
	}

	template <typename T>
	template <typename... Args>
	void SystemImpl<T>::addComponent(Args&&... args)
	{
		m_Components.push_back(T(std::forward<Args>(args)...));
	}
}
