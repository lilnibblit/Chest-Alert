#include <Geode/Geode.hpp>
using namespace geode::prelude;

$on_mod(Loaded) {
	// Getting chest data so user doesnt have to open the chest menu
	GameLevelManager* glm = GameLevelManager::sharedState();
	glm->getGJRewards(0);
}

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto mark = CCSprite::createWithSpriteFrameName("exMark_001.png");
		mark->setPosition({40, 35});
		mark->setScale(0.55f);
		mark->setVisible(false);

		if (GameStatsManager::sharedState()->m_rewardItems->count() == 0) {
			// If no chest data found
			GameLevelManager* glm = GameLevelManager::sharedState();
			glm->getGJRewards(0);
		}

		GameStatsManager* gsm;

		gsm = GameStatsManager::sharedState();

		int time1_value = 0x1;
		int time2_value = 0x2;
		GJRewardItem* time1 = typeinfo_cast<GJRewardItem*>(gsm->m_rewardItems->objectForKey(time1_value));
		GJRewardItem* time2 = typeinfo_cast<GJRewardItem*>(gsm->m_rewardItems->objectForKey(time2_value));

		if (time1 != nullptr || time2 != nullptr) {
			if (time1->m_timeRemaining == 0 || time2->m_timeRemaining == 0) {
				mark->setVisible(true);
			}
		}

		auto menu = this->getChildByIDRecursive("daily-chest-button");
		menu->addChild(mark);

		mark->setID("mark"_spr);
		
		return true;
	}
};

#include <Geode/modify/RewardsPage.hpp>
class $modify(MyRewardsPage, RewardsPage) {
	void rewardsStatusFinished(int a1) {
		//kostыli for hiding the mark after opening everything
		if (this->m_leftOpen == false || this->m_rightOpen == false) {
			auto scene = CCDirector::sharedDirector()->getRunningScene();
			auto dcb = scene->getChildByIDRecursive("daily-chest-button");
			if (dcb) dcb->getChildByID("mark"_spr)->setVisible(false);
		}

		RewardsPage::rewardsStatusFinished(a1);
	}
};
