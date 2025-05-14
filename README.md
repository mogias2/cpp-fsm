# cpp-fsm
1. 개요
- Game Programming Gems 를 참고하여 FSM 을 구현.
- 상태 정의 부분과 구현 부분을 분리하여 어떤 객체든 범용적으로 활용 가능.
- tuple을 사용하여 진입 함수에 파라미터 전송 가능 (파라미터는 진입함수에서만 사용 가능)
- 적용하면 상태 관련 버그에 자유로워짐.

2. 빌드 환경
- Microsoft Visual Studio Community 2022 Version 17.14.0

3. 디렉토리 구조
- bin : 실행 파일 (fsm.exe)
- bin/json : transition과 action 데이터 (obj state, input enum 범위)
- contents : object와 json data 처리
- fsm : fsm 관련 소스파일
- rapidjson-1.0.2: json 처리하기 위한 외부 소스 (SILENCE_ALL_CXX17_DEPRECATION_WARNINGS 매크로 정의 필요)

4. 데이터 설정
- 상태는 3가지 행동 양식(Enter, Update, Exit) 을 가지고, 각각의 행동 양식에 적합한 내용을 기술.
- 특정 입력의 전이에 대한 규칙을 정함.
- json 데이터 입력

5. 결과
- json data에 따라 상태 진입 내용, 입력값, 상태 종료 내용 출력 (상태 전이 후 1초 딜레이)
- idle 상태일 때만 진입시 파라미터 출력되도록 설정 되어 있음.

* 하나의 객체에 대해서만 간단한 상태전이 테스트 출력 용
* 객체의 update 함수에서 즉시 순차적으로 상태 전이가 발생하도록 되어 있음. (실제 사용시에는 적절하게 상태 전이가 이루어지도록 해야 함)
* json 데이터가 코드상의 enum에 의존적이기 때문에 입력시 확인이 필요 (잘못된 값은 에러 출력 후 무시)
* json 데이터 path가 상대경로로 정의 되어 있기 때문에 bin 디렉토리는 그대로 사용.