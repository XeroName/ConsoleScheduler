// winNotifHandler.cpp

#include <iostream>
#include <string>
#include <windows.h>
//#include <future> // for async functions
//#include <thread> // for async functions

#include "notifications.h"

int MsgBoxNotif::showTestNotif()
{
    // show test message box of window notification
    MessageBox( NULL, TEXT( "This is test message" ), TEXT( "Test Message Box" ), MB_ICONINFORMATION | MB_OK );

    return 0;
}

int MsgBoxNotif::showNotifBox(const std::vector<int> t_intHeap, const std::vector<std::string> t_strHeap)
{
    const std::string sch_name = std::string( t_strHeap[0] ).substr(0, 19); // name of schedule, limit size of string to 19 byte (error occurs from 20 bytes)
    const char* sch_name_char = sch_name.c_str(); // name of schedule, in pointer of C-style character array

    const int sch_year = t_intHeap[0]; // year of schedule
    const int sch_month = t_intHeap[1]; // month of schedule
    const int sch_day = t_intHeap[2]; // day of schedule
    const int sch_hour = t_intHeap[3]; // hour of schedule
    const int sch_minute = t_intHeap[4]; // minute of schedule
    const int sch_second = t_intHeap[5]; // second of schedule

    const std::string sch_comment = std::string( t_strHeap[1] ).substr(0, 19); // comment of schedule, limit size of string to 19 byte (error occurs from 20 bytes)



    // create date-time to display in string
    std::string dateToText = ( std::to_string(sch_year) + "년 " + std::to_string(sch_month) + "월 " + std::to_string(sch_day) + "일" );
    if (sch_hour > 0) { dateToText.append( " " + std::to_string(sch_hour) + "시" ); }
    if (sch_minute > 0) { dateToText.append( " " + std::to_string(sch_minute) + "분" ); }
    if (sch_second > 0) { dateToText.append( " " + std::to_string(sch_second) + "초" ); }

    const std::string msgContent = std::string( std::string("# 일정 알림 #\n\n") + "[ " + sch_name + " ]\n" + dateToText + "\n\n" + sch_comment + "\n\n* '확인' 버튼을 누르면 알림을 확인한 것으로 간주됩니다." );
    // create message content of schedule, and save it to pointer of C-style character array
    const char* msgContent_char = (msgContent.c_str());



    /*
    // this code has been abandoned, because 'MessageBox()' function is not available in asynchronous function
    // to show multiple notifications window at same time, 'CreateDialog()' function should be used
    // referenced by : https://stackoverflow.com/questions/3556089/is-there-any-way-to-have-async-messagebox

    std::future <int> notif_box = std::async( std::launch::async, MessageBox, NULL, TEXT("This is test message"), TEXT(sch_name_char), (MB_ICONINFORMATION | MB_OK) );
    return notif_box.get(); // returns IDOK if 'ok' button pressed, IDOK is defined to 1
    */

    //int _msgBoxID = MessageBox( NULL, TEXT( msgContent_char ), TEXT( sch_name_char ), MB_ICONINFORMATION | MB_OK );
    int _msgBoxID = MessageBox( NULL, TEXT( msgContent_char ), TEXT( sch_name_char ), MB_ICONINFORMATION | MB_OK );
    if (_msgBoxID == IDOK) { return 1; }
}


/*
int MsgBoxWNotif::showTestNotif()
{
    // show test message box of window notification
    MessageBoxW( NULL, LPCWSTR( "This is test message" ), LPCWSTR( "Test Message Box" ), MB_ICONINFORMATION | MB_OK );

    return 0;
}

int MsgBoxWNotif::showNotifBoxW(const std::vector<int> t_intHeap, const std::vector<std::wstring> t_wstrHeap)
{
    const std::wstring sch_name = std::wstring( t_wstrHeap[0] ).substr(0, 30); // name of schedule, limit size of string to 19 byte (error occurs from 20 bytes)
    const wchar_t* sch_name_wch = sch_name.c_str(); // name of schedule, in pointer of C-style character array

    const int sch_year = t_intHeap[0]; // year of schedule
    const int sch_month = t_intHeap[1]; // month of schedule
    const int sch_day = t_intHeap[2]; // day of schedule
    const int sch_hour = t_intHeap[3]; // hour of schedule
    const int sch_minute = t_intHeap[4]; // minute of schedule
    const int sch_second = t_intHeap[5]; // second of schedule

    const std::wstring sch_comment = std::wstring( t_wstrHeap[1] ).substr(0, 30); // comment of schedule, limit size of string to 19 byte (error occurs from 20 bytes)



    // create date-time to display in wstring
    std::wstring dateToWstr = ( std::to_wstring(sch_year) + L"년 " + std::to_wstring(sch_month) + L"월 " + std::to_wstring(sch_day) + L"일" );
    if (sch_hour > 0) { dateToWstr.append( " " + std::to_wstring(sch_hour) + L"시" ); }
    if (sch_minute > 0) { dateToWstr.append( " " + std::to_wstring(sch_minute) + L"분" ); }
    if (sch_second > 0) { dateToWstr.append( " " + std::to_wstring(sch_second) + L"초" ); }

    const std::wstring msgContent = std::wstring( std::wstring(L"# 일정 알림 #\n\n") + L"[ " + sch_name + L" ]\n" + dateToWstr + L"\n\n" + sch_comment + L"\n\n* '확인' 버튼을 누르면 알림을 확인한 것으로 간주됩니다." );
    // create message content of schedule, and save it to pointer of C-style character array
    const wchar_t* msgContent_wch = (msgContent.c_str());

    int _msgBoxID = MessageBoxW( NULL, msgContent_wch, sch_name_wch, MB_ICONINFORMATION | MB_OK );
    if (_msgBoxID == IDOK) { return 1; }
}
*/
