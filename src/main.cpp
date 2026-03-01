#include <Geode/Geode.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class $modify(CommentCopyCell, CommentCell) {

	void loadFromComment(GJComment* comment) {
		CommentCell::loadFromComment(comment);
		if (!this->m_comment->m_isSpam && !this->getChildByID("copy-menu"_spr)) {
            addCopyButton();
        }
	}

	void onUnhide(CCObject* sender) {
		CommentCell::onUnhide(sender);
		if (!this->getChildByID("copy-menu"_spr)) {
            addCopyButton();
        }
	}

	void addCopyButton() {
        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        this->addChild(menu, 1); 

        
        auto sprite = ButtonSprite::create("Copy");
        sprite->setScale(0.5f);

        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(CommentCopyCell::onCopy)
        );

        auto size = this->getContentSize();
        btn->setPosition({
            242.f,
            60.f
        });

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

		FLAlertLayer::create(
			"Copied!",
			"Comment copied to clipboard.",
			"OK"
		)->show();
	}
};