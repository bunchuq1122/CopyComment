#include <Geode/Geode.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class $modify(CommentCopyCell, CommentCell) {

	void loadFromComment(GJComment* comment) {
		CommentCell::loadFromComment(comment);
        auto id = Mod::get()->getID() + "/copy-comment";
		if (!this->m_comment->m_isSpam && !this->getChildByID(id)) {
            addCopyButton();
        }
	}

	void onUnhide(CCObject* sender) {
		CommentCell::onUnhide(sender);
        auto id = Mod::get()->getID() + "/copy-comment";
		if (!this->getChildByID(id)) {
            addCopyButton();
        }
	}

	void addCopyButton() {
        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        this->addChild(menu, 1); 
        
        auto sprite = CCSprite::createWithSpriteFrameName("GJ_copyBtn_001.png");
        sprite->setScale(0.5f);

        CCPoint pos = {240.f, 60.f};
        if(this->m_compactMode) {
            pos.setPoint(240.f,28.f);
        }
        if (this->m_comment->m_hasLevelID) {
            pos.setPoint(160.f,60.f);
        }
        if(this->m_accountComment) {
            pos.setPoint(230.f, 70.f);
        }

        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(CommentCopyCell::onCopy)
        );
        btn->setID(Mod::get()->getID() + "/copy-comment");
        btn->setPosition(pos);

        menu->addChild(btn);
    }


    void onCopy(CCObject*) {
		if (!m_comment) return;

		std::string text = m_comment->m_commentString;

		if (!Mod::get()->getSettingValue<bool>("copyusername")) {
			geode::utils::clipboard::write(text);
		}else {
			geode::utils::clipboard::write("@" + m_comment->m_userName + ": " + text);
		}

        if(!Mod::get()->getSettingValue<bool>("popup")) return;

		FLAlertLayer::create(
			"Copied!",
			"Comment copied to clipboard.",
			"OK"
		)->show();
        return;
	}
};