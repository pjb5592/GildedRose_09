# Gilded Rose

Gilded Rose 는 게임 "World of Warcraft"에 나오는 여관 이름입니다.  
아이템은 퀄리티 속성을 가지고 있으며, 예외 사항이 있는 아이템들이 있습니다.  
- 아이템은 [아이템 이름, 퀄리티, 유통기한]의 속성을 가집니다.  
- 아이템의 퀄리티는 0 이상이고 아이템의 퀄리티는 하루가 지날 때마다 1씩 줄어듭니다.  
- 유통 기한이 지난 아이템의 퀄리티는 2배의 속도로 떨어집니다.  
- 퀄리티는 최대값이 50입니다.  

### Aged Brie, Backstage Pass, Sulfuras 의 예외적 규칙  

- Aged Brie는 하루가 지날 때마다 퀄리티가 1씩 증가합니다.  
- 유통기한이 지나면 퀄리티가 2씩 증가합니다.  
- Backstage Pass 는 유통기한(콘서트일)이 다가올수록 퀄리티가 증가합니다.  
  유통기한이 11일 이상일 때는 1, 10일 이하일 때는 2, 5일 이하일 때는 3씩 증가하지만  
  콘서트 날이 지나면 퀄리티는 0이 됩니다.  
- Sulfuras의 퀄리티는 변화가 없습니다.  
  
---------------------------------------------------------  
## 실습 목적 :  테스트 코드 작성을 통한 legacy code 이해

### 프로젝트 구조 (C++)

```
cpp/
├── CMakeLists.txt          # CMake 빌드 설정 (Google Test 자동 다운로드)
├── include/
│   ├── Item.h              # Item 클래스 (수정 불가)
│   └── GildedRose.h        # GildedRose 클래스 헤더
├── src/
│   └── GildedRose.cpp      # updateQuality() 구현
└── test/
    └── GildedRoseTest.cpp  # Google Test 기반 테스트
```

### 빌드 및 테스트 실행

```bash
cd cpp
cmake -B build
cmake --build build
ctest --test-dir build
```

### GildedRoseTest.cpp : failed test 수정
- updateQuality() 의 test case를 추가 작성해 보세요.
- unit test 내용 : 문서상에 나타난 동작들 확인

### unit test VS golden-master test 비교


### To-Do List
- TC-01: 일반 아이템 품질 하한
- TC-02: 기한 지난 일반 아이템
- TC-03: 전설 아이템은 변하지 않음
- TC-04: 기한 마감 후에도 변하진 않음
- TC-05: 기한 지난 Aged Brie는 품질 +2
- TC-06: 품질 상한 테스트
- TC-07: Backstage: 10일 초과 시 +1
- TC-08: 공연 지난 후 품질 0
- TC-09: 품질 50 초과 후 공연 종료
- TC-10: 아이템이 없을 경우 처리
- 문제점
  - updateQuality 함수가 너무 길고 많은 역할을 가지고 있음(sellIn, quality 등) - 긴 함수, SRP 위반
  - if 구문 중첩 - 
  - 하드 코딩된 item명
  - 로직 중복 - 중복 코드, 산탄총 수술
  - GildedRose가 Item 객체의 data에 직접 입출력 - 전역 데이터, 가변 데이터, 내부자 거래
  - GildedRose가 Item 객체의 함수를 과도하게 사용 - 기능 편애, 상속 편의
- 추가 기능
  - Food&Beverage: NormalItem의 2배 Quality 감소. 기한 지나면 추가 2배(총 4배)
    - Test Case:
      1. "[F&B] Bread", 5, 20 → 4, 18
      2. "[F&B] Milk", 0, 20 →  16
      3. "[F&B] Water", 0, 1 → 0