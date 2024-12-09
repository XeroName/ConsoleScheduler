// msg_formats.h

// Header file which defines output messages and its formats

// information messages
#define MSG_KEYNPUT_ANYKEY_BACKTO_MENU "메뉴로 돌아가려면 아무 키나 누르세요."
#define MSG_SCHEDULE_CHOOSE "위 목록에 나열된 일정 중 한 가지를 골라 번호를 입력하세요."
#define MSG_NOTICE_REMOVEALL "_rmall 을 입력해 모든 일정을 삭제할 수 있습니다."
#define MSG_NOTICE_REMOVE_OUTDATED "_rmotd 를 입력해, 날짜가 만료된 모든 일정을 삭제할 수 있습니다."
#define MSG_NOTICE_REMOVE_CHECKED "_rmchk 를 입력해, 알림을 확인한 모든 일정을 삭제할 수 있습니다."
#define MSG_NOTICE_RESERVEALL "_rsall 을 입력해 모든 일정을 예약할 수 있습니다."
#define MSG_NOTICE_DERESERVEALL "_drsall 을 입력해 모든 일정을 예약 해제할 수 있습니다."
#define MSG_NOTICE_BACKTO_MENU "_menu 를 입력해 메뉴로 돌아갈 수 있습니다."
#define MSG_NOTICE_BACKTO_PREV "_back 을 입력해 이전으로 돌아갈 수 있습니다."
#define MSG_NOTICE_BACKTO_PREVOR_MENU "_back 을 입력해 이전으로 돌아가거나, _menu 를 입력해 메뉴로 돌아갈 수 있습니다."
#define MSG_SCHEDULE_EMPTY "등록된 일정이 없습니다."
#define MSG_SCHEDULE_ADDED "일정이 추가되었습니다."
#define MSG_SCHEDULE_REMOVED "일정이 삭제되었습니다."
#define MSG_SCHEDULE_REMOVE_CANCELED "일정 삭제가 취소되었습니다."
#define MSG_SCHEDULE_RESERVED "일정에 대한 알림이 예약되었습니다."
#define MSG_SCHEDULE_DERESERVED "일정에 대한 알림이 예약 해제되었습니다."

// error messages
#define MSG_ERR_INPUT_NONINT "숫자로 된 올바른 항목 값을 입력해 주세요."
#define MSG_ERR_INPUT_OUTOF_RANGE "입력 값이 너무 큽니다."
#define MSG_ERR_INPUT_LENGTH_INVALID "입력 형식의 길이가 잘못되었습니다."
#define MSG_ERR_DATE_INVALID "유효하지 않은 날짜입니다."
#define MSG_ERR_TIME_INVALID "유효하지 않은 시간입니다."
#define MSG_ERR_SCHEDULE_OUTDATED_NOTRESERVED "만료된 일정은 예약할 수 없습니다."

// system displays
#define MENU_SCHEDULES_DISPLAY "등록된 일정 목록 보기"
#define MENU_SCHEDULE_ADD "일정 추가하기"
#define MENU_SCHEDULE_REMOVE "일정 삭제하기"
#define MENU_SCHEDULE_RESERVE "알림 예약/해제하기"
#define MENU_MODE_SCHEDULE_RESERVE_DEFAULT "일정 생성 시 알림 자동 예약"
#define MENU_MODE_POWERSAVING "프로그램 절전 모드"
#define MENU_PROGRAM_TERMINATE "프로그램 종료"
#define DISPLAY_SCHEDULE_NEW "신규 일정"
#define DISPLAY_SCHEDULE_NONAME "이름 없는 일정"
#define DISPLAY_SCHEDULE_RESERVED "알림 예약됨"
#define DISPLAY_SCHEDULE_OUTDATED "만료됨"
#define DISPLAY_SCHEDULE_CHECKED "확인됨"
#define DISPLAY_ON "켜짐"
#define DISPLAY_OFF "꺼짐"
