/*
* =====================================================================================
*
*       Filename:  TalkRoomHandler.cpp
*
*    Description:
*
*        Version:  1.0
*        Created:  2007年05月24日 19时43分28秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua xihels, lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#include <glib/gi18n.h>
#include "Bodies.h"
#include "TalkRoomHandler.h"


void TalkRoomHandler::joinRoom()
{
        ROOMLIST::const_iterator iter = roomList.begin();

        for (; iter != roomList.end();iter++)
                (*iter).second->join();


}

void TalkRoomHandler::addRoomItem(const std::string& jid, RoomItem* item)
{
        roomList.insert(roomList.end(), ROOMLIST::value_type(jid, item));
}

RoomItem* TalkRoomHandler::findRoom(const std::string& id) const
{
        ROOMLIST::const_iterator iter = roomList.find(id);

        if ( iter == roomList.end())
                return NULL;

        return (*iter).second;
}

RoomItem* TalkRoomHandler::findRoom(const MUCRoom* room)const
{
        std::string id = room->name() + "@" + room->service();
        return findRoom(id);
}

void TalkRoomHandler::handleMUCParticipantPresence(MUCRoom * room ,
                const MUCRoomParticipant
                participant, Presence::PresenceType presence)
{
        RoomItem* item = findRoom(room);
        MsgPage* page = item->getPage();
        std::string name = participant.nick->resource();
        std::string mid = participant.nick->full();

        //std::cout << "flags = " << participant.flags << " reason = " << participant.reason << " newNick = " << participant.newNick << " Affiliation= " << participant.affiliation << " role = " << participant.role << std::endl;
        MemberMap memberMap = item->getMemberMap();
        MemberMap::iterator iter;
        iter = memberMap.find(name);

        if (iter == memberMap.end()) {
                Member member;
                member.id = participant.nick->full();
                member.affiliation = participant.affiliation;
                member.role = participant.role;
                member.flags = participant.flags;
                member.status = participant.status;
                member.presence = presence;

                item->addMember(name, member);

                if (NULL != page) {
                        std::string msg_ = name + _(" join in the room");
                        page->showStatusBarMsg(msg_);
                        page->addMember(name, member);
                }
        }

        if (presence == Presence::Unavailable) {
                item->removeMember(name);

                if (NULL != page) {
                        std::string msg_ = name + _(" leave the room");
                        page->showStatusBarMsg(msg_);
                        page->removeMember(mid);
                }
        }

        //if (NULL != page)
        //        page->refreshMember();

}


void TalkRoomHandler::handleMUCMessage(MUCRoom* room,
                                       const Message& msg, bool priv)
{

        const std::string nick = msg.from().resource();
        bool history = msg.when() ? true : false;
        RoomItem* item = findRoom(room);
        MsgPage* page = item->getPage();

        if (NULL == page) {
                const std::string label = room->name() + "@" + room->service();
                MsgPage* page_ = new MsgPage(label, item, 1);
                item->setPage(page_);
                page = item->getPage();
                page->refreshMember();
                page->setSubject();
                Bodies::Get_Bodies().get_msg_window().add_page(*page);
        }

        if (priv) {
                Glib::ustring msg_ = _("said to you: ") + msg.body();
                page->showMessage(nick, msg_);
                return ;
        }

        if (history) {
                const DelayedDelivery* dd = msg.when();

                if (dd)
                        DLOG("message was sent at %s\n", dd->stamp().c_str());

                page->showHistroy(nick, msg.body());
        } else
                page->showMessage(nick, msg.body());



}

void TalkRoomHandler::handleMUCSubject(MUCRoom * room ,
                                       const std::string & nick,
                                       const std::string & subject)
{
        RoomItem* item = findRoom(room);
        MsgPage* page = item->getPage();
        Glib::ustring msg = nick + _(" has set the Subject: ") + subject;
        item->setSubject(subject);

        if (page != NULL) {
                page->setSubject();
                page->showSystemMsg(msg);

        }

}

void TalkRoomHandler::handleMUCError(MUCRoom * room , StanzaError error)
{
        DLOG(" room %s got an error: %d\n", room->name().c_str(), error);
        //room->leave();
}

void TalkRoomHandler::handleMUCInfo(MUCRoom * room , int features,
                                    const std::string & name,
                                    const DataForm * infoForm)
{
        //printf("features: %d, name: %s, form xml: %s\n", features,
        //      name.c_str(), infoForm->tag()->xml().c_str());
}

void TalkRoomHandler::handleMUCItems(MUCRoom * room ,
                                     const Disco::ItemList & items)
{
        RoomItem* item = findRoom(room);
        item->setMemberList(items);

}



void TalkRoomHandler::handleMUCInviteDecline(MUCRoom * room ,
                const JID & invitee,
                const std::string & reason)
{
        DLOG("Invitee %s declined invitation. reason given: %s\n",
             invitee.full().c_str(), reason.c_str());
}

bool TalkRoomHandler::handleMUCRoomCreation(MUCRoom * room)
{
        DLOG("room %s didn't exist, beeing created.\n",
             room->name().c_str());
        return true;
}


