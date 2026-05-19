# Gilded Rose 테스트 계획서

## 1. 테스트 목표와 범위

본 테스트 계획서는 C++17 기반 Gilded Rose 구현의 핵심 비즈니스 규칙을 Google Test로 보호하고, 이후 리팩토링 및 `Conjured` 신규 요구사항 구현 시 회귀를 빠르게 탐지하는 것을 목표로 한다.

테스트 대상은 `GildedRose::updateQuality()`의 하루 경과 처리이며, `Item` 구조체와 `GildedRose::items` public 인터페이스는 요구사항상 변경하지 않는다는 전제를 둔다.

주요 검증 범위는 다음과 같다.

- 아이템별 `sellIn` 감소 규칙
- 아이템별 `quality` 증가·감소 규칙
- `quality` 하한 `0`, 상한 `50` 보장
- `Sulfuras`의 품질 `80` 및 판매 기한 불변 예외
- `Backstage passes`의 구간별 증가와 콘서트 이후 `quality == 0`
- `Conjured` 아이템의 일반 아이템 대비 2배 빠른 품질 감소
- 빈 목록, 여러 아이템 동시 처리, 이미 판매 기한이 지난 상태 등 특이 입력

## 2. 테스트 설계 원칙

### 2.1 Google Test 구조

신규 단위 테스트는 `TEST_F` 기반으로 작성한다. 공통 픽스처는 테스트 데이터 생성과 `updateQuality()` 호출을 단순화해 각 테스트가 Given-When-Then 흐름을 명확히 드러내도록 한다.

권장 픽스처 예시는 다음과 같다.

```cpp
class GildedRoseTest : public ::testing::Test {
protected:
    std::vector<Item> items;

    void AddItem(const std::string& name, int sellIn, int quality) {
        items.emplace_back(name, sellIn, quality);
    }

    void Update() {
        GildedRose app(items);
        app.updateQuality();
    }
};
```

테스트 이름은 `NormalItem_DecreasesQualityByOneBeforeSellDate`처럼 대상, 조건, 기대 결과가 드러나게 작성한다.

### 2.2 테스트 우선순위

| 우선순위 | 테스트 영역 | 목적 |
|---|---|---|
| P0 | Normal, Aged Brie, Backstage Pass, Sulfuras 핵심 규칙 | 기존 요구사항 회귀 방지 |
| P0 | `quality` 하한·상한 경계값 | 데이터 무결성 보장 |
| P0 | `sellIn == 0`, `sellIn == -1` | 판매 기한 경계 처리 검증 |
| P1 | Conjured 신규 요구사항 | 새 기능 구현 및 리팩토링 보호 |
| P1 | 여러 아이템 동시 처리 | 반복 처리 중 아이템 간 영향 없음 검증 |
| P2 | 빈 벡터, 비표준 이름, 초기 품질 이상값 | 견고성 및 특이 입력 확인 |
| P2 | 장기 시뮬레이션/승인 테스트 | 넓은 회귀 탐지 보조 |

## 3. TEST_F 기반 단위 테스트 범위

### 3.1 Normal 아이템

- 판매 기한 전에는 하루가 지나면 `sellIn`이 `1` 감소하고 `quality`가 `1` 감소한다.
- 호출 전 `sellIn == 0`이면 호출 후 `sellIn == -1`이 되고 `quality`가 총 `2` 감소한다.
- 호출 전 `sellIn == -1`이면 이미 기한이 지난 상태로 보고 `quality`가 `2` 감소한다.
- `quality == 0`이면 더 이상 감소하지 않는다.
- `quality == 1`이고 판매 기한 경과 상태여도 음수가 되지 않고 `0`에서 멈춘다.

### 3.2 Aged Brie

- 판매 기한 전에는 하루가 지나면 `sellIn`이 `1` 감소하고 `quality`가 `1` 증가한다.
- 호출 전 `sellIn == 0`이면 호출 후 `quality`가 총 `2` 증가한다.
- 호출 전 `sellIn == -1`이면 `quality`가 `2` 증가한다.
- `quality == 49`에서 증가해도 `50`을 초과하지 않는다.
- `quality == 50`이면 더 이상 증가하지 않는다.

### 3.3 Backstage passes

- `sellIn > 10`이면 `quality`가 `1` 증가한다.
- `sellIn == 10`이면 `quality`가 `2` 증가한다.
- `sellIn == 5`이면 `quality`가 `3` 증가한다.
- `quality == 49` 또는 `quality == 50` 근처에서 증가해도 `50`을 초과하지 않는다.
- 호출 전 `sellIn == 0`이면 콘서트 이후가 되어 `quality == 0`이 된다.
- 호출 전 `sellIn == -1`이면 계속 `quality == 0`을 유지한다.

