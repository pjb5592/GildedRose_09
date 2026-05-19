# Gilded Rose C++17 요구사항 분석

## 1. 아이템 타입별 비즈니스 규칙

| 아이템 타입 | 식별 기준 | `sellIn` 변경 | `quality` 기본 변경 | 판매 기한 경과 후 규칙 | 품질 범위/예외 |
|---|---|---:|---:|---|---|
| Normal | 특별 아이템 이름과 일치하지 않는 일반 아이템 | 매일 `-1` | 매일 `-1` | 기한 경과 후 하루에 총 `-2` 감소 | `0 <= quality <= 50` |
| Aged Brie | `name == "Aged Brie"` | 매일 `-1` | 매일 `+1` | 기한 경과 후 하루에 총 `+2` 증가 | 최대 `50`을 초과하지 않음 |
| Backstage Pass | `name == "Backstage passes to a TAFKAL80ETC concert"` | 매일 `-1` | `sellIn > 10`: `+1`<br>`6 <= sellIn <= 10`: `+2`<br>`1 <= sellIn <= 5`: `+3` | 콘서트가 지난 뒤 `quality = 0` | 증가 중에는 최대 `50`, 콘서트 후에는 `0` |
| Sulfuras | `name == "Sulfuras, Hand of Ragnaros"` | 변경 없음 | 변경 없음 | 변경 없음 | 전설 아이템 예외. 일반 품질 상한과 별개로 `quality == 80` 유지 |
| Conjured | 예: 이름이 `"Conjured"` 계열인 아이템 | 매일 `-1` | 일반 아이템보다 2배 빠르게 감소, 즉 매일 `-2` | 기한 경과 후 일반 아이템의 2배 기준으로 하루에 총 `-4` 감소 | `0 <= quality <= 50` |

> 구현 관점에서는 `updateQuality()`가 하루 경과를 처리하는 함수다. 현재 레거시 흐름처럼 품질을 먼저 갱신하고 `sellIn`을 감소시킨 뒤, 감소된 `sellIn < 0`이면 판매 기한 경과 규칙을 적용할 수 있다. 이 경우 호출 전 `sellIn == 0`인 아이템은 호출 후 `sellIn == -1`이 되므로 기한 경과 효과까지 반영된다.

## 2. 문자열 비교/분기 시 주의점

1. 특별 아이템은 정확한 이름 비교가 필요하다.
   - `"Aged Brie"`
   - `"Backstage passes to a TAFKAL80ETC concert"`
   - `"Sulfuras, Hand of Ragnaros"`

2. 정확히 하나의 고정 이름을 식별할 때는 `std::string::operator==`를 우선 사용한다.
   ```cpp
   if (item.name == "Aged Brie") {
       // Aged Brie 규칙
   }
   ```

3. `std::string::find()`를 조건식에 직접 넣으면 안 된다.
   - `find()`는 찾은 위치 또는 `std::string::npos`를 반환한다.
   - 위치가 `0`이면 `false`처럼 평가될 수 있고, `npos`는 큰 정수라 `true`처럼 평가될 수 있다.
   - 따라서 반드시 `!= std::string::npos`로 비교한다.
   ```cpp
   if (item.name.find("Conjured") != std::string::npos) {
       // Conjured 계열 아이템
   }
   ```

4. `Conjured`를 카테고리로 볼지, 정확한 아이템명으로 볼지 명확히 정해야 한다.
   - 요구사항의 `"Conjured" items`는 보통 `"Conjured Mana Cake"`처럼 접두어를 포함한 계열 아이템으로 해석된다.
   - C++17에는 `starts_with()`가 없으므로 접두어 기준이면 `item.name.rfind("Conjured", 0) == 0`를 사용할 수 있다.
   - 포함 기준이면 `item.name.find("Conjured") != std::string::npos`를 사용하되, 의도치 않은 문자열 매칭을 테스트로 보호한다.

5. 분기 순서는 예외 아이템을 먼저 처리하는 편이 안전하다.
   - `Sulfuras`는 `sellIn`과 `quality`가 모두 변하지 않아야 하므로 가장 먼저 제외하거나 전용 분기로 처리한다.
   - `Backstage Pass`, `Aged Brie`, `Conjured`, `Normal` 순으로 규칙을 명확히 분리하면 중복 감소/증가를 줄일 수 있다.

## 3. 예외/경계값 조건

1. `quality` 하한은 `0`이다.
   - Normal, Conjured는 감소량이 커도 `0` 아래로 내려가면 안 된다.
   - Backstage Pass는 콘서트 이후 `0`이 되며 음수가 되지 않는다.

2. `quality` 상한은 `50`이다.
   - Aged Brie와 Backstage Pass는 증가 전후 모두 `50`을 초과하면 안 된다.
   - 증가량이 `+2`, `+3`이어도 결과는 `50`으로 제한되어야 한다.

3. `Sulfuras`는 품질 범위의 예외다.
   - 요구사항상 `quality == 80`이며 변경되지 않는다.
   - 일반 아이템의 `0~50` 보정 로직을 `Sulfuras`에 적용하면 안 된다.

