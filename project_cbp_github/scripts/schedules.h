// schedules.h

// Header file which contains schedule class of Console Scheduler

#include <string> // Allows the string type data
#include <vector> // Allows dynamic arrays, easy to manage, take advantages at preventing memory leakage
#include <fstream> // Allows to read and write files
#include <filesystem> // Allows to create directory

// Define path & name of save files
#define PATH_OUTPUT_SAVES ".\\data_saves\\"
#define FILENAME_DATA_SCHEDULES "sch_data.dat"



// Class of schedules data
class Schedules {

    private :
        // Schedule Arrays
        std::vector <std::string> sch_name; // Array which contains Name of schedules
        std::vector <int> sch_year; // Array which contains Year of schedules
        std::vector <int> sch_month; // Array which contains Month of schedules
        std::vector <int> sch_day; // Array which contains Day of schedules
        std::vector <int> sch_hour; // Array which contains Hour of schedules
        std::vector <int> sch_minute; // Array which contains Minute of schedules
        std::vector <int> sch_second; // Array which contains Second of schedules
        std::vector <std::string> sch_comment; // Array which contains Comment of schedules

        /*
        Vector array 'sch_status' saves current status of schedule.

        0 stands for *default*, which means schedule is not reserved, schedule is not outdated.
        1 stands for *reserved*, which means schedule is reserved to get an alert, schedule is not outdated.
        2 stands for *outdated*, which means schedule is outdated.
        3 stands for *checked*, which means schedule is outdated and user checked the alert.
        */
        std::vector <int> sch_status;



    public :
        // Function which gets amount of whole schedules
        const long getSchedulesAmount() {
            return sch_name.size();
        }

        // Function which returns name of specific single schedule
        const std::string getScheduleName(const long schArrNum)
        {
            if ( sch_name.empty() == false ) {
                return sch_name[schArrNum];
            }
        }

        // Function which returns pointer of times array of specific single schedule
        const int* getScheduleTimes(const long schArrNum, std::vector<int> t_intHeap)
        {
            if ( sch_year.empty() ) {
                t_intHeap.clear(); // clean vector elements (capacity still remains)
                t_intHeap.assign(6, 0);

                const int* t_ptr = t_intHeap.data(); // create and set pointer to point first element of temp var

                return t_ptr;
            }
            else {
                t_intHeap.clear(); // clean vector elements (capacity still remains)
                t_intHeap.reserve(6); // reserve memory heaps

                // save times data
                t_intHeap.push_back( sch_year[schArrNum] ); // Year of schedule
                t_intHeap.push_back( sch_month[schArrNum] ); // Month of schedule
                t_intHeap.push_back( sch_day[schArrNum] ); // Day of schedule
                t_intHeap.push_back( sch_hour[schArrNum] ); // Hour of schedule
                t_intHeap.push_back( sch_minute[schArrNum] ); // Minute of schedule
                t_intHeap.push_back( sch_second[schArrNum] ); // Second of schedule

                const int* t_ptr = t_intHeap.data(); // create and set pointer to point first element of temp var

                return t_ptr;
            }
        }

        // Function which returns comment of specific single schedule
        const std::string getScheduleComment(const long schArrNum) {
            return sch_comment[schArrNum];
        }

        // Function which returns integer of specific single schedule, which stands the status of schedule
        const int getScheduleStatus(const long schArrNum) {
            return sch_status[schArrNum];
        }

        // Function which gets existence of data save file
        const bool getSaveFileExistence()
        {
            const std::string t_filepath = std::string(PATH_OUTPUT_SAVES) + std::string(FILENAME_DATA_SCHEDULES); // define file path
            return std::ifstream(t_filepath).good(); // return the existence of file
        }

