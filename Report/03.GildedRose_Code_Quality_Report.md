# Gilded Rose `updateQuality()` 코드 품질 분석

## SOLID 및 Code Smell 분석

| 문제점 | 위반 원칙/스멜 | 영향 | 개선 방향 | 우선순위 |
|---|---|---|---|---:|
| `updateQuality()`가 모든 아이템의 종류 판별, `quality` 변경, `sellIn` 감소, 만료 후 규칙 적용을 한 번에 처리한다. | SRP 위반, Long Method | 하루 경과 처리라는 공통 흐름과 아이템별 정책이 강하게 결합되어 변경 범위가 커진다. 특정 아이템 규칙을 수정해도 전체 메서드 회귀 위험이 높다. | 아이템별 품질 정책을 별도 함수나 전략 객체로 분리하고, `updateQuality()`는 순회와 위임만 담당하게 만든다. | 1 |
| 새 아이템 타입을 추가하려면 기존 조건문 내부를 직접 수정해야 한다. | OCP 위반 | `Conjured` 같은 신규 규칙이 들어올 때 기존 분기 구조를 건드리게 되어 정상 동작 중인 `Aged Brie`, `Backstage Pass`, `Sulfuras`까지 영향을 받을 수 있다. | `QualityUpdater` 전략 인터페이스, 함수 테이블, 또는 아이템 타입 분류 후 규칙 디스패치 구조를 도입한다. | 1 |
| `"Aged Brie"`, `"Backstage passes to a TAFKAL80ETC concert"`, `"Sulfuras, Hand of Ragnaros"` 문자열이 여러 조건식에 반복된다. | Duplicated Code, Primitive Obsession | 오탈자나 이름 변경 시 여러 위치를 동시에 수정해야 하며, 분기 조건의 의미가 코드에 묻힌다. | `constexpr std::string_view` 또는 상수 이름으로 추출한다. 예: `AGED_BRIE`, `BACKSTAGE_PASS`, `SULFURAS`. | 2 |
| `0`, `50`, `11`, `6` 같은 숫자가 도메인 의미 없이 직접 사용된다. | Magic Number | `quality` 하한/상한, Backstage Pass 임계일의 의미를 읽는 사람이 추론해야 한다. 경계값 테스트와 요구사항 매핑도 어려워진다. | `MIN_QUALITY = 0`, `MAX_QUALITY = 50`, `BACKSTAGE_DOUBLE_THRESHOLD = 10`, `BACKSTAGE_TRIPLE_THRESHOLD = 5`처럼 의미 있는 상수로 추출한다. 현재 조건이 `< 11`, `< 6`이므로 도메인 표현은 `<= 10`, `<= 5`로 정리하는 편이 명확하다. | 2 |
| 품질 증가와 감소 전에 `quality < 50`, `quality > 0` 검사가 여러 번 반복된다. | Duplicated Code, Shotgun Surgery | 품질 범위 보정 규칙이 흩어져 있어 증가량이 `+2`, `+3`인 경우 상한 처리 실수가 생기기 쉽다. | `increaseQuality(item, amount)`, `decreaseQuality(item, amount)`, `clampQuality()` 같은 보조 함수를 만든다. `Sulfuras` 예외는 호출 전에 분리한다. | 2 |
| `if` 중첩이 깊고 부정 조건이 많다. 예: `name != A && name != B`, 내부에서 다시 `name != Sulfuras` 확인. | 조건문 복잡도, Nested Conditionals | 실제 규칙보다 제어 흐름 이해 비용이 커진다. 예외 아이템이 추가될수록 조건 조합이 폭발한다. | `Sulfuras`를 조기 반환하거나 전용 전략으로 먼저 분리한다. 그 뒤 `Aged Brie`, `Backstage Pass`, 일반 아이템 규칙을 독립적인 함수로 나눈다. | 1 |
| `sellIn` 감소 전후의 규칙 적용 순서가 메서드 내부 흐름에 암묵적으로 묶여 있다. | Temporal Coupling | `sellIn == 0` 경계에서 품질이 언제 몇 번 바뀌는지 코드만 보고 파악하기 어렵다. 리팩토링 중 순서가 바뀌면 동작이 달라질 수 있다. | "품질 갱신 -> `sellIn` 감소 -> 만료 후 보정" 흐름을 명시하거나, 아이템별 전략 내부에서 하루 경과 규칙을 하나의 단위로 캡슐화한다. | 3 |
| `Backstage Pass` 규칙이 기본 증가 후 임계일별 추가 증가로 표현되어 있다. | Hidden Business Rule, Low Readability | 요구사항인 `sellIn > 10: +1`, `6~10: +2`, `1~5: +3`, 만료 후 `0`과 코드 구조가 직접 대응하지 않는다. | 증가량을 계산하는 함수로 분리한다. 예: `backstageQualityDelta(sellIn)`가 `1`, `2`, `3`, 또는 만료 후 `0` 리셋을 명확히 반환하게 한다. | 3 |
| `items[i]` 인덱스 접근이 반복된다. | Readability Smell | 같은 아이템을 반복 참조해 코드가 장황해지고, 실제 규칙보다 컨테이너 접근 문법이 눈에 띈다. | 범위 기반 for 문과 참조를 사용한다. 예: `for (Item& item : items)`. | 4 |
| 아이템 타입이 문자열 비교에만 의존한다. | Primitive Obsession, Type Code Smell | 이름 문자열이 타입 판별과 표시 이름 역할을 동시에 맡는다. 분류 로직이 여러 곳에 흩어지기 쉽다. | `enum class ItemKind` 분류 함수 또는 `std::variant` 기반 타입 모델을 고려한다. 단, 현재 `Item` public 인터페이스를 바꾸지 않는 제약이 있으므로 분류 함수부터 도입하는 것이 안전하다. | 4 |
| 모든 규칙이 하나의 큰 조건문 구조에 들어 있어 테스트 실패 시 원인 범위가 넓다. | Low Testability | 특정 아이템 규칙만 단위로 검증하기 어렵고, 골든 마스터 테스트에 과하게 의존하게 된다. | 아이템별 업데이트 함수를 독립적으로 테스트할 수 있게 분리한다. 경계값(`0`, `50`, `sellIn == 0`, `sellIn == -1`) 테스트를 우선 보강한다. | 5 |

