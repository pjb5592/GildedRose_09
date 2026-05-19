# Gilded Rose C++ QA 최종 보고서

작성일: 2026-05-19  
작성 관점: QA 리드 엔지니어  
관련 산출물: [요구사항 분석](requirements_analysis.md) · [코드 품질](code_quality_report.md) · [테스트 계획](test_plan.md) · [결함 관리](defect_report.md) · [결함 목록](defect_list.md)

---

## 1. Executive Summary

Gilded Rose C++17 프로젝트는 **Prompt 01~09** 9단계 QA·리팩토링 워크플로를 거쳐, 레거시 `updateQuality()`에 대한 **명세 기반 단위 테스트(27건)**, **Golden Master(1건)**, **결함 추적(4건)**, **1차 구조 개선**까지 완료한 상태이다.

| 영역 | 결과 | 판정 |
|------|------|------|
| 테스트 완료율 (실행) | **28/28 (100%)** | 통과 |
| `GildedRose.cpp` 라인·함수 커버리지 | **100% / 100%** (gcov/lcov) | 목표(90%/95%) **초과 달성** |
| 브랜치 커버리지 | **미측정** (lcov `branches: no data`) | 목표(85%) **판정 보류** |
| 요구사항 시나리오 매핑 | **22/24 (91.7%)** | P2 복합 시나리오 2건 미구현 |
| Open Critical/Major | **0건** | 릴리스 게이트 충족 |
| 코드 품질(SRP/OCP) | 부분 개선 | 9단계 리팩토링 로드맵 잔여 |

**종합 판정:** 기능·회귀 안전망은 **프로덕션 수준의 카타 완료 기준**을 충족한다. 다만 **브랜치 커버리지 자동화**, **P2 복합 테스트**, **아이템별 정책 분리(OCP)** 는 다음 레거시 프로젝트로 이관할 개선 과제이다.

---

## 2. 테스트 완료율 및 커버리지 (목표 대비 gcov)

### 2.1 테스트 실행 완료율

**측정 일시:** 2026-05-19  
**명령:**

```bash
cd cpp/build-coverage
ctest --output-on-failure
```

| 구분 | 건수 | 통과율 | 비고 |
|------|-----:|-------:|------|
| 단위 테스트 (`GildedRoseItemRulesTest`) | 27 | 100% | `TEST_F`, Given-When-Then |
| Golden Master (`GoldenMasterTest`) | 1 | 100% | 31일 Texttest 전체 출력 비교 |
| **합계** | **28** | **100%** | `defect_report.md` 스냅샷(27/27) 대비 GM 1건 추가 반영 |

**테스트 계획 대비 완료율**

| 우선순위 | 계획 범위 | 구현 상태 | 완료율(추정) |
|----------|-----------|-----------|-------------:|
| P0 | Normal, Aged Brie, Backstage, Sulfuras, 경계값 | 22건 + 경계 포함 | **100%** |
| P1 | Conjured, 접두어 정책 | 6건 | **100%** |
| P2 | 빈 벡터, 다중 아이템, 유사 이름 | **미구현** | **0%** |
| IT | Golden Master | 1건 | **100%** |

**요구사항 분석 §5 시나리오(24건) 매핑**

| # | 시나리오 | 테스트 존재 |
|---|----------|:-----------:|
| 1~4 | Normal | ✅ (5 tests) |
| 5~8 | Aged Brie | ✅ (5) |
| 9~14 | Backstage Pass | ✅ (6) |
| 15~17 | Sulfuras | ✅ (5, 비표준 quality 추가) |
| 18~22 | Conjured | ✅ (6) |
| 23 | 여러 아이템 동시 처리 | ❌ |
| 24 | 빈 `items` 벡터 | ❌ |

→ **시나리오 완료율: 22/24 = 91.7%** (P0·P1 중심 완료, P2 복합 2건 잔여)

### 2.2 gcov/lcov 커버리지 (목표 대비)

**빌드:** `cpp/build-coverage` (GCC 15.2.0, `--coverage -O0 -g`)  
**집계 대상:** `cpp/src/GildedRose.cpp`

