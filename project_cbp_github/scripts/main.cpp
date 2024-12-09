// main.cpp

/*

** An important notice : XERONAME™ is a pseudonym and also trademark of the author of this program. **

Console Scheduler by XERONAME™
First created at 2024-10-14 (UTC+9)
Latest edit at 2024-12-09 (UTC+9)

First developed IDE environment : Code::Blocks 20.03
Tested OS : Windows 10 Home 22H2 (OS Build : 19045.3570)

Changelogs :
Ver 2.0.0 finished at 2024-12-07 23:44 (UTC+9); Code::Blocks 20.09 IDE
Ver 2.0.1 (2024-12-08 23:18 UTC+9) : Added Semantic versioning system to program; Added console title
Ver 2.0.2 (2024-12-09 04:17 UTC+9) : Bug fixes related about 'long' data type
Ver 2.0.3 (2024-12-09 17:06 UTC+9) : Bug fixes about "program not terminated off when user select 'terminate program' menu"

*/

#include <iostream>
#include <string>
#include <future> // for async functions
#include <ctime>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "tmp_containers.h"
#include "schedules.h"
#include "notifications.h"
#include "msg_formats.h"



Schedules _sch; // create object of 'Schedules' class
MsgBoxNotif _winNotif; // create object of 'MsgBoxNotif' class

using namespace std;



// ========== name, version, title of the program .begin ==========

const string program_name = "Console Scheduler"; // name of the program

const int program_version_major = 2; // major version of program, according to Semantic versioning
const int program_version_minor = 0; // minor version of program, according to Semantic versioning
const int program_version_patch = 3; // patch version of program, according to Semantic versioning
const string program_version_string = ( to_string(program_version_major) + "." + to_string(program_version_minor) + "." + to_string(program_version_patch) ); // full version of program

const char* program_title = ( (string(program_name) + " " + program_version_string).c_str() ); // title of the program, in pointer of C-style char array

// ========== name, version, title of the program .end ==========



// ========== command and modes .begin ==========

const string UIN_BACK = "_back"; // user input command to back to previous
const string UIN_MENU = "_menu"; // user input command to back to menu
const string UIN_RMALL = "_rmall"; // user input command to remove all schedule
const string UIN_RMOUTD = "_rmotd"; // user input command to remove all outdated schedule
const string UIN_RMCHECK = "_rmchk"; // user input command to remove all checked schedule
const string UIN_RSALL = "_rsall"; // user input command to reserve all schedule
const string UIN_DRSALL = "_drsall"; // user input command to de-reserve all schedule

bool mode_powerSaving = false; // power-saving mode of program; add 100ms delay to 'asyncLoops' when enabled, through 'Sleep()' in <windows.h>
bool mode_rsvSchByDef = true; // if enabled, program will automatically reserve schedule when registered

// ========== command and modes .end ==========



// ========== system functions .begin ==========