### 3.4 Sulfuras

- 하루가 지나도 `sellIn`이 변경되지 않는다.
- 하루가 지나도 `quality == 80`을 유지한다.
- 호출 전 `sellIn == 0`, `sellIn == -1`이어도 `sellIn`과 `quality`가 모두 변경되지 않는다.
- 일반 품질 상한 `50` 보정 로직이 `Sulfuras`에 적용되지 않는다.

### 3.5 Conjured

- 판매 기한 전에는 하루가 지나면 `quality`가 `2` 감소한다.
- 호출 전 `sellIn == 0`이면 호출 후 `quality`가 총 `4` 감소한다.
- 호출 전 `sellIn == -1`이면 `quality`가 `4` 감소한다.
- `quality == 1` 또는 `quality == 0`에서 감소해도 음수가 되지 않는다.
- 이름 매칭 정책을 테스트로 고정한다. 권장 정책은 `"Conjured Mana Cake"`처럼 `"Conjured"` 접두어를 가진 아이템을 Conjured 계열로 처리하는 것이다.

### 3.6 복합 시나리오

- 여러 아이템을 하나의 `std::vector<Item>`에 넣고 한 번에 갱신해도 각 아이템이 독립적으로 자신의 규칙을 따른다.
- 빈 `items` 벡터를 전달해도 예외 없이 종료된다.
- 동일 이름의 아이템이 여러 개 있어도 각각 독립적으로 갱신된다.

## 4. 경계값 케이스 목록

아래 경계값은 모든 관련 아이템 타입에서 우선 검증한다.

### 4.1 `quality` 경계값

| 초기 quality | 주요 검증 대상 | 기대 결과 |
|---:|---|---|
| `0` | Normal, Conjured, Backstage after concert | 음수로 내려가지 않는다. Backstage는 콘서트 이후 `0` 유지 |
| `1` | Normal, Conjured | 감소량이 초기값보다 커도 `0`에서 멈춘다 |
| `49` | Aged Brie, Backstage Pass | 증가 후 `50`을 초과하지 않는다 |
| `50` | Aged Brie, Backstage Pass | 더 이상 증가하지 않는다 |
| `80` | Sulfuras | 전설 아이템 예외로 변경되지 않는다 |

### 4.2 `sellIn` 경계값

| 초기 sellIn | 주요 검증 대상 | 기대 결과 |
|---:|---|---|
| `0` | 전체 아이템 타입 | 호출 후 일반 아이템은 기한 경과 규칙 적용. Backstage는 `quality == 0`. Sulfuras는 불변 |
| `-1` | 전체 아이템 타입 | 이미 기한이 지난 상태로 처리. 음수 `sellIn`은 오류가 아니다 |
| `5` | Backstage Pass | `quality`가 최대 `3` 증가하되 `50` 초과 금지 |
| `10` | Backstage Pass | `quality`가 최대 `2` 증가하되 `50` 초과 금지 |
| `11` | Backstage Pass | `quality`가 `1` 증가 |

### 4.3 우선 테스트 조합

| 아이템 타입 | sellIn | quality | 기대 품질 변화 |
|---|---:|---:|---:|
| Normal | `1` | `1` | `1 -> 0` |
| Normal | `0` | `1` | `1 -> 0` |
| Normal | `-1` | `1` | `1 -> 0` |
| Normal | `0` | `50` | `50 -> 48` |
| Aged Brie | `1` | `49` | `49 -> 50` |
| Aged Brie | `0` | `49` | `49 -> 50` |
| Aged Brie | `-1` | `49` | `49 -> 50` |
| Backstage Pass | `11` | `49` | `49 -> 50` |
| Backstage Pass | `10` | `49` | `49 -> 50` |
| Backstage Pass | `5` | `49` | `49 -> 50` |
| Backstage Pass | `0` | `49` | `49 -> 0` |
| Sulfuras | `0` | `80` | `80 -> 80` |
| Sulfuras | `-1` | `80` | `80 -> 80` |
| Conjured | `1` | `1` | `1 -> 0` |
| Conjured | `0` | `1` | `1 -> 0` |
| Conjured | `-1` | `1` | `1 -> 0` |
| Conjured | `0` | `50` | `50 -> 46` |

## 5. 예외 및 특이 케이스 목록

### 5.1 도메인 예외

- `Sulfuras`는 일반 품질 상한 `50`의 예외이며 `quality == 80`을 유지한다.
- `Sulfuras`는 `sellIn`도 감소하지 않는다.
- `Backstage passes`는 콘서트가 지난 뒤 품질이 감소하는 것이 아니라 즉시 `0`이 된다.
- `Aged Brie`는 일반 아이템과 달리 오래될수록 품질이 증가한다.
- `Conjured`는 일반 아이템보다 2배 빠르게 품질이 감소한다.

