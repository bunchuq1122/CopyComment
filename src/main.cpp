#include <Geode/Geode.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/general.hpp>

using namespace geode::prelude;

class $modify(CommentCopyCell, CommentCell) {

	void loadFromComment(GJComment* comment) {
		CommentCell::loadFromComment(comment);
        auto id = Mod::get()->getID() + "/copy-comment";
		if (this->m_comment && !this->m_comment->m_isSpam && !this->getChildByID(id)) {
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
        if (this->getChildByID(Mod::get()->getID() + "/copy-comment")) return;
        
        auto sprite = CCSprite::createWithSpriteFrameName("GJ_copyBtn_001.png");
        if (!sprite) return;

        sprite->setScale(0.5f);

        CCPoint pos = {240.f, 60.f};
        if(this->m_compactMode) {
            pos.setPoint(240.f,28.f);
        }
        if (this->m_comment && this->m_comment->m_hasLevelID) {
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
        btn->setPosition(pos);

        auto menu = CCMenu::create();
        menu->setID(Mod::get()->getID() + "/copy-comment");
        menu->setPosition({0, 0});
        this->addChild(menu, 1); 

        menu->addChild(btn);
    }


    void onCopy(CCObject*) {
		if (!m_comment) return;

		std::string text = m_comment->m_commentString;

		if (!Mod::get()->getSettingValue<bool>("copyusername")) {
			if (!geode::utils::clipboard::write(text)) {
                FLAlertLayer::create("Error!", "comment not found or failed to load geode utils", "ok")->show();
            }
		}else {
			if (!geode::utils::clipboard::write("@" + m_comment->m_userName + ": " + text)) {
                FLAlertLayer::create("Error!", "comment not found or failed to load geode utils", "ok")->show();
            }
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