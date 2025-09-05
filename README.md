언리얼 엔진 C++로 제작한 네트워크 채팅 프로그램입니다.  
메인 화면에 실시간 타이머, 턴 진행 상황, 게임 결과 등을 UI로 표시하고,  
RPC를 활용해 서버-클라이언트 간 메시지와 상태가 동기화되도록 구현했습니다.


🎮 JSLee Number Baseball Game

📌 필수 기능

*GameMode 서버 로직 구현

-정답 판정 / 게임 초기화 등 주요 게임 로직을 서버(GameMode)에서 처리

*난수 생성 로직 구현

-GenerateRandomNumber() 함수를 통해 고유한 3자리 난수 생성

*판정 로직

-GameMode에서 입력받은 숫자를 기반으로 스트라이크 / 볼 판정

-결과를 UI에 출력

*시도 횟수 및 상태 관리

-RecentTurn / MaxTurn 으로 플레이어의 시도 횟수 관리 및 화면에 표시

*승리, 무승부, 게임 리셋

-특정 조건(예: 3 Strike 또는 최대 턴 초과) 시 게임을 초기화

*C++ 라이브러리 작성

-UJSLeeTalkFunctionLibrary 클래스로 공용 함수 제공

-GameMode 등에서 호출 가능

🚀 도전 기능

*턴 제어 기능

-GameMode에서 FTimerHandle을 사용해 턴 시간 제어

-시간이 끝나면 자동으로 턴 카운트 증가

-UI 타이머 표시

-현재 시간 / 최대 시간을 UI에 표시