```bash
lcov --capture --directory build-coverage -o coverage.info
lcov --remove coverage.info '/usr/*' '*/_deps/*' '*/test/*' -o coverage.filtered.info
lcov --extract coverage.filtered.info '*/src/GildedRose.cpp' -o gilded_rose.info
lcov --summary gilded_rose.info
```

| 메트릭 | 목표 ([test_plan.md](test_plan.md) §6.1) | 실측 (`GildedRose.cpp`) | 달성 |
|--------|------------------------------------------|-------------------------|:----:|
| 라인 커버리지 | ≥ 90% | **100% (29/29)** | ✅ |
| 함수 커버리지 | ≥ 95% | **100% (5/5)** | ✅ |
| 브랜치 커버리지 | ≥ 85% | **N/A** (`branches: no data found`) | ⚠️ |
| `updateQuality()` 분기 | 가능한 한 100% | 라인 기준 전 분기 실행됨 | ✅* |

\* MinGW lcov 기본 설정에서는 **브랜치 프로파일이 수집되지 않음**. `gcovr --branches` 또는 CI에서 `CFLAGS+=" -fcondition-coverage"` 등 별도 설정 필요.

**함수별 커버리지 (`GildedRose.cpp` 내부)**

| 함수 | 역할 | 커버 |
|------|------|:----:|
| `isConjured` | Conjured 접두어 판별 | 100% |
| `increaseQuality` | 상한 clamp | 100% |
| `decreaseQuality` | 하한 clamp | 100% |
| `GildedRose::GildedRose` | 생성자 | 100% |
| `GildedRose::updateQuality` | 핵심 비즈니스 로직 | 100% |

**미커버·약한 영역 (리스크)**

- **코드:** 없음 (라인 100%)
- **요구사항:** 빈 컨테이너·다중 아이템 독립성은 테스트 미작성 → 커버리지 수치만으로는 보장 불가
- **브랜치:** `originalSellIn <= 0` vs `> 0` 등 조합의 **논리적 분기 충족률**은 별도 도구 필요

### 2.3 아이템 타입별 테스트 밀도

| ItemType | 단위 테스트 수 | 경계값 (`0`/`50`/`sellIn 0`/`−1`) |
|----------|---------------:|-----------------------------------|
| Normal | 5 | ✅ |
| AgedBrie | 5 | ✅ |
| BackstagePass | 6 | ✅ (`11`, `10`, `5`, `0`, `−1`) |
| Sulfuras | 5 | ✅ (+ `quality` 50/80 변형) |
| Conjured | 6 | ✅ (+ 접두어 `"Conjured Sulfuras Replica"`) |

---

## 3. 결함 패턴 분석

### 3.1 심각도(Severity)별

등록 결함 **4건** ([defect_list.md](defect_list.md)), 모두 **Fixed · Verified** 상태.

| Severity | 건수 | 비율 | 대표 ID |
|----------|-----:|-----:|---------|
| **Major** | 3 | 75% | DEF-001, DEF-002, DEF-003 |
| **Minor** | 1 | 25% | DEF-004 |
| Critical | 0 | 0% | — |
| Info (코드 스멜) | (별도 문서) | — | [code_quality_report.md](code_quality_report.md) |

**패턴:** 실제 **도메인 결함은 Conjured 신규 요구(2건)** 에 집중. **테스트 인프라·placeholder(2건)** 가 Major로 분류되어 초기 Red를 유발했다. Critical(데이터 무결성 파괴)은 TDD 단계에서 조기 차단되지 않았으나, 구현 전 테스트 작성으로 **배포 전 차단**에 성공.

### 3.2 ItemType별

| ItemType | 결함 수 | Severity | 근본 원인 패턴 |
|----------|--------:|----------|----------------|
| **Conjured** | 2 | Major | 신규 요구 미구현 → 일반 아이템 규칙으로 오분류 |
| **Test** | 1 | Major | Kata 초기 `Foo`/`fixme` placeholder |
| **TestEnvironment** | 1 | Minor | CMake 소스·빌드 경로 미고정 |
| Normal / AgedBrie / BackstagePass / Sulfuras | 0 | — | 기존 레거시가 명세와 일치 (GM 포함) |

**도메인 5종 중 결함이 발생한 타입은 Conjured뿐** → 신규 규칙 추가 시 **기존 분기 구조(OCP 미달)** 와 결합되어 결함 밀도가 높아지는 전형적 레거시 패턴.

