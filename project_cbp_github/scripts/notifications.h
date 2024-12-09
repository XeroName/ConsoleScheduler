// notifications.h

// Header file which contains classes of Notifications
// ����� ���� ������ ����

#include <vector>



class Notifications {

    protected :
        virtual int showTestNotif();

};



// Message box notification; use char
class MsgBoxNotif : protected Notifications {

    public :
        virtual int showTestNotif() override;
        int showNotifBox(const std::vector<int> t_intHeap, const std::vector<std::string> t_strHeap);

};



// Message box notification wide; use wchar(wide-character, utf-8 & utf-16)
class MsgBoxWNotif : protected Notifications {

    public :
        virtual int showTestNotif() override;
        int showNotifBoxW(const std::vector<int> t_intHeap, const std::vector<std::wstring> t_wstrHeap);

};