### 5.2 입력 특이 케이스

- 빈 `std::vector<Item>` 입력
- `sellIn`이 이미 음수인 아이템
- `quality == 0`인 감소형 아이템
- `quality == 50`인 증가형 아이템
- `Sulfuras`의 `quality == 80`
- 동일한 이름의 아이템 여러 개
- 특별 아이템 이름과 비슷하지만 정확히 일치하지 않는 이름
- `Conjured` 접두어 정책을 벗어나는 이름

### 5.3 현재 구현 대비 주의할 위험

- 현재 `GildedRose.cpp`는 `Conjured` 규칙을 별도로 처리하지 않는다. Conjured 테스트는 신규 요구사항 구현 전에는 실패하는 것이 정상이다.
- 현재 CMake는 `test/GildedRoseTest.cpp`만 테스트 실행 파일에 연결한다. 테스트 파일을 분리할 경우 `CMakeLists.txt`에 추가 등록이 필요하다.
- 레거시 카타 기본 테스트인 `Foo` 스타일 테스트는 의도적으로 실패하도록 작성된 예시일 수 있으므로 실제 회귀 테스트로 교체해야 한다.
- 승인 테스트와 단위 테스트를 함께 운용할 경우, 승인 파일은 넓은 회귀 탐지용으로 두고 비즈니스 규칙의 기대값은 `TEST_F` 단위 테스트에서 명시적으로 검증한다.

## 6. 커버리지 목표

### 6.1 목표 수치

- 라인 커버리지: `90%` 이상
- 함수 커버리지: `95%` 이상
- 브랜치 커버리지: `85%` 이상
- `GildedRose::updateQuality()` 핵심 분기 커버리지: 가능한 한 `100%`

라인 커버리지보다 브랜치 커버리지를 더 중요하게 본다. `updateQuality()`는 조건 분기가 많기 때문에 라인은 실행되더라도 특정 아이템 타입이나 경계 조건이 검증되지 않을 수 있다.

### 6.2 gcov/lcov 측정 전략

GCC 또는 Clang의 gcov 호환 옵션을 사용해 커버리지를 측정한다. 권장 빌드 디렉터리는 `cpp/build-coverage`이다.

```bash
cd cpp
cmake -S . -B build-coverage -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="--coverage -O0 -g" \
  -DCMAKE_EXE_LINKER_FLAGS="--coverage"
cmake --build build-coverage
ctest --test-dir build-coverage --output-on-failure
lcov --capture --directory build-coverage --output-file build-coverage/coverage.info
lcov --remove build-coverage/coverage.info '/usr/*' '*/_deps/*' '*/test/*' --output-file build-coverage/coverage.filtered.info
genhtml build-coverage/coverage.filtered.info --output-directory build-coverage/coverage-report
```

Windows 환경에서는 MinGW, MSYS2, WSL 중 실제 사용하는 GCC 계열 도구체인에 맞춰 실행한다. Visual Studio/MSVC만 사용하는 경우에는 OpenCppCoverage 같은 대체 도구를 별도로 검토한다.

### 6.3 커버리지 개선 전략

1. 먼저 P0 테스트를 작성해 기존 요구사항의 핵심 분기를 보호한다.
2. `lcov --summary` 또는 HTML 리포트에서 `GildedRose.cpp`의 미실행 라인과 미충족 브랜치를 확인한다.
3. 미충족 분기를 아이템 타입별로 분류한다.
   - Normal 감소 분기
   - Aged Brie 증가 및 상한 분기
   - Backstage Pass `>10`, `<=10`, `<=5`, 콘서트 이후 분기
   - Sulfuras 불변 분기
   - Conjured 감소 및 하한 분기
4. 커버리지 수치를 올리기 위한 무의미한 테스트보다, 경계값과 요구사항 문장에 직접 대응되는 테스트를 우선 추가한다.
5. 리팩토링 후에는 동일한 테스트 스위트와 커버리지 측정을 반복해 커버리지 하락 및 동작 회귀를 확인한다.

## 7. 실행 및 완료 기준

### 7.1 기본 테스트 실행

```bash
cd cpp
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

### 7.2 완료 기준

- P0 테스트가 모두 Green이다.
- Conjured 구현 이후 P1 테스트가 모두 Green이다.
- `Item` 구조체와 `GildedRose::items` public 인터페이스가 변경되지 않았다.
- `GildedRose.cpp` 기준 라인 커버리지 `90%+`, 브랜치 커버리지 `85%+`를 달성했다.
- 커버리지 리포트에서 핵심 도메인 분기가 테스트 없이 남아 있지 않다.
- 리팩토링 전후 동일 테스트 스위트가 Green 상태를 유지한다.