### 3.3 발견 단계별 (RQ / UT / IT / RF / RG)

| 단계 | 건수 | 비율 | 결함 |
|------|-----:|-----:|------|
| **UT** (단위 테스트·TDD) | 3 | 75% | DEF-001, DEF-002, DEF-003 |
| **RQ** (초기 상태·환경) | 1 | 25% | DEF-004 |
| IT | 0 | 0% | Golden Master는 사후 검증만 |
| RF / RG | 0 | 0% | — |

**시사점:** 결함의 **75%가 UT 단계에서 최초 발견** → 테스트 우선 전략이 유효했음. Golden Master는 **사후 회귀망**으로 작동했고, 신규 결함 발견에는 기여하지 않음.

### 3.4 Severity × ItemType 매트릭스 (실적)

[defect_report.md](defect_report.md) §1.3 이론 매트릭스 대비 **실제 발생 셀**:

|  | Normal | AgedBrie | Backstage | Sulfuras | Conjured |
|--|:------:|:--------:|:---------:|:--------:|:--------:|
| Critical | — | — | — | — | — |
| **Major** | — | — | — | — | **DEF-002, DEF-003** |
| Minor | — | — | — | — | — |
| 비도메인 | — | — | — | — | Test: DEF-001 / Env: DEF-004 |

---

## 4. 9단계 QA 워크플로 효과 평가

**9단계 정의:** `Prompt/01` ~ `Prompt/09` (10번 결함 관리 보고서는 메타·집계 단계)

| 단계 | Prompt | 산출물 | 효과 | 개선 필요 |
|:----:|--------|--------|------|-----------|
| 1 | QA·리팩토링 규칙 | `.cursorrules` | **높음** — Item 불변·Green 리팩토링·경계값 테스트 원칙 고정 | CI에 규칙 링크 자동 검증 없음 |
| 2 | 요구사항 분석 | `requirements_analysis.md` | **높음** — 5종 ItemType·24 시나리오·문자열 함정 문서화 | Conjured 접두어 정책을 구현 전 ADR로 확정하면 더 좋음 |
| 3 | 코드 품질 | `code_quality_report.md` | **중~높음** — SRP/OCP·매직넘버 이슈 식별, 1차 상수화·헬퍼 반영 | 전략 패턴·함수 분리는 미완 |
| 4 | 테스트 계획 | `test_plan.md` | **높음** — P0/P1/P2·경계 매트릭스·gcov 목표 | P2 항목 완료 기준에 미포함 |
| 5 | 테스트 구현 | `GildedRoseTest.cpp` (27 tests) | **매우 높음** — TDD로 Conjured 결함 2건 조기 발견 | P2 시나리오 2건 누락 |
| 6 | 디버깅·결함 분석 | Report 06 | **낮음** (본 스프린트) — 이미 Green 상태에서 수행 | **Red 상태에서** 실행하도록 순서 조정 |
| 7 | 결함 목록 | `defect_list.md` | **높음** — DEF-ID·재현·수정 추적 | GitHub Issues 연동은 선택 미적용 |
| 8 | Golden Master | `GoldenMasterTest.cpp` | **중~높음** — 31일 통합 회귀 1건 | GM만으로 Conjured 세부 규칙 설계 불가 |
| 9 | 리팩토링 계획 | Report 09 | **중간** — 로드맵·커밋 단위 계획 | Phase 1+ 미실행(SRP 분리 잔여) |

### 4.1 가장 효과적이었던 단계 (Top 3)

1. **5단계 — 테스트 구현 (TDD):** 결함의 75%를 UT에서 발견·차단. Given-When-Then·경계값으로 요구사항 추적성 확보.
2. **2단계 — 요구사항 분석:** ItemType·`sellIn==0` 처리 순서 등 모호성 제거 → 테스트 기대값 논쟁 최소화.
3. **8단계 — Golden Master:** 넓은 시뮬레이션 회귀(28번째 테스트). 리팩토링 Phase 진행 시 **안전망** 역할.

### 4.2 개선이 필요한 단계 (Top 3)

