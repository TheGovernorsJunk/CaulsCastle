#include "animation.h"
#include "texture_manager.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

namespace te
{
	class Animation::FixedIntervalAnimation : public Animation
	{
	public:
		FixedIntervalAnimation(const std::string& name, int millisecondsPerClip, std::vector<Clip>&& clips)
			: Animation{name}
			, mTimePerClip{sf::milliseconds(millisecondsPerClip)}
			, mClips{std::move(clips)}
		{}

		sf::Time getDuration() const
		{
			return sf::milliseconds(mTimePerClip.asMilliseconds() * mClips.size());
		}

		const sf::Sprite& getSprite(const sf::Time& dt) const
		{
			size_t index = (size_t)(dt / mTimePerClip) % mClips.size();
			return mClips[index].sprite;
		}

	private:
		sf::Time mTimePerClip;
		std::vector<Clip> mClips;
	};

	class Animation::VariableIntervalAnimation : public Animation
	{
	public:
		VariableIntervalAnimation(const TextureAtlas::Animation& animationData, const TextureManager& textureManager)
			: Animation(animationData.id)
		{
			const std::vector<TextureAtlas::Clip>& clips = animationData.clips;
			const TextureAtlas::Clip* lastClip = clips.size() > 0 ? &clips[0] : nullptr;
			for (auto& clip : clips)
			{
				for (int nextIndex = lastClip->index + 1; nextIndex < clip.index; ++nextIndex)
				{
					//mClipSprites.push_back(textureManager.getSprite(lastClip->spriteID));
				}
				//mClipSprites.push_back(textureManager.getSprite(clip.spriteID));
				lastClip = &clip;
			}
		}

		sf::Time getDuration() const
		{
			return sf::milliseconds(mClipSprites.size() * MillisecondsPerClip);
		}

		const sf::Sprite& getSprite(const sf::Time& dt) const
		{
			size_t index = (size_t)(dt / sf::milliseconds(MillisecondsPerClip)) % mClipSprites.size();
			return mClipSprites[index];
		}

	private:
		const static float MillisecondsPerClip;
		std::vector<sf::Sprite> mClipSprites;
	};

	const float Animation::VariableIntervalAnimation::MillisecondsPerClip = 1000.f / 30.f;

	std::unique_ptr<Animation> Animation::make(const std::string& animationID, int millisecondsPerClip, std::vector<Clip>&& clips)
	{
		return std::unique_ptr<Animation>{new FixedIntervalAnimation{animationID, millisecondsPerClip, std::move(clips)}};
	}

	std::unique_ptr<Animation> Animation::make(const TextureAtlas::Animation& animationData, const TextureManager& textureManager)
	{
		return std::unique_ptr<Animation>{new VariableIntervalAnimation{animationData, textureManager}};
	}

	//std::vector<std::unique_ptr<Animation>> Animation::load(const std::string& filename, TextureManager& textureManager)
	//{
	//	rapidxml::file<> animFile(filename.c_str());
	//	rapidxml::xml_document<> animXML;
	//	animXML.parse<0>(animFile.data());
	//	rapidxml::xml_node<char>* pListNode = animXML.first_node("animations");

	//	std::vector<std::unique_ptr<Animation>> animations;
	//	for (auto pAnimNode = pListNode->first_node("animation"); pAnimNode != 0; pAnimNode = pAnimNode->next_sibling("animation"))
	//	{
	//		TextureID name = TextureManager::getID(pAnimNode->first_attribute("name")->value());
	//		int mpc = std::stoi(pAnimNode->first_attribute("millisecondsPerClip")->value());

	//		std::vector<Clip> clips;
	//		for (auto pClipNode = pAnimNode->first_node("clip"); pClipNode != 0; pClipNode = pClipNode->next_sibling("clip"))
	//		{
	//			//std::string textureStr = dir + pClipNode->first_attribute("texture")->value();
	//			//textureManager.load(textureStr);
	//			TextureID id = TextureManager::getID(pClipNode->first_attribute("sprite")->value());
	//			clips.push_back({
	//				//TextureManager::getID(textureStr),
	//				id,
	//				textureManager.getSprite(id)
	//			});
	//		}

	//		animations.push_back(make(name, mpc, std::move(clips)));
	//	}

	//	return animations;
	//}

	const std::string& Animation::getID() const
	{
		return mName;
	}

	Animation::Animation(const std::string& name)
		: mName(name)
	{}
}