// Function which clear characters in console window (requires <windows.h>)
// Referenced by : https://stackoverflow.com/questions/6486289/how-can-i-clear-console
void clearScreen() {
    char fill = ' ';

    COORD topLeft = {0,0};
    CONSOLE_SCREEN_BUFFER_INFO _scr;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(console, &_scr);
    DWORD written, cells = _scr.dwSize.X * _scr.dwSize.Y;
    FillConsoleOutputCharacter(console, fill, cells, topLeft, &written);
    FillConsoleOutputAttribute(console, _scr.wAttributes, cells, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

void printLine() { cout << "========================================" << endl; }

// Function that resets reserved message
string reservedMessage = "";
void resetReservedMsg() { reservedMessage = ""; }

// Function that prints reserved message to console
void printReservedMsg() {
    if (reservedMessage != "") {
        printLine();
        cout << "\n" << reservedMessage << "\n\n";
        resetReservedMsg();
    }
}

void waitForAnyKeysToReturn() {
    cout << "\n* " << MSG_KEYNPUT_ANYKEY_BACKTO_MENU;
    getch(); // wait for any key input
}

void printGreeting() {
    printLine();

    cout << "\nWelcome to Console Scheduler\nVer: " << program_version_string << " by XERONAME\n\n제작자 : 전형진\n\n";
}

void printMenus() {

    printLine();

    cout << "\n# 메뉴 #\n\n";
    cout << "[ 1 ] " << MENU_SCHEDULES_DISPLAY << endl;
    cout << "[ 2 ] " << MENU_SCHEDULE_ADD << endl;
    cout << "[ 3 ] " << MENU_SCHEDULE_REMOVE << endl;
    cout << "[ 4 ] " << MENU_SCHEDULE_RESERVE << endl;

    cout << "[ 5 ] ";
    if (mode_rsvSchByDef == true) {  cout << MENU_MODE_SCHEDULE_RESERVE_DEFAULT << " [ *" << DISPLAY_ON << " ]" << endl; }
    else { cout << MENU_MODE_SCHEDULE_RESERVE_DEFAULT << " [ " << DISPLAY_OFF << " ]" << endl; }

    cout << "[ 6 ] ";
    if (mode_powerSaving == true) {  cout << MENU_MODE_POWERSAVING << " [ *" << DISPLAY_ON << " ]" << endl; }
    else { cout << MENU_MODE_POWERSAVING << " [ " << DISPLAY_OFF << " ]" << endl; }

    cout << "[ 7 ] " << MENU_PROGRAM_TERMINATE << endl;

    cout << endl;
    cout << "* 절전 모드가 활성화되면, 프로그램이 저장된 일정의 만료 여부를 덜 주기적으로 검사합니다.\n";
    cout << "* 모든 일정은 로컬 파일에 자동으로 저장되어 동기화됩니다.\n\n";

}

int getMenuSelection (int startNum, int endNum) {

    bool isValid = false;
    string userInput;
    int userSelection;

    printLine();

    while ( isValid == false ) {

        cout << "\n* 메뉴 목록 중 한 가지를 선택하세요 : ";

        try {
            std::getline(std::cin >> std::ws, userInput); // get user input, including spaces

            userSelection = stoi(userInput); // convert string input to integer

            // detect whether input value is in valid range
            if ( startNum <= userSelection && userSelection <= endNum ) {
                cout << endl;
                isValid = true;
            }
            else { cout << "* \"" << userSelection <<  "\" 은(는) 올바른 항목이 아닙니다. 범위 내의 값을 선택해 주세요.\n\n"; }
        }

        catch (const invalid_argument & e) { cout << "* " << MSG_ERR_INPUT_NONINT << "\n\n"; } // handle the invalid input exception
        catch (const out_of_range & e) { cout << "* " << MSG_ERR_INPUT_OUTOF_RANGE << "\n\n"; } // handle the input overflow exception
    }

    return userSelection;

}

string getScheduleAsText(int sNum) {

    string textFormatToVisible;

    // get name of target schedule
    const string t_schName = _sch.getScheduleName(sNum);

    Ts_int main_gSAT; // create object of 'Ts_int' class, to use integer-type temporary vector array

    // get times of target schedule
    const int* t_ptr = _sch.getScheduleTimes(sNum, main_gSAT.intHeap);
    int sch_times[6];
    copy( t_ptr, t_ptr +6, sch_times );

    const string t_schYear = to_string(sch_times[0]);
    const string t_schMon = to_string(sch_times[1]);
    const string t_schDay = to_string(sch_times[2]);
    const string t_schHour = to_string(sch_times[3]);
    const string t_schMin = to_string(sch_times[4]);
    const string t_schSec = to_string(sch_times[5]);

    textFormatToVisible = "[ " + t_schName + " ] .. " + t_schYear + "년 " + t_schMon + "월 " + t_schDay + "일 " + t_schHour + "시 " + t_schMin + "분 " + t_schSec + "초";

    const int sch_status = _sch.getScheduleStatus(sNum); // get status of target schedule

    // add information display about status of schedule
    if (sch_status == 1) {
        textFormatToVisible.append( string(" .. (") + DISPLAY_SCHEDULE_RESERVED + ")" );
    }
    else if (sch_status == 2) {
        textFormatToVisible.append( string(" ..  (") + DISPLAY_SCHEDULE_OUTDATED + ")" );
    }
    else if (sch_status == 3) {
        textFormatToVisible.append( string(" ..  (") + DISPLAY_SCHEDULE_CHECKED + ")" );
    }

    return textFormatToVisible;
}

void printAllSchedules() {

    const long t_schAmount = _sch.getSchedulesAmount(); // amount of schedules

    if ( t_schAmount > 0 ) {
        for (long tmpc=0; tmpc < t_schAmount; tmpc++) {
            string visibleText = "\n[ " + to_string(tmpc+1) + " ] ...... " + getScheduleAsText(tmpc);

            cout << visibleText;
        }
    }
    else {
        cout << "\n* " << MSG_SCHEDULE_EMPTY;
    }

}

int selectSchedule(const int msgType) {
    /*
    msgType presents type of output message :
    0 = print only 'back to menu'
    1 = print only 'back to previous'
    2 = print 'back to menu and previous'
    3 = print 'remove all' and 'remove outdated' and 'remove checked' and 'back to menu'
    4 = print 'reserve and dereserve all' and 'back to menu'

    return type presents type of command :
    >0 = normal schedule selection
    0 = back to menu
    -1 = back to previous
    -2 = remove all schedule
    -3 = reserve all schedule
    -4 = de-reserve all schedule
    */

    bool isValid = false;
    string userInput;
    int userSelection;

    while ( isValid == false ) {

        cout << endl;
        printLine();

        const long schAmount = _sch.getSchedulesAmount();

        cout << "\n* " << MSG_SCHEDULE_CHOOSE << endl;

        // print messages according to 'msgType'
        if (msgType == 0) { cout << "\n* " << MSG_NOTICE_BACKTO_MENU; }
        else if (msgType == 1) { cout << "\n* " << MSG_NOTICE_BACKTO_PREV; }
        else if (msgType == 2) { cout << "\n* " << MSG_NOTICE_BACKTO_PREVOR_MENU; }
        else if (msgType == 3) {
                cout << "\n* " << MSG_NOTICE_REMOVEALL;
                cout << "\n* " << MSG_NOTICE_REMOVE_OUTDATED;
                cout << "\n* " << MSG_NOTICE_REMOVE_CHECKED;
                cout << "\n* " << MSG_NOTICE_BACKTO_MENU;
        }
        else if (msgType == 4) {
                cout << "\n* " << MSG_NOTICE_RESERVEALL;
                cout << "\n* " << MSG_NOTICE_DERESERVEALL;
                cout << "\n* " << MSG_NOTICE_BACKTO_MENU;
        }

        cout << "\n\n-> ";

        try {
                cin >> userInput;

                if ( (userInput == UIN_MENU) && ((msgType == 0) || (msgType == 2) || (msgType == 3) || (msgType == 4)) )
                {
                    // input 'menu command' to go back to menu
                    return 0;
                }
                // input 'back command' to go back to previous
                else if ( (userInput == UIN_BACK) && ((msgType == 1) || (msgType == 2)) ) { return -1; }
                // input 'remove all command' to remove all schedule
                else if ( (userInput == UIN_RMALL) && (msgType == 3) ) { return -2; }
                // input 'reserve all command' to reserve all schedule
                else if ( (userInput == UIN_RSALL) && (msgType == 4) ) { return -3; }
                // input 'de-reserve all command' to de-reserve all schedule
                else if ( (userInput == UIN_DRSALL) && (msgType == 4) ) { return -4; }
                // input 'remove all outdated command' to remove all outdated schedule
                else if ( (userInput == UIN_RMOUTD) && (msgType == 3) ) { return -5; }
                // input 'remove all checked command' to remove all checked schedule
                else if ( (userInput == UIN_RMCHECK) && (msgType == 3) ) { return -6; }
                // normal input selection
                else { userSelection = stoi(userInput); } // convert string input to integer



                // detect whether input value is in valid range
                if ( 1 <= userSelection && userSelection <= schAmount ) {
                    isValid = true;
                }
                else { cout << "* \"" << userSelection << "\" 은(는) 올바른 항목이 아닙니다. 범위 내의 값을 선택해 주세요.\n"; }
            }

            catch (const invalid_argument & e) { cout << "* " << MSG_ERR_INPUT_NONINT << "\n"; } // handle the invalid input exception
            catch (const out_of_range & e) { cout << "* " << MSG_ERR_INPUT_OUTOF_RANGE << "\n"; } // handle the input overflow exception
    }

    return userSelection;
}

string* getUserInput_schName() {

    bool isValid = false;
    string userInput;

    while ( isValid == false ) {

        cout << "\n* 일정의 이름을 입력하세요.\n* " << MSG_NOTICE_BACKTO_MENU << " : ";

        try {
            std::getline(std::cin >> std::ws, userInput); // get user input, including spaces

            if (userInput == UIN_MENU) {
                // input 'menu key' to cancel selection and back to menu
                cout << endl;

                Ts_str main_gUI_sN; // create object of 'Ts_str' class, to use string-type temporary vector array
                main_gUI_sN.cleanElements(); // clear elements in vector
                main_gUI_sN.strHeap.reserve(2); // reserve memory heaps

                // save data to temp variable
                main_gUI_sN.strHeap.push_back("b");
                main_gUI_sN.strHeap.push_back("");

                string* t_ptr = main_gUI_sN.strHeap.data(); // create and set pointer to point first element of temp var

                return t_ptr;
            }

            isValid = true;
        }
        catch (const out_of_range & e) { cout << "* " << MSG_ERR_INPUT_OUTOF_RANGE << "\n\n"; } // handle the input overflow exception

    }

    cout << endl;

    Ts_str main_gUI_sN; // create object of 'Ts_str' class, to use string-type temporary vector array
    main_gUI_sN.cleanElements(); // clear elements in vector
    main_gUI_sN.strHeap.reserve(2); // reserve memory heaps

    // save data to temp variable
    main_gUI_sN.strHeap.push_back("p"); // first data of array declares "back to menu" status ("b" for back to previous, "p" for normal data transfer)
    main_gUI_sN.strHeap.push_back(userInput);

    string* t_ptr = main_gUI_sN.strHeap.data(); // create and set pointer to point first element of temp var

    return t_ptr;

}

int* getUserInput_schTimes() {

    bool t_isValid = false;
    string userInput;
    int userSelection;

    cout << "\n* Y=연도, M=월, D=일, h=시, m=분, s=초";
    cout << "\n* 올바른 입력 형식 : YYYYMMDDhhmmss (hhmmss는 전체 또는 일부를 생략할 수 있습니다.)\n";

    while ( t_isValid == false ) {

        cout << "\n* 일정의 날짜 및 시간을 올바른 형식으로 입력하세요.\n* " << MSG_NOTICE_BACKTO_PREV << " : ";

        try {
            std::getline(std::cin >> std::ws, userInput); // get user input, including spaces

            if (userInput == UIN_BACK) {
                // input 'back key' to cancel selection and back to previous process
                cout << endl;

                Ts_int main_gUI_sT; // create object of 'Ts_int' class, to use integer-type temporary vector array
                main_gUI_sT.cleanElements(); // clear elements in vector
                main_gUI_sT.intHeap.reserve(7); // reserve memory heaps

                // save data to temp variable
                main_gUI_sT.intHeap.assign(7, 0);
                main_gUI_sT.intHeap[0] = -1; // first data of array declares "back to previous" status (-1 for back to previous, 0 for normal data transfer)


                int* t_ptr = main_gUI_sT.intHeap.data(); // create and set pointer to point first element of temp var

                return t_ptr;
            }

            int uinLen = userInput.length();

            // detect whether input value is in valid format
            if ( 8 <= uinLen && uinLen <= 14 && (uinLen%2) == 0 ) {

                // try to convert string-type of each character to integer-type, to check integer-type validity of input string
                for (int tmpc=0; tmpc<uinLen; tmpc++) {
                    stoi( userInput.substr(tmpc, 1) ); // try to convert string-type of single character to integer-type
                }

                // PASS input format validity

                int dataAmount = (uinLen/2)-1; // set data block amount to save
                int additionalDataAmount = dataAmount -3;

                string eDataFormat = userInput;
                for (int tmpc=0; tmpc<(3 - additionalDataAmount); tmpc++) { eDataFormat += "00"; }

                const int t_year = stoi( eDataFormat.substr(0, 4) ); // year
                const int t_month = stoi( eDataFormat.substr(4, 2) ); // month
                const int t_day = stoi( eDataFormat.substr(6, 2) ); // day
                const int t_hour = stoi( eDataFormat.substr(8, 2) ); // hour
                const int t_minute = stoi( eDataFormat.substr(10, 2) ); // minute
                const int t_second = stoi( eDataFormat.substr(12, 2) ); // second

                // check actual validity of date and time
                const bool t_validity_date = _sch.isValidDate(t_year, t_month, t_day);
                const bool t_validity_time = _sch.isValidTime(t_hour, t_minute, t_second);

                if ( (t_validity_date == true) && (t_validity_time == true) ) {
                    // PASS date and time validity

                    cout << endl;

                    t_isValid = true;

                    Ts_int main_gUI_sT; // create object of 'Ts_int' class, to use integer-type temporary vector array
                    main_gUI_sT.intHeap.clear(); // clear elements in vector
                    main_gUI_sT.intHeap.reserve(7); // reserve memory heaps

                    // save data to temp variable
                    main_gUI_sT.intHeap.push_back(0); // first data of array declares "back to previous" status (-1 for back to previous, 0 for normal data transfer)
                    main_gUI_sT.intHeap.push_back(t_year);
                    main_gUI_sT.intHeap.push_back(t_month);
                    main_gUI_sT.intHeap.push_back(t_day);
                    main_gUI_sT.intHeap.push_back(t_hour);
                    main_gUI_sT.intHeap.push_back(t_minute);
                    main_gUI_sT.intHeap.push_back(t_second);

                    int* t_ptr = main_gUI_sT.intHeap.data(); // create and set pointer to point first element of temp var

                    return t_ptr;
                }
                else if (t_validity_date != true) {
                    // date is not valid
                    cout << "* " << MSG_ERR_DATE_INVALID << "\n";
                }
                else if (t_validity_time != true) {
                    // time is not valid
                    cout << "* " << MSG_ERR_TIME_INVALID << "\n";
                }

            }
            else { cout << "* " << MSG_ERR_INPUT_LENGTH_INVALID << "\n"; }
        }
        catch (const invalid_argument & e) { cout << "* " << MSG_ERR_INPUT_NONINT << "\n"; } // handle the invalid input exception
        catch (const out_of_range & e) { cout << "* " << MSG_ERR_INPUT_OUTOF_RANGE << "\n"; } // handle the input overflow exception

    }

}

string* getUserInput_schComment() {

    bool isValid = false;
    string userInput;

    while ( isValid == false ) {

        cout << "\n* 일정의 주석을 입력하세요.\n* " << MSG_NOTICE_BACKTO_PREV << " : ";

        try {
            std::getline(std::cin >> std::ws, userInput); // get user input, including spaces

            if (userInput == UIN_BACK) {
                // input 'back key' to cancel selection and back to previous process
                cout << endl;

                Ts_str main_gUI_sC; // create object of 'Ts_str' class, to use string-type temporary vector array
                main_gUI_sC.cleanElements(); // clear elements in vector
                main_gUI_sC.strHeap.reserve(2); // reserve memory heaps

                // save data to temp variable
                main_gUI_sC.strHeap.push_back("b");
                main_gUI_sC.strHeap.push_back("");

                string* t_ptr = main_gUI_sC.strHeap.data(); // create and set pointer to point first element of temp var

                return t_ptr;
            }

            isValid = true;
        }
        catch (const out_of_range & e) { cout << "* " << MSG_ERR_INPUT_OUTOF_RANGE << "\n\n"; } // handle the input overflow exception

    }

    cout << endl;

    Ts_str main_gUI_sC; // create object of 'Ts_str' class, to use string-type temporary vector array
    main_gUI_sC.cleanElements(); // clear elements in vector
    main_gUI_sC.strHeap.reserve(2); // reserve memory heaps

    // save data to temp variable
    main_gUI_sC.strHeap.push_back("p"); // first data of array declares "back to menu" status ("b" for back to previous, "p" for normal data transfer)
    main_gUI_sC.strHeap.push_back(userInput);

    string* t_ptr = main_gUI_sC.strHeap.data(); // create and set pointer to point first element of temp var

    return t_ptr;

}

int registerNewSchedule()
{

    int tmp_process = -1;

    int* t_ptr_int;
    string* t_ptr_str;

    string sch_name;
    int t_year;
    int t_month;
    int t_day;
    int t_hour;
    int t_minute;
    int t_second;
    string sch_comment;

    int tmp_taskProcess = 1;
    do {

        while ( tmp_taskProcess == 1 ) {
            printLine();

            // get name of schedule from user input
            t_ptr_str = getUserInput_schName();

            if ( t_ptr_str[0] == "b" ) {
                tmp_taskProcess = -1; // back to menu
            }
            else {
                sch_name = t_ptr_str[1]; // name of schedule
                tmp_taskProcess = 2; // process completed and continue the next step
            }
        }

        while ( tmp_taskProcess == 2 ) {
            printLine();

            // get times data of schedule from user input
            t_ptr_int = getUserInput_schTimes();

            if ( t_ptr_int[0] == -1 ) {
                tmp_taskProcess = 1; // back to previous process
            }

            else {
                int t_times[6];
                copy(t_ptr_int +1, t_ptr_int +7, t_times);

                t_year = t_times[0]; // year
                t_month = t_times[1]; // month
                t_day = t_times[2]; // day
                t_hour = t_times[3]; // hour
                t_minute = t_times[4]; // minute
                t_second = t_times[5]; // second

                tmp_taskProcess = 3; // process completed and continue the next step
            }
        }

        while ( tmp_taskProcess == 3 ) {
            printLine();

            // get comment of schedule from user input
            t_ptr_str = getUserInput_schComment();

            if ( t_ptr_str[0] == "b" ) {
                tmp_taskProcess = 2; // back to previous process
            }
            else {
                sch_comment = t_ptr_str[1]; // comment of schedule
                tmp_taskProcess = 0; // process completed and continue the final step
            }
        }

    }
    while ( tmp_taskProcess > 0 );



    // final step to add new schedule
    if ( tmp_taskProcess == 0 ) {
        cout << "\n* " << DISPLAY_SCHEDULE_NEW << " [ " << sch_name << " ]을(를) 저장하는 중입니다...\n";

        // write/overwrite time data to array
        _sch.addSchedule(sch_name, t_year, t_month, t_day, t_hour, t_minute, t_second, sch_comment, mode_rsvSchByDef);
        _sch.saveSchedulesToFile(); // save schedules data to save file

        reservedMessage = ( string("* [ ") + sch_name + " ] " + MSG_SCHEDULE_ADDED );

        return 0;
    }

    else if ( tmp_taskProcess == -1 ) {
        return -1;
    }

}

void removeSchedule(int sNum)
{
    string userInput;

    const string sch_name = _sch.getScheduleName(sNum);

    cout << "\n* 정말로 [ " << (sNum +1) << " ] 번 항목 [ " << sch_name << " ] 일정을 삭제하시겠습니까? (y/n) : ";
    std::getline(std::cin >> std::ws, userInput); // get user input, including spaces

    if (userInput == "y" || userInput == "Y") {
        _sch.removeSchedule(sNum); // remove schedule from vector
        _sch.saveSchedulesToFile(); // save schedules data to save file

        reservedMessage = ( string("* [ ") + sch_name + " ] " + MSG_SCHEDULE_REMOVED );
    }
    else { reservedMessage = ( string("* ") + MSG_SCHEDULE_REMOVE_CANCELED ); }
}

// Remove every schedule
void removeAllSchedules()
{
    string userInput;

    cout << "\n* 정말로 모든 일정을 삭제하시겠습니까? (y/n) : ";
    std::getline(std::cin >> std::ws, userInput); // get user input, including spaces

    if (userInput == "y" || userInput == "Y") {
        _sch.clearSchedules(); // remove schedule from vector
        _sch.saveSchedulesToFile(); // save schedules data to save file

        reservedMessage = ( string("* 모든 ") + MSG_SCHEDULE_REMOVED );
    }
    else { reservedMessage = ( string("* ") + MSG_SCHEDULE_REMOVE_CANCELED ); }
}

// Remove every outdated schedule
void removeOutdatedSchedules()
{
    string userInput;

    cout << "\n* 정말로 만료된 모든 일정을 삭제하시겠습니까? (y/n) : ";
    std::getline(std::cin >> std::ws, userInput); // get user input, including spaces

    if (userInput == "y" || userInput == "Y") {

        bool _taskFinished = false;

        while (_taskFinished == false)
        {
            const long sch_amount = _sch.getSchedulesAmount(); // get amount of schedules
            for (long i=0; i<sch_amount; i++) {
                if ( (i+1) >= sch_amount ) { _taskFinished = true; }

                const int sch_status = _sch.getScheduleStatus(i); // get status of specific schedule
                if (sch_status >= 2) {
                    // remove schedule if outdated or checked
                    _sch.removeSchedule(i);
                    break; // break loops to re-start the scope
                }
            }
        }
        _sch.saveSchedulesToFile(); // save schedules data to save file

        reservedMessage = ( string("* 만료된 모든 ") + MSG_SCHEDULE_REMOVED );

    }
    else { reservedMessage = ( string("* ") + MSG_SCHEDULE_REMOVE_CANCELED ); }
}

// Remove every checked schedule
void removeCheckedSchedules()
{
    string userInput;

    cout << "\n* 정말로 확인된 모든 일정을 삭제하시겠습니까? (y/n) : ";
    std::getline(std::cin >> std::ws, userInput); // get user input, including spaces

    if (userInput == "y" || userInput == "Y") {

        bool _taskFinished = false;

        while (_taskFinished == false)
        {
            const long sch_amount = _sch.getSchedulesAmount(); // get amount of schedules
            for (long i=0; i<sch_amount; i++) {
                if ( (i+1) >= sch_amount ) { _taskFinished = true; }

                const int sch_status = _sch.getScheduleStatus(i); // get status of specific schedule
                if (sch_status == 3) {
                    // remove schedule if status is 'checked'
                    _sch.removeSchedule(i);
                    break; // break loops to re-start the scope
                }
            }
        }
        _sch.saveSchedulesToFile(); // save schedules data to save file

        reservedMessage = ( string("* 확인된 모든 ") + MSG_SCHEDULE_REMOVED );

    }
    else { reservedMessage = ( string("* ") + MSG_SCHEDULE_REMOVE_CANCELED ); }
}

void reserveSchedule_main(int sNum)
{
    const string sch_name = _sch.getScheduleName(sNum);
    const int sch_status = _sch.getScheduleStatus(sNum);

    if (sch_status == 0) {
        _sch.reserveSchedule(sNum); // reserve schedule to get notification
        _sch.saveSchedulesToFile(); // save schedules data to save file

        reservedMessage = ( string("* [ ") + sch_name + " ] " + MSG_SCHEDULE_RESERVED );
    }
    else if (sch_status == 1) {
        _sch.dereserveSchedule(sNum); // de-reserve schedule not to get notification
        _sch.saveSchedulesToFile(); // save schedules data to save file

        reservedMessage = ( string("* [ ") + sch_name + " ] " + MSG_SCHEDULE_DERESERVED );
    }
    else { reservedMessage = ( string("* ") + MSG_ERR_SCHEDULE_OUTDATED_NOTRESERVED ); }
}

void reserveAllSchedules()
{
    const long sch_amount = _sch.getSchedulesAmount(); // get amount of schedules

    for (long i=0; i<sch_amount; i++) {
        const int sch_status = _sch.getScheduleStatus(i); // get status of specific schedule
        if (sch_status == 0) { _sch.reserveSchedule(i); } // reserve schedule if not reserved
    }

    reservedMessage = ( string("* 모든 ") + MSG_SCHEDULE_RESERVED );
    _sch.saveSchedulesToFile(); // save schedules data to save file

}

void dereserveAllSchedules()
{
    const long sch_amount = _sch.getSchedulesAmount(); // get amount of schedules

    for (long i=0; i<sch_amount; i++) {
        const int sch_status = _sch.getScheduleStatus(i); // get status of specific schedule
        if (sch_status == 1) { _sch.dereserveSchedule(i); } // reserve schedule if not reserved
    }

    reservedMessage = ( string("* 모든 ") + MSG_SCHEDULE_DERESERVED );
    _sch.saveSchedulesToFile(); // save schedules data to save file

}

// Function which checks all schedules and returns index of first single schedule, which is outdated and un-triggered
int getFirstNewOutdatedSchedule(const long arg_startNum = 0) {
    const long sch_amount = _sch.getSchedulesAmount();

    long start_num = arg_startNum;
    if (arg_startNum >= sch_amount) { start_num = 0; }

    for (long i=start_num; i<sch_amount; i++) {

        Ts_int main_gFNOS; // create object of 'Ts_int' class, to use integer-type temporary vector array

        // get times of target schedule
        const int* t_ptr = _sch.getScheduleTimes(i, main_gFNOS.intHeap);
        int sch_times[6];
        copy( t_ptr, t_ptr +6, sch_times );

        // save to constant variables
        const int sYear = sch_times[0];
        const int sMonth = sch_times[1];
        const int sDay = sch_times[2];
        const int sHour = sch_times[3];
        const int sMinute = sch_times[4];
        const int sSecond = sch_times[5];

        // get informations about current date and time
        std::time_t cTime = std::time(0); // get current time
        std::tm* now = std::localtime(&cTime);

        // save to constant variables
        const int cYear = (now->tm_year +1900);
        const int cMonth = (now->tm_mon +1);
        const int cDay = (now->tm_mday);
        const int cHour = (now->tm_hour);
        const int cMinute = (now->tm_min);
        const int cSecond = (now->tm_sec);

        bool isOutdated = false;

        if ( cYear > sYear ) { isOutdated = true; }
        else if ( cYear == sYear ) {
            if ( cMonth > sMonth ) { isOutdated = true; }
            else if ( cMonth == sMonth ) {
                if ( cDay > sDay ) { isOutdated = true; }
                else if ( cDay == sDay ) {
                    if ( cHour > sHour ) { isOutdated = true; }
                    else if ( cHour == sHour ) {
                        if ( cMinute > sMinute ) { isOutdated = true; }
                        else if ( cMinute == sMinute ) {
                            if ( cSecond > sSecond ) { isOutdated = true; }
                        }
                    }
                }
            }
        }

        if (isOutdated == true) { return i; } // return index of first single schedule, which is outdated

    }

    return -1; // no outdated schedule found
}

bool saveAndExit() {
    printLine();

    string userInput;

    cout << "\n* 일정을 저장하고 프로그램을 종료하시겠습니까? (y/n) : ";
    std::getline(std::cin >> std::ws, userInput); // get user input, including spaces

    if (userInput == "y" || userInput == "Y") {
        _sch.saveSchedulesToFile(); // save schedules
        return true;
    }
    else { return false; }
}

// ========== system functions .end ==========



// ======== async functions .begin ========

// function task of triggering schedule notification
int trigSchNotif(const long _schOutd)
{
    Ts_str main_aL_str; // create first object of 'Ts_str' class, to use string-type temporary vector array
    Ts_int main_aL_time; // create second object of 'Ts_int' class, to use integer-type temporary vector array

    Ts_int main_aL_time2; // create third object of 'Ts_int' class, to use integer-type temporary vector array

    const int* t_ptr = _sch.getScheduleTimes(_schOutd, main_aL_time2.intHeap); // get pointer of times data of target schedule
    const string sch_name = _sch.getScheduleName(_schOutd);
    const string sch_comment = _sch.getScheduleComment(_schOutd);

    main_aL_time.cleanElements(); // clear elements in vector
    main_aL_time.intHeap.reserve(6); // reserve memory capacity
    main_aL_time.intHeap.insert( main_aL_time.intHeap.begin(), t_ptr, t_ptr +6 ); // copy data to temp vector

    main_aL_str.cleanElements(); // clear elements in vector
    main_aL_str.strHeap.reserve(2); // reserve memory capacity
    main_aL_str.strHeap.push_back(sch_name);
    main_aL_str.strHeap.push_back(sch_comment);

    int notif_response = _winNotif.showNotifBox(main_aL_time.intHeap, main_aL_str.strHeap); // show notification window and get response

    if (notif_response == 1) {
        _sch.confirmScheduleChecked(_schOutd); // change status of schedule to 'checked'
        _sch.saveSchedulesToFile(); // auto-save schedules
    }

    return 0; // task successfully finished
}

// asynchronous loop function task; this task constantly checks and obsoletes every schedule
int asyncLoops()
{
    long sch_start_num_search = 0;

    while (true) {

        const long sch_outdated = getFirstNewOutdatedSchedule( sch_start_num_search );
        if ( sch_outdated >= 0 ) {
            sch_start_num_search = (sch_outdated +1);

            const int sch_outdated_status = _sch.getScheduleStatus(sch_outdated); // get status of outdated schedule
            if (sch_outdated_status == 1) {
                // change status of schedule to outdated and create notification task

                _sch.obsoleteSchedule(sch_outdated); // obsolete the schedule
                _sch.saveSchedulesToFile(); // auto-save schedules

                async( launch::async, trigSchNotif, sch_outdated ); // create asynchronous task of notification
            }
            else if (sch_outdated_status == 0) {
                _sch.obsoleteSchedule(sch_outdated); // obsolete the schedule
                _sch.saveSchedulesToFile(); // auto-save schedules
            }

        }

        if (mode_powerSaving == true) { Sleep(200); } // sleep for 200ms, if power-saving mode is enabled
        else { Sleep(10); } // sleep for 10ms, if power-saving mode is enabled

    }

    return 0; // task finished
}

// ======== async functions .end ========



// ========== main function .begin ==========

int main() {

    SetConsoleTitle( TEXT(program_title) ); // set title of console window

    const bool saveFileExistence = _sch.getSaveFileExistence(); // get existence of save file
    if (saveFileExistence == true) { _sch.loadSchedulesFromFile(); } // load data from save file if exists

    future <int> async_notificationTask = async(launch::async, asyncLoops); // create asynchronous endless loop task about notification

    bool endProgram = false; // if true, program will be terminated off

    while (endProgram == false) {

        clearScreen();
        printGreeting();
        printMenus();

        int selectedMenu = getMenuSelection(1, 7); // get selection of menu from user input

        switch (selectedMenu) {

        // Display the list of all schedules
        case 1 : {
            clearScreen();
            printReservedMsg();
            printLine();

            cout << "\n# " << MENU_SCHEDULES_DISPLAY << " #\n";

            printAllSchedules();

            cout << "\n\n";
            printLine();

            waitForAnyKeysToReturn();
            break;
        }



        // Add new schedule
        case 2 : {
            bool escToMenu = false;

            while (escToMenu == false) {

                clearScreen();
                printReservedMsg();
                printLine();

                cout << "\n# " << MENU_SCHEDULE_ADD << " #\n";

                printAllSchedules();
                cout << "\n\n";

                int t_process = registerNewSchedule();
                if (t_process == -1) { escToMenu = true; break; }

            }
            break;
        }



        // Remove schedule
        case 3 : {
            bool escToMenu = false;

            while (escToMenu == false) {

                clearScreen();
                printReservedMsg();
                printLine();

                cout << "\n# " << MENU_SCHEDULE_REMOVE << " #\n";

                printAllSchedules();
                cout << endl;

                int targetSchedule = selectSchedule(3);

                if (targetSchedule > 0) { removeSchedule(targetSchedule -1); }
                else if (targetSchedule == 0) { escToMenu = true; break; }
                else if (targetSchedule == -2) { removeAllSchedules(); }
                else if (targetSchedule == -5) { removeOutdatedSchedules(); }
                else if (targetSchedule == -6) { removeCheckedSchedules(); }

            }
            break;
        }



        // Reserve or de-reserve schedule
        case 4 : {
            bool escToMenu = false;

            while (escToMenu == false) {

                clearScreen();
                printReservedMsg();
                printLine();

                cout << "\n# " << MENU_SCHEDULE_RESERVE << " #\n";

                printAllSchedules();
                cout << endl;

                int targetSchedule = selectSchedule(4);

                if (targetSchedule > 0) { reserveSchedule_main(targetSchedule -1); }
                else if (targetSchedule == 0) { escToMenu = true; break; }
                else if (targetSchedule == -3) { reserveAllSchedules(); }
                else if (targetSchedule == -4) { dereserveAllSchedules(); }

            }
            break;
        }



        // Switch auto-reserve-schedule mode of the program
        case 5 : {
            // switch auto-reserve-schedule mode (reserve schedule automatically when enabled)
            if (mode_rsvSchByDef == true) { mode_rsvSchByDef = false; }
            else { mode_rsvSchByDef = true; }

            clearScreen();

            break;
        }



        // Switch power-saving mode of the program
        case 6 : {
            // switch power-saving mode (add delay to 'asyncLoops' when enabled)
            if (mode_powerSaving == true) { mode_powerSaving = false; }
            else { mode_powerSaving = true; }

            clearScreen();

            break;
        }



        // Terminate the program
        case 7 : {
            clearScreen();

            bool _endProgram = saveAndExit();
            if ( _endProgram == true ) { exit(0); } // terminate off the program
        }

        }

    }

    return 0;
}

// ========== main function .end ==========
