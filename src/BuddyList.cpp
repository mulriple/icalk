/*
 * =====================================================================================
 *
 *       Filename:  BuddyList.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2007年06月18日 星期一 10时28分37秒 
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */
#include <glib/gi18n.h>
#include <gtkmm/dialog.h>
#include "BuddyList.h"
#include "Buddy.h"
#include "Bodies.h"
#include "TalkCard.h"

Buddy* BuddyList::find_buddy(const std::string& id) const
{
	BUDDY_MAP::const_iterator iter = buddy_map.find(id); 
	if ( iter == buddy_map.end())
		return NULL;
	return (*iter).second;

}

void BuddyList::handleItemSubscribed(const JID & jid)
{
	printf( "subscribed %s\n", jid.bare().c_str() );
}
void BuddyList::handleItemAdded(const JID & jid)
{
	std::string jid_str = jid.bare().c_str();
	if( NULL == find_buddy(jid_str) )
	{
		/** 将新的好友加入到BuddyMap里。*/
		RosterItem* item = Bodies::Get_Bodies().get_client().rosterManager()->getRosterItem(jid_str);
		buddy_map.insert(buddy_map.end(), BUDDY_MAP::value_type(item->jid(), new Buddy(*item)));
		Bodies::Get_Bodies().get_cardManage().fetch_vcard(jid_str);
		Bodies::Get_Bodies().get_main_window().get_buddy_view().add(jid_str);
	}
}
void BuddyList::handleItemUnsubscribed(const JID & jid)
{
	printf( "ItemUnsubscribed  %s\n", jid.bare().c_str() );
}
void BuddyList::handleItemRemoved(const JID & jid)
{
	printf( "ItemRemoved   %s\n", jid.bare().c_str() );
	std::string jid_str = jid.bare();
	Bodies::Get_Bodies().get_main_window().get_buddy_view().remove(jid_str);
	BUDDY_MAP::iterator iter = buddy_map.find(jid_str);
	if( iter != buddy_map.end() )
		buddy_map.erase(iter);

}
void BuddyList::handleItemUpdated(const JID & jid)
{
	printf( "ItemUnpdated %s\n", jid.bare().c_str() );
}
void BuddyList::handleRoster(const Roster & roster)
{
	Roster::const_iterator it = roster.begin();
	for (; it != roster.end(); ++it) {

		const RosterItem* item = (*it).second;
		buddy_map.insert(buddy_map.end(), BUDDY_MAP::value_type(item->jid(), new Buddy(*item)));

		/**获取vcard*/
		Bodies::Get_Bodies().get_cardManage().fetch_vcard((*it).second->jid());

		StringList g = (*it).second->groups();
		StringList::const_iterator it_g = g.begin();
		for (; it_g != g.end(); ++it_g)
			printf("\tgroup: %s\n", (*it_g).c_str());
		RosterItem::ResourceMap::const_iterator rit =
			(*it).second->resources().begin();
		for (; rit != (*it).second->resources().end(); ++rit)
			printf("resource: %s\n", (*rit).first.c_str());
	}

	//Bodies::Get_Bodies().get_main_window().get_buddy_view().refreshList();
}
void BuddyList::handleRosterPresence(const RosterItem & item,
		const std::string & resource,
		Presence::PresenceType presence,
		const std::string & msg)
{
	Buddy* buddy=Bodies::Get_Bodies().get_buddy_list().find_buddy(item.jid().c_str());
	buddy->set_status(presence);
	buddy->setResource(resource);
	if(Presence::Unavailable == presence)
		buddy->set_sign_msg(_("offline"));
	else
		buddy->set_sign_msg(msg);
	Bodies::Get_Bodies().get_main_window().get_buddy_view().refreshBuddyStatus(item.jid());

}

void BuddyList::handleSelfPresence(const RosterItem & item,
		const std::string & resource,
		Presence::PresenceType presence,
		const std::string & msg )
{
	printf( "self presence received: %s/%s -- %d\n", item.jid().c_str(), resource.c_str(), presence );

}
bool BuddyList::handleSubscriptionRequest(const JID & jid,
		const std::string& msg  )
{
	//printf(" %s subscriptionRequest with %s\n",jid.bare().c_str(),msg.c_str());
	Gtk::MessageDialog dialog(_("Information from stranger"),false /*use markup*/,Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_OK_CANCEL);
	Glib::ustring msg_text = jid.bare() + _("Ask for a friend , press OK to add");
	dialog.set_secondary_text(msg_text);
	int result = dialog.run();
	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			//对方请求加为好友。
			StringList groups;
			JID id(jid);
			Bodies::Get_Bodies().get_client().rosterManager()->subscribe( id , "", groups, "");
			return true;
	break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
			return false;
			break;
		}
		default:
		{
			return false;
			break;
		}
	}
	return false;
}
bool  BuddyList::handleUnsubscriptionRequest(const JID & jid,
		const std::string& msg  )
{
	//printf(" %s UnsubscriptionRequest with %s\n",jid.bare().c_str(),msg.c_str());
	Gtk::MessageDialog dialog(_("The message from friend"));
	Glib::ustring msg_text = jid.bare() + _("Delete you from his buddy list");
	dialog.set_secondary_text(msg_text);
	dialog.run();
	return true;
}
void BuddyList::handleNonrosterPresence(Stanza * stanza)
{
	printf("接收到状态并不在列表中的人 %s \n",stanza->from().full().c_str());
	printf("可能说啥: %s\n", stanza->body().c_str());
}