4. `sellIn == 0`은 중요한 경계값이다.
   - 호출 전 `sellIn == 0`이면 하루가 지난 뒤 `sellIn == -1`이 된다.
   - Normal은 하루에 총 `-2`, Conjured는 총 `-4`, Aged Brie는 총 `+2` 효과가 기대된다.
   - Backstage Pass는 콘서트가 지난 상태가 되어 `quality == 0`이 된다.

5. `sellIn == -1` 또는 더 작은 음수는 오류가 아니다.
   - 판매 기한이 이미 지난 상태로 보고 경과 후 규칙을 계속 적용한다.
   - Normal은 매일 `-2`, Conjured는 매일 `-4`, Aged Brie는 매일 `+2`로 처리한다.
   - Backstage Pass는 계속 `quality == 0`을 유지한다.

6. 기존 `Item` 클래스와 `items` 보관 구조는 수정하지 않는다.
   - 요구사항에서 `Item` 클래스와 `Items` 속성은 변경 금지로 명시되어 있다.
   - 리팩토링은 `updateQuality()`와 보조 함수/상수 중심으로 제한하는 것이 안전하다.

## 4. Conjured 신규 요구사항 명세

1. `Conjured` 아이템은 일반 아이템과 동일하게 매일 `sellIn`이 `1` 감소한다.

2. `Conjured` 아이템의 `quality` 감소율은 일반 아이템의 2배다.
   - 판매 기한 전: 하루 `-2`
   - 판매 기한 경과 후: 하루 `-4`

3. `Conjured` 아이템도 일반 품질 범위를 따른다.
   - `quality`는 `0` 미만이 될 수 없다.
   - `quality`는 `50`을 초과할 수 없다.

4. `Conjured`는 `Sulfuras` 예외보다 우선할 수 없다.
   - 이름 분류가 겹치는 상황이 생기지 않도록 테스트 데이터를 명확히 한다.
   - 구현에서는 전설 아이템인 `Sulfuras`를 먼저 처리하는 것이 안전하다.

5. 이름 매칭 정책은 테스트로 고정한다.
   - `"Conjured"` 정확 비교만 지원할지, `"Conjured Mana Cake"` 같은 접두어 계열을 지원할지 결정한다.
   - Gilded Rose 카타 관례상 접두어 계열 지원을 권장한다.

## 5. Google Test 기준 테스트 시나리오 목록

1. Normal 아이템은 하루가 지나면 `sellIn`이 `1` 감소하고 `quality`가 `1` 감소한다.

2. Normal 아이템은 호출 전 `sellIn == 0`이면 하루가 지난 뒤 `quality`가 총 `2` 감소한다.

3. Normal 아이템은 호출 전 `sellIn < 0`이면 `quality`가 `2` 감소한다.

4. Normal 아이템은 `quality == 0`에서 더 감소하지 않는다.

5. Aged Brie는 하루가 지나면 `quality`가 `1` 증가한다.

6. Aged Brie는 호출 전 `sellIn == 0`이면 `quality`가 총 `2` 증가한다.

7. Aged Brie는 호출 전 `sellIn < 0`이면 `quality`가 `2` 증가한다.

8. Aged Brie는 `quality == 50`에서 더 증가하지 않는다.

9. Backstage Pass는 `sellIn > 10`일 때 `quality`가 `1` 증가한다.

10. Backstage Pass는 `sellIn == 10`일 때 `quality`가 `2` 증가한다.

11. Backstage Pass는 `sellIn == 5`일 때 `quality`가 `3` 증가한다.

12. Backstage Pass는 증가 후에도 `quality`가 `50`을 초과하지 않는다.

13. Backstage Pass는 호출 전 `sellIn == 0`이면 하루가 지난 뒤 `quality`가 `0`이 된다.

14. Backstage Pass는 호출 전 `sellIn < 0`이면 `quality`가 `0`으로 유지된다.

15. Sulfuras는 하루가 지나도 `sellIn`이 변경되지 않는다.

16. Sulfuras는 하루가 지나도 `quality == 80`을 유지한다.

17. Sulfuras는 `quality` 상한 보정 로직의 영향을 받지 않는다.

18. Conjured 아이템은 판매 기한 전 하루가 지나면 `quality`가 `2` 감소한다.

19. Conjured 아이템은 호출 전 `sellIn == 0`이면 하루가 지난 뒤 `quality`가 총 `4` 감소한다.

20. Conjured 아이템은 호출 전 `sellIn < 0`이면 `quality`가 `4` 감소한다.

21. Conjured 아이템은 감소량이 `quality`보다 커도 `quality == 0`에서 멈춘다.

22. Conjured 아이템 이름 매칭 정책을 검증한다.
   - 접두어 정책이면 `"Conjured Mana Cake"`가 Conjured 규칙으로 처리되어야 한다.
   - 정확 비교 정책이면 `"Conjured"`만 Conjured 규칙으로 처리되어야 한다.

23. 여러 아이템을 한 번에 전달했을 때 각 아이템이 독립적으로 자신의 규칙에 따라 갱신된다.

24. 빈 `items` 벡터를 전달해도 예외 없이 처리된다.