        // Function which saves schedule data to single save file
        int saveSchedulesToFile()
        {
            std::filesystem::path savePath( std::string(PATH_OUTPUT_SAVES) ); // create path object to create directory
            std::filesystem::create_directories(savePath); // create directory

            const std::string saveFilePathStr = std::string(PATH_OUTPUT_SAVES) + std::string(FILENAME_DATA_SCHEDULES); // set file path of target savefile to string
            std::ofstream savefile( saveFilePathStr, std::ofstream::binary );

            const long schAmt = sch_name.size(); // get length of vector array, which contains name of schedules (this is necessary when read or write time and status data of savefile)

            savefile.write( reinterpret_cast<const char*>(&schAmt), sizeof(schAmt) ); // write the amount of schedules; its size must me same as size of longtype(4 bytes)

            // write date, time, status data of schedules
            savefile.write( reinterpret_cast<const char*>(sch_year.data()), (sizeof(int) * schAmt) ); // write year data of schedules
            savefile.write( reinterpret_cast<const char*>(sch_month.data()), (sizeof(int) * schAmt) ); // write month data of schedules
            savefile.write( reinterpret_cast<const char*>(sch_day.data()), (sizeof(int) * schAmt) ); // write day data of schedules
            savefile.write( reinterpret_cast<const char*>(sch_hour.data()), (sizeof(int) * schAmt) ); // write hour data of schedules
            savefile.write( reinterpret_cast<const char*>(sch_minute.data()), (sizeof(int) * schAmt) ); // write minute data of schedules
            savefile.write( reinterpret_cast<const char*>(sch_second.data()), (sizeof(int) * schAmt) ); // write second data of schedules
            savefile.write( reinterpret_cast<const char*>(sch_status.data()), (sizeof(int) * schAmt) ); // write status data of schedules

            // write dynamic string values such as name or comment of schedules
            for (int i=0; i<schAmt; i++) {
                const std::string targetString = sch_name[i];
                const size_t targetStrSize = targetString.size();

                savefile.write( reinterpret_cast<const char*>(&targetStrSize), sizeof(targetStrSize) );
                savefile.write( targetString.c_str(), targetStrSize );
            }
            for (int i=0; i<schAmt; i++) {
                const std::string targetString = sch_comment[i];
                const size_t targetStrSize = targetString.size();

                savefile.write( reinterpret_cast<const char*>(&targetStrSize), sizeof(targetStrSize) );
                savefile.write( targetString.c_str(), targetStrSize );
            }

            savefile.close(); // close save file

            return 0; // end function successfully
        }

        // Function which loads schedule data from save file
        int loadSchedulesFromFile()
        {
            const std::string saveFilePathStr = std::string(PATH_OUTPUT_SAVES) + std::string(FILENAME_DATA_SCHEDULES); // set file path of target savefile to string
            std::ifstream savefile( saveFilePathStr, std::ifstream::binary );

            long tmp_long;
            savefile.read( reinterpret_cast<char*>(&tmp_long), sizeof(long) );
            const long schAmtToRead = tmp_long;

            clearSchedules(); // clear schedules data

            // resize vectors to save data
            sch_year.resize(schAmtToRead);
            sch_month.resize(schAmtToRead);
            sch_day.resize(schAmtToRead);
            sch_hour.resize(schAmtToRead);
            sch_minute.resize(schAmtToRead);
            sch_second.resize(schAmtToRead);
            sch_status.resize(schAmtToRead);

            // read date, time, status data of schedules
            savefile.read( reinterpret_cast<char*>(sch_year.data()), (sizeof(int) * schAmtToRead) );
            savefile.read( reinterpret_cast<char*>(sch_month.data()), (sizeof(int) * schAmtToRead) );
            savefile.read( reinterpret_cast<char*>(sch_day.data()), (sizeof(int) * schAmtToRead) );
            savefile.read( reinterpret_cast<char*>(sch_hour.data()), (sizeof(int) * schAmtToRead) );
            savefile.read( reinterpret_cast<char*>(sch_minute.data()), (sizeof(int) * schAmtToRead) );
            savefile.read( reinterpret_cast<char*>(sch_second.data()), (sizeof(int) * schAmtToRead) );
            savefile.read( reinterpret_cast<char*>(sch_status.data()), (sizeof(int) * schAmtToRead) );

            // resize vectors to save data
            sch_name.resize(schAmtToRead);
            sch_comment.resize(schAmtToRead);

            // read dynamic string values such as name or comment of schedules
            for (int i=0; i<schAmtToRead; i++) {
                std::string tmpString; // create string-typed object
                size_t tmpStrSize;

                savefile.read( reinterpret_cast<char*>(&tmpStrSize), sizeof(tmpStrSize) );
                tmpString.resize(tmpStrSize);
                savefile.read( &tmpString[0], tmpStrSize );

                sch_name[i] = tmpString; // copy temporary string to target vector container
            }
            for (int i=0; i<schAmtToRead; i++) {
                std::string tmpString; // create string-typed object
                size_t tmpStrSize;

                savefile.read( reinterpret_cast<char*>(&tmpStrSize), sizeof(tmpStrSize) );
                tmpString.resize(tmpStrSize);
                savefile.read( &tmpString[0], tmpStrSize );

                sch_comment[i] = tmpString; // copy temporary string to target vector container
            }

            savefile.close(); // close save file

            return 0; // end function successfully
        }

        // Function which adds new single schedule
        int addSchedule(std::string s_name, int s_year, int s_month, int s_day, int s_hour, int s_minute, int s_second, std::string s_comment, bool rsvSchAlert=false)
        {
            // add schedule data to vectors
            sch_name.push_back(s_name);
            sch_year.push_back(s_year);
            sch_month.push_back(s_month);
            sch_day.push_back(s_day);
            sch_hour.push_back(s_hour);
            sch_minute.push_back(s_minute);
            sch_second.push_back(s_second);
            sch_comment.push_back(s_comment);

            if (rsvSchAlert) { sch_status.push_back(1); }
            else { sch_status.push_back(0); }

            saveSchedulesToFile(); // save schedules to file

            return 0; // end function successfully
        }

