namespace te
{
	template <typename Resource>
	template <typename... Args>
	void ResourceManager<Resource>::load(const std::string& id, const std::string& filename, Args&&... args)
	{
		auto pResource = std::make_unique<Resource>(std::forward<Args>(args)...);
		if (!pResource->loadFromFile(filename))
			throw std::runtime_error{"ResourceManager::load: failed to load " + filename};

		insertResource(id, std::move(pResource));
	}

	template <typename Resource>
	Resource& ResourceManager<Resource>::get(const std::string& id)
	{
		auto found = m_ResourceMap.find(id);
		assert(found != m_ResourceMap.end());
		return *found->second;
	}

	template <typename Resource>
	const Resource& ResourceManager<Resource>::get(const std::string& id) const
	{
		auto found = m_ResourceMap.find(id);
		assert(found != m_ResourceMap.end());
		return *found->second;
	}

	template <typename Resource>
	void ResourceManager<Resource>::insertResource(const std::string& id, std::unique_ptr<Resource>&& pResource)
	{
		auto inserted = m_ResourceMap.insert(std::make_pair(id, std::move(pResource)));
		assert(inserted.second);
	}
}
