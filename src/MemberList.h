/*
* =====================================================================================
*
*       Filename:  MemberList.h
*
*    Description:  聊天室里的成员列表
*
*        Version:  1.0
*        Created:  2007年08月25日 11时13分33秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#ifndef  MEMBERLIST_FILE_HEADER_INC
#define  MEMBERLIST_FILE_HEADER_INC


#include <gtkmm.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include "pixmaps.h"
#include <functional>
#include "icalk.h"

using namespace std;

class MemberList : public Gtk::TreeView
{

public:
        MemberList();


        Glib::ustring getSelectMemberid();
        void addMember(const std::string& name, const Member& f_member);
        void removeMember(const std::string& id);
        void clearMember();
        bool isMember(const std::string& mid);
        /** 从聊天室成员的所属关系中获取相应的图标，有管理员，创建者，会员，被屏蔽者等等。*/
        Glib::RefPtr<Gdk::Pixbuf> getPixfromAffilliation(MUCRoomAffiliation affilliation);

protected:
        bool on_button_press_event(GdkEventButton *);

private:

struct MemberColumns : public Gtk::TreeModel::ColumnRecord
        {
                MemberColumns()
                {
                        add
                                (icon);

                        add
                                (name);

                        add
                                (mid);

                        add
                                (statusMsg);

                        add
                                (presence);

                        add
                                (affiliation);
                }

                Gtk::TreeModelColumn < Glib::RefPtr < Gdk::Pixbuf > >icon;
                Gtk::TreeModelColumn < Glib::ustring > name;
                Gtk::TreeModelColumn < Glib::ustring > mid;
                Gtk::TreeModelColumn < Glib::ustring > statusMsg;
                Gtk::TreeModelColumn < int >presence;
                Gtk::TreeModelColumn < int >affiliation;

        };

private:
        //排序函数
        int on_sort_compare(const Gtk::TreeModel::iterator& a,
                            const Gtk::TreeModel::iterator& b);

        Gtk::TreeModel::iterator getListIter(Gtk::TreeModel::
                                             Children children,
                                             const Glib::ustring & id);

        /** 比较好友*/

struct CompareMember: public binary_function < Gtk::TreeModel::Row,
                                const Glib::ustring, bool >
        {
                explicit CompareMember(const MemberColumns &
                                       column_): column(column_)
                {}

                bool operator () (const Gtk::TreeRow & lhs,
                                  const Glib::ustring & var) const
                {
                        return lhs[column.mid] == var;
                }

                const MemberColumns & column;
        };


private:
        MemberColumns columns;
        Glib::RefPtr<Gtk::ListStore> refListStore;

};



#endif   /* ----- #ifndef MEMBERLIST_FILE_HEADER_INC  ----- */