## 개선 방향 요약

1. 가장 먼저 `Sulfuras`, `Aged Brie`, `Backstage Pass`, 일반 아이템 규칙을 함수 단위로 분리한다. 이 단계는 설계를 크게 바꾸지 않으면서 SRP, 중첩 조건, 중복 품질 보정 문제를 줄일 수 있다.

2. `MIN_QUALITY`, `MAX_QUALITY`, Backstage Pass 임계일, 특별 아이템 이름을 상수화한다. 숫자와 문자열의 의미가 드러나면 요구사항, 테스트, 코드 간 추적성이 좋아진다.

3. 품질 변경은 `increaseQuality()`와 `decreaseQuality()` 같은 C++17 보조 함수로 모은다. `std::clamp`를 사용할 수 있지만, `Sulfuras`는 일반 품질 범위의 예외이므로 공통 보정 로직 적용 대상에서 제외해야 한다.

4. 그 다음 단계로 전략 패턴을 도입할 수 있다. 예를 들어 `QualityStrategy` 인터페이스를 두고 `NormalStrategy`, `AgedBrieStrategy`, `BackstagePassStrategy`, `SulfurasStrategy`가 각자 하루 경과 규칙을 처리하게 하면 OCP 위반을 크게 줄일 수 있다.

5. 더 가벼운 대안으로는 테이블 기반 디스패치가 있다. `std::unordered_map<std::string_view, UpdateFunction>` 형태로 특별 아이템 이름과 처리 함수를 연결하고, 매칭되지 않는 경우 일반 아이템 함수를 호출한다. 아이템 종류가 적고 `Item` 구조를 바꾸기 어렵다면 전략 클래스보다 부담이 낮다.

6. `std::variant` 기반 모델은 아이템 타입을 컴파일 타임에 분리할 수 있어 타입 안정성이 높다. 다만 현재 요구사항상 `Item` 구조체와 public 인터페이스를 수정하지 않는 제약이 있으므로, 이 프로젝트에서는 즉시 적용하기보다 장기 개선안으로 보는 것이 적절하다.

## 리팩토링 우선순위

1. 아이템별 규칙 함수 분리: 가장 큰 SRP/OCP 문제와 조건문 복잡도를 동시에 낮춘다.
2. 매직 넘버와 특별 아이템 이름 상수화: 변경 비용과 경계값 해석 비용을 빠르게 줄인다.
3. 품질 증감 보조 함수 도입: `0~50` 범위 보장 중복을 제거하고 실수 가능성을 낮춘다.
4. 범위 기반 for 문과 조기 반환/조기 분기 적용: 읽기 쉬운 제어 흐름으로 만든다.
5. 전략 패턴 또는 테이블 기반 디스패치 적용: 신규 아이템 추가 요구가 반복될 때 OCP를 본격적으로 개선한다.