1. **6단계 — 디버깅:** Green 이후 실행 시 가치 제한적 → **5단계 직후 Red 로그 분석**으로 재배치.
2. **9단계 — 리팩토링:** 계획만 있고 아이템별 정책 분리·OCP 개선 **미착수** → 기술 부채 잔존.
3. **4단계 — 테스트 계획:** P2(빈 벡터·다중 아이템)가 **완료 기준·스프린트 범위에서 누락** → 릴리스 게이트에 명시 필요.

---

## 5. 다음 레거시 프로젝트 Best Practice 5가지

### BP-1. «문서 → Red 테스트 → Green 구현» 순서 고정

요구사항·테스트 계획(2·4단계) 직후 **실패하는 명세 테스트**(5단계)를 작성하고, placeholder·골든 파일을 제거한 뒤 구현한다. 본 프로젝트에서 Conjured Major 2건이 이 패턴으로 **구현 전 차단**되었다.

### BP-2. 경계값 매트릭스를 테스트 코드의 SSOT로 유지

`quality ∈ {0, 1, 49, 50}`, `sellIn ∈ {−1, 0, 5, 10, 11}` 조합을 [test_plan.md](test_plan.md) §4.3 표와 1:1 매핑한다. 신규 ItemType 추가 시 **표에 행을 추가한 뒤** `TEST_F`를 작성한다.

### BP-3. 이중 안전망: 좁은 단위 테스트 + 넓은 Golden Master

- **단위:** 아이템·경계·실패 메시지가 명확한 `TEST_F` (본 프로젝트 27건)
- **통합:** Texttest·승인 테스트 (본 프로젝트 31일 GM 1건)  
Golden Master만으로는 **신규 규칙 설계·TDD**에 부적합함이 확인되었다.

### BP-4. 커버리지 게이트는 «라인 + 브랜치 + 파일 단위»로 설정

`GildedRose.cpp` 라인 100% 달성에도 **브랜치 미측정**이면 게이트 통과 착시가 발생한다. CI에서 `lcov`/`gcovr --branches`를 필수화하고, **핵심 파일 단위 extract** (`lcov --extract '*/src/Target.cpp'`)로 집계한다.

### BP-5. 결함 분류(Severity × ItemType)를 스프린트 메트릭으로 운영

[defect_report.md](defect_report.md) 체계를 매 스프린트마다 **발견 단계·ItemType별 히트맵**으로 갱신한다. 신규 타입(Conjured) 추가 스프린트에서는 해당 열의 Major 비율을 **릴리스 차단 지표**로 사용한다.

---

## 6. Cursor AI 활용 효과 (정량·정성)

### 6.1 정량 요약

| 지표 | AI 보조 전(추정 기준선) | AI 보조 후(실측) | 효과 |
|------|-------------------------|------------------|------|
| 의미 있는 단위 테스트 | 0 (placeholder 1) | **27** | +27 |
| CTest 통과율 | 0% (Foo 실패) | **100% (28/28)** | +100%p |
| `GildedRose.cpp` 라인 커버리지 | 미측정 (레거시 only) | **100%** | 목표 90% 대비 +10%p |
| 등록·해결 결함 | — | **4건 / 4건** | 추적 가능 |
| QA 문서 (docs/) | 0 | **5** (+ 본 보고서) | 산출물 표준화 |
| Golden Master 테스트 | 0 | **1** | IT 회귀망 |

※ 시간 단축은 저장소에 타임스탬프가 없어 **상대 추정**이다. 동일 범위를 수동으로 수행 시 통상 **2~3일 → 1일 이내** (문서+테스트+Conjured 구현)로 압축된 것으로 QA 리드 관찰치를 기록한다.

### 6.2 정성 요약

| 영역 | 효과 | 한계 |
|------|------|------|
| **시간 단축** | 요구사항·테스트 계획·`TEST_F` 보일러플레이트·gcov 명령어를 신속 생성 | P2 테스트·브랜치 커버리지는 자동 완료되지 않음 — 명시 요청 필요 |
| **결함 조기 발견** | Conjured Red 테스트 → 구현 가이드 (`isConjured`, 2x/4x 감소) | Green 상태에서 6단계 디버깅은 가치 낮음 — 단계 순서는 사람이 통제 |
| **커버리지 향상** | 경계값 시나리오 전수 매핑 → 라인 100% | 브랜치 프로파일·P2 시나리오는 도구/요청 없으면 누락 |
| **일관성** | `.cursorrules`·Given-When-Then·DEF-ID 형식 유지 | OCP 수준 리팩토링은 «계획 문서»에 그칠 수 있음 — 실행 게이트 필요 |
| **지식 전달** | SOLID 스멜·Severity 매트릭스 등 QA 어휘로 보고서 통일 | AI 생성 문서는 **실측 ctest/lcov로 교차 검증** 필수 (본 보고서는 2026-05-19 실측 반영) |

