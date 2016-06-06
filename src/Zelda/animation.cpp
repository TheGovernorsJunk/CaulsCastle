#include "animation.h"
#include "texture_manager.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

namespace te
{
	class Animation::FixedIntervalAnimation : public Animation
	{
	public:
		FixedIntervalAnimation(TextureID name, int millisecondsPerClip, std::vector<Clip>&& clips)
			: Animation{ name, std::move(clips) }
			, mMillisecondsPerClip(millisecondsPerClip)
		{}

		int getMillisecondsPerClip() const
		{
			return mMillisecondsPerClip;
		}

	private:
		int mMillisecondsPerClip;
	};

	std::unique_ptr<Animation> Animation::make(TextureID animationID, int millisecondsPerClip, std::vector<Clip>&& clips)
	{
		return std::unique_ptr<Animation>{new FixedIntervalAnimation{ animationID, millisecondsPerClip, std::move(clips) }};
	}

	std::vector<std::unique_ptr<Animation>> Animation::load(const std::string& filename, TextureManager& textureManager)
	{
		rapidxml::file<> animFile(filename.c_str());
		rapidxml::xml_document<> animXML;
		animXML.parse<0>(animFile.data());
		rapidxml::xml_node<char>* pListNode = animXML.first_node("animations");

		std::vector<std::unique_ptr<Animation>> animations;
		for (auto pAnimNode = pListNode->first_node("animation"); pAnimNode != 0; pAnimNode = pAnimNode->next_sibling("animation"))
		{
			TextureID name = TextureManager::getID(pAnimNode->first_attribute("name")->value());
			int mpc = std::stoi(pAnimNode->first_attribute("millisecondsPerClip")->value());

			std::vector<Clip> clips;
			for (auto pClipNode = pAnimNode->first_node("clip"); pClipNode != 0; pClipNode = pClipNode->next_sibling("clip"))
			{
				//std::string textureStr = dir + pClipNode->first_attribute("texture")->value();
				//textureManager.load(textureStr);
				TextureID id = TextureManager::getID(pClipNode->first_attribute("sprite")->value());
				clips.push_back({
					//TextureManager::getID(textureStr),
					id,
					textureManager.getSprite(id)
				});
			}

			animations.push_back(make(name, mpc, std::move(clips)));
		}

		return animations;
	}

	TextureID Animation::getID() const
	{
		return mName;
	}

	const sf::Sprite& Animation::getSprite(size_t index) const
	{
		size_t i = index % mClips.size();
		return mClips[i].sprite;
	}

	sf::Time Animation::getDuration() const
	{
		return sf::milliseconds(getMillisecondsPerClip() * mClips.size());
	}

	Animation::Animation(TextureID name, std::vector<Clip>&& clips)
		: mName(name)
		, mClips(std::move(clips))
	{}
}