        // Function which edits name of single specific schedule
        int editScheduleName(const long schArrNum, std::string s_name)
        {
            sch_name[schArrNum] = s_name;
            return 0; // end function successfully
        }

        // Function which edits times data of single specific schedule
        int editScheduleTimes(const long schArrNum, int s_year, int s_month, int s_day, int s_hour, int s_minute, int s_second)
        {
            // replace schedule data in vectors
            sch_year[schArrNum] = s_year;
            sch_month[schArrNum] = s_month;
            sch_day[schArrNum] = s_day;
            sch_hour[schArrNum] = s_hour;
            sch_minute[schArrNum] = s_minute;
            sch_second[schArrNum] = s_second;

            return 0; // end function successfully
        }

        // Function which edits single specific schedule
        int editScheduleComment(const long schArrNum, std::string s_comment)
        {
            sch_comment[schArrNum] = s_comment;
            return 0; // end function successfully
        }

        // Function which reserves single specific schedule, to get an alarm
        int reserveSchedule(const long schArrNum)
        {
            sch_status[schArrNum] = 1;
            return 0; // end function successfully
        }

        // Function which de-reserves single specific schedule, not to get an alarm
        int dereserveSchedule(const long schArrNum)
        {
            sch_status[schArrNum] = 0;
            return 0; // end function successfully
        }

        // Function which obsolete single specific schedule, not to get an alarm
        int obsoleteSchedule(const long schArrNum)
        {
            sch_status[schArrNum] = 2;
            return 0; // end function successfully
        }

        // Function which change status of single specific schedule to checked, not to get an alarm
        int confirmScheduleChecked(const long schArrNum)
        {
            sch_status[schArrNum] = 3;
            return 0; // end function successfully
        }

        // Functions that check validity of time or date
        const bool isLeapYear(int t_year) const {
            // return true if year is a multiple of 4 and not multiple of 100, or year is multiple of 400.
            return ( ((t_year % 4 == 0) && (t_year % 100 != 0)) || (t_year % 400 == 0) );
        }

        const bool isValidDate(const int t_year, const int t_month, const int t_day)
        {
            const int MAX_YEAR_VALID = 9999;
            const int MIN_YEAR_VALID = 1800;

            // if year, month and day are not in given range
            if ( (t_year > MAX_YEAR_VALID) || (t_year < MIN_YEAR_VALID) ) { return false; }
            if ( (t_month < 1) || (t_month > 12) ) { return false; }
            if ( (t_day < 1) || (t_day > 31) ) { return false; }

            // handle February month with leap year
            if (t_month == 2) {
                if ( isLeapYear(t_year) ) { return (t_day <= 29); }
                else { return (t_day <= 28); }
            }

            // months of April, June, Sept and Nov must have number of days less than or equal to 30.
            if ( (t_month == 4) || (t_month == 6) || (t_month == 9) || (t_month == 11) ) { return (t_day <= 30); }

            return true; // Passed all
        }

        const bool isValidTime(int t_hour, int t_minute, int t_second) const
        {
            // handle the hour, minute, second
            if ( (t_hour < 0) || (23 < t_hour) ) { return false; } // Check hour in range
            if ( (t_minute < 0) || (59 < t_minute) ) { return false; } // Check minute in range
            if ( (t_second < 0) || (59 < t_second) ) { return false; } // Check second in range

            return true; // Passed all
        }



        // Function which clears whole schedule
        int clearSchedules()
        {
            sch_name.clear();
            sch_year.clear();
            sch_month.clear();
            sch_day.clear();
            sch_hour.clear();
            sch_minute.clear();
            sch_second.clear();
            sch_comment.clear();
            sch_status.clear();

            return 0; // end function successfully
        }

        // Function which removes single schedule
        int removeSchedule(const long schArrNum)
        {
            // remove schedule data in vector (this does not takes heaped memory back to computer, removes only element)
            sch_name.erase( sch_name.begin() + schArrNum );
            sch_year.erase( sch_year.begin() + schArrNum );
            sch_month.erase( sch_month.begin() + schArrNum );
            sch_day.erase( sch_day.begin() + schArrNum );
            sch_hour.erase( sch_hour.begin() + schArrNum );
            sch_minute.erase( sch_minute.begin() + schArrNum );
            sch_second.erase( sch_second.begin() + schArrNum );
            sch_comment.erase( sch_comment.begin() + schArrNum );
            sch_status.erase( sch_status.begin() + schArrNum );

            saveSchedulesToFile(); // save schedules to file

            return 0; // end function successfully
        }

};
