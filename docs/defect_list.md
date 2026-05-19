# Gilded Rose 결함 목록

작성일: 2026-05-19

본 문서는 현재까지 QA 및 테스트 구현 과정에서 발견된 테스트 실패와 결함을 정리한다. 항목 형식은 `[ID] [Severity] [ItemType] [Steps] [Expected] [Actual] [Root Cause] [Fix Summary]`를 따른다.

## 결함 목록

### [DEF-001] [Major] [Test]

**[Steps]**

1. 기존 `cpp/test/GildedRoseTest.cpp`의 기본 테스트를 유지한 상태에서 빌드한다.
2. `ctest` 또는 `ctest --test-dir build --output-on-failure`를 실행한다.

**[Expected]**

요구사항을 검증하는 의미 있는 테스트가 실행되고, 실제 도메인 규칙에 맞는 기대값을 검증해야 한다.

**[Actual]**

`GildedRoseTest.Foo` 테스트가 `"foo"` 아이템 이름에 대해 `"fixme"`를 기대하여 실패한다.

**[Root Cause]**

프로젝트 초기 상태의 placeholder 테스트가 제거되지 않았다. 해당 테스트는 Gilded Rose 도메인 규칙을 검증하지 않고, 의도적으로 잘못된 기대값을 사용한다.

**[Fix Summary]**

placeholder 테스트를 제거하고 `GildedRoseItemRulesTest` 픽스처 기반의 명세 테스트로 교체했다. Normal, Aged Brie, Backstage Pass, Sulfuras, Conjured 아이템에 대해 총 27개 테스트를 작성했다.

### [DEF-002] [Major] [Conjured]

**[Steps]**

1. `Conjured Mana Cake` 아이템을 `sellIn = 10`, `quality = 20`으로 생성한다.
2. `GildedRose::updateQuality()`를 1회 호출한다.
3. 호출 후 `sellIn`과 `quality`를 확인한다.

**[Expected]**

`sellIn`은 `9`가 되고, `quality`는 일반 아이템의 2배 속도로 감소하여 `18`이 되어야 한다.

**[Actual]**

초기 구현에서는 `Conjured` 전용 분기가 없어 일반 아이템 규칙으로 처리될 수 있었다. 이 경우 `quality`가 `19`가 되어 요구사항의 2배 감소 규칙을 만족하지 못한다.

**[Root Cause]**

신규 요구사항인 `Conjured` 아이템 규칙이 `updateQuality()`에 구현되어 있지 않았다. 아이템 이름 매칭 정책도 테스트로 고정되어 있지 않았다.

**[Fix Summary]**

`Conjured` 접두어를 판별하는 `isConjured()` 헬퍼를 추가하고, 판매 기한 전에는 `quality`를 2 감소, 판매 기한 경과 후에는 4 감소하도록 구현했다. 품질 하한은 `decreaseQuality()`에서 `0`으로 보정한다.

### [DEF-003] [Major] [Conjured]

**[Steps]**

1. `Conjured Mana Cake` 아이템을 `sellIn = 0`, `quality = 20`으로 생성한다.
2. `GildedRose::updateQuality()`를 1회 호출한다.
3. 호출 후 `sellIn`과 `quality`를 확인한다.

**[Expected]**

호출 전 `sellIn == 0`인 Conjured 아이템은 하루가 지난 뒤 판매 기한 경과 규칙이 적용되어 `sellIn = -1`, `quality = 16`이 되어야 한다.

**[Actual]**

초기 구현에서는 Conjured 전용 기한 경과 규칙이 없어 일반 아이템처럼 `quality`가 `18`까지 감소하는 수준에 그칠 수 있었다.

**[Root Cause]**

Conjured 아이템의 판매 기한 경과 후 감소량이 일반 아이템의 2배, 즉 하루 총 `-4`라는 요구사항이 구현에 반영되지 않았다.

**[Fix Summary]**

`originalSellIn <= 0` 조건에서 Conjured 아이템의 감소량을 `4`로 적용했다. `sellIn == 0`과 `sellIn == -1` 경계값 테스트를 추가해 회귀를 방지했다.

### [DEF-004] [Minor] [TestEnvironment]

**[Steps]**

1. 저장소 루트에서 테스트 실행 방법을 확인한다.
2. VS Code 또는 터미널에서 CTest 실행을 시도한다.

**[Expected]**

프로젝트의 CMake 소스 디렉터리와 빌드 디렉터리가 명확히 지정되어 VS Code에서도 동일한 테스트를 실행할 수 있어야 한다.

**[Actual]**

워크스페이스 `settings.json`이 없어 VS Code CMake Tools가 사용할 소스 디렉터리와 빌드 디렉터리가 명시되어 있지 않았다. 또한 저장소 루트에서 단순 `ctest`만 실행하면 CTest 설정 파일을 찾지 못할 수 있다.

**[Root Cause]**

CMake 프로젝트 파일은 `cpp/CMakeLists.txt`에 있고, 실제 테스트 설정은 루트의 `build` 디렉터리에 생성된다. 이 구조를 VS Code 워크스페이스 설정으로 고정하지 않았다.

**[Fix Summary]**

`.vscode/settings.json`을 추가하여 `cmake.sourceDirectory`를 `${workspaceFolder}/cpp`, `cmake.buildDirectory`를 `${workspaceFolder}/build`로 지정했다. `cmake.ctestArgs`에는 `--output-on-failure`를 추가했다.

## 현재 검증 상태

최신 상태에서 다음 명령으로 확인했다.

```bash
cmake --build build
ctest --test-dir build --output-on-failure
```

결과는 `27/27` 테스트 통과이며, 현재 재현되는 테스트 실패는 없다.