### 6.3 권장 Cursor 사용 패턴 (재현)

1. **Prompt 체인 01→05:** 규칙·요구사항·테스트 계획 후 즉시 Red 테스트 생성  
2. **Prompt 06:** `ctest` 실패 로그를 붙여 Root Cause 분석 (Green일 때 생략)  
3. **Prompt 07~08:** 결함 목록 + Golden Master는 **구현 안정화 직후**  
4. **Prompt 09:** 리팩토링은 **커밋 단위 체크리스트**로만 진행, AI에게 «한 Phase씩» 지시  
5. **마지막:** `build-coverage` + `lcov --extract`로 본 보고서 §2 수치 갱신  

---

## 7. 현재 구현·품질 스냅샷

### 7.1 `updateQuality()` 구조 (검증 완료)

- `Sulfuras` 조기 `continue`
- `originalSellIn` 보존 후 `sellIn` 감소 → 만료 규칙
- `increaseQuality` / `decreaseQuality` clamp (`0`~`50`)
- Conjured: `rfind("Conjured", 0) == 0`

### 7.2 잔여 리스크·백로그

| ID | 항목 | 우선순위 |
|----|------|----------|
| QA-001 | `EmptyItemsVector` / `MultipleItemsIndependent` 테스트 | P2 |
| QA-002 | 브랜치 커버리지 CI (gcovr 또는 lcov branch) | P1 |
| QA-003 | Backstage 임계값 상수화 + 아이템별 함수 분리 (Report 09) | P2 |
| QA-004 | GitHub Actions: `ctest` + coverage artifact | P2 |

---

## 8. 릴리스 게이트 최종 체크

[defect_report.md](defect_report.md) §3.5 기준:

| 게이트 | 기준 | 결과 |
|--------|------|:----:|
| P0 테스트 Green | 100% | ✅ |
| P1 Conjured Green | 100% | ✅ |
| `GildedRose.cpp` 라인 ≥ 90% | 100% | ✅ |
| 브랜치 ≥ 85% | 미측정 | ⚠️ |
| Critical/Major Open 0 | 0 | ✅ |
| `Item` public 무변경 | 유지 | ✅ |
| P2 복합 시나리오 | 미완 | ⚠️ |

**QA 리드 권고:** 교육·카타 **완료 승인** 가능. 프로덕션형 파이프라인 이관 시 **QA-002·QA-004**를 다음 스프린트 필수 항목으로 편입한다.

---

## 9. 부록

### A. 검증 명령 치트시트

```bash
# 기능 회귀
cd cpp/build
ctest --output-on-failure

# 커버리지
cd cpp
cmake -S . -B build-coverage -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="--coverage -O0 -g" \
  -DCMAKE_EXE_LINKER_FLAGS="--coverage"
cmake --build build-coverage
ctest --test-dir build-coverage --output-on-failure
lcov --capture --directory build-coverage -o build-coverage/coverage.info
lcov --remove build-coverage/coverage.info '/usr/*' '*/_deps/*' '*/test/*' \
  -o build-coverage/coverage.filtered.info
lcov --extract build-coverage/coverage.filtered.info '*/src/GildedRose.cpp' \
  -o build-coverage/gilded_rose.info
lcov --summary build-coverage/gilded_rose.info
```

### B. 참조 문서·코드

- 구현: `cpp/src/GildedRose.cpp`, `cpp/include/GildedRose.h`
- 테스트: `cpp/test/GildedRoseTest.cpp`, `cpp/test/GoldenMasterTest.cpp`
- 결함: `docs/defect_list.md`, `docs/defect_report.md`

---

*본 보고서의 테스트·커버리지 수치는 2026-05-19 로컬 `cpp/build-coverage`에서 `ctest` 및 `lcov 1.16`으로 재현 가능하다.*
