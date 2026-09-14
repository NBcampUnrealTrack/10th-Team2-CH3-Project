# WBP HUD 전환 및 에디터 인수인계

## 코드 구성

- PriestHUDWidget은 Abstract 부모 클래스다. C++ 배치 생성 코드는 제거했다.
- 필수 BindWidget: HealthBar(ProgressBar), HealthText/WeaponText/MissionText(TextBlock).
- NativeConstruct에서 보관한 데이터를 반영한다. SetHealth는 내부 데이터 갱신 후 Refresh를 사용한다.
- UIManager.SetHUDWidgetClass로 실제 WBP 클래스를 지정한 후 ShowHUD(Data)를 호출한다. 클래스 미지정/추상 클래스는 false를 반환하고 경고를 출력한다.
- 같은 클래스는 HUD를 재사용한다. 클래스 변경 시 기존 HUD를 제거하므로 이후 ShowHUD를 호출한다.
- UpdateHUD는 생성하지 않는다. 전체 스냅샷이므로 변경하지 않은 필드도 유지해야 한다.
- IngamePlayerController의 HUD Widget Class가 설정되어 있으면 BeginPlay에서 Initial HUD Data로 자동 표시한다.
- HUDTestActor의 HUD Widget Class는 테스트용 재정의다. 비워두면 PlayerController가 설정한 클래스를 사용한다.
- PlayerController가 캐릭터/무기 이벤트를 구독해 실제 HP와 탄약을 반영한다. Initial HUD Data의 전투 값은 실제 값으로 대체되며 미션/시간은 유지된다. 기존 MVC 틀은 사용하지 않는다.

## 1. 에디터 재시작과 WBP 생성

헤더/리플렉션 변경이므로 에디터를 열어둔 상태였다면 종료 후 다시 연다.
콘텐츠 브라우저에서 Widget Blueprint를 생성하고 부모 클래스를 PriestHUDWidget으로 선택한다.
이름은 WBP_PriestHUD를 권장한다. 이미 만든 WBP가 있다면 Class Settings의 Parent Class를 PriestHUDWidget으로 변경한다.

## 2. 디자이너 구성

Canvas Panel을 루트로 만들고 아래 위젯을 배치한다. 필수 4개는 정확한 이름과 타입으로 만들고 Is Variable을 켠다.

```text
Canvas Panel
├─ HealthText (Text Block, 필수)
├─ HealthBar (Progress Bar, 필수)
├─ WeaponText (Text Block, 필수)
├─ MissionPanel (Border, 자유 이름)
│  └─ MissionText (Text Block, 필수)
└─ Crosshair (Text Block 또는 Image, 자유 이름)
```

| 위젯 | Anchor | Alignment | Position X,Y | Size X,Y |
|---|---|---|---|---|
| HealthText | 좌측 하단 | 0,0 | 40,-100 | 300,32 |
| HealthBar | 좌측 하단 | 0,0 | 40,-60 | 280,20 |
| WeaponText | 우측 하단 | 1,0 | -40,-110 | 360,80 |
| MissionPanel | 우측 중앙 | 1,0.5 | -40,0 | 360,160 |
| Crosshair | 중앙 | 0.5,0.5 | 0,0 | 40,40 |

기존 스타일: HP 20pt, 무기 22pt, 미션 20pt, 조준점 + 28pt. 흰 글자/검정 그림자(1,1), 녹색 체력바.
WeaponText는 오른쪽 정렬이며 코드가 무기명과 탄약을 두 줄로 표시한다.
MissionPanel은 검정에 가까운 색/알파 0.6, Padding 20. MissionText는 Auto Wrap Text를 켠다.
원하는 디자인으로 변경해도 된다. Percent/Text에 별도의 Blueprint Bind나 Tick을 추가하지 않는다.
디자이너 미리보기 값은 직접 입력할 수 있으며 실행하면 C++ 전달 데이터로 대체된다.
Compile/Save하고 필수 위젯 누락 오류가 없는지 확인한다.

## 3. 게임에서 자동 표시

실제 사용하는 BP_IngamePlayerController의 Class Defaults → Priest | UI:
- HUD Widget Class: WBP_PriestHUD
- Initial HUD Data: 초기 HP, 무기명, 탄약, 미션 입력

현재 맵의 World Settings → GameMode Override와 해당 GameMode의 Player Controller Class를 확인한다.
프로젝트 기본 GameMode를 쓰는 맵은 Project Settings의 Maps & Modes도 확인한다.
실제로 위 BP 컨트롤러를 사용하는 경우 BeginPlay에서 HUD를 자동 표시한다. 별도 Create Widget/Add to Viewport 노드는 필요 없다.
BP BeginPlay에서 직접 ShowHUD를 호출해둔 경우 자동 초기 표시와 중복 초기화를 피하도록 정리한다.

다른 컨트롤러에서 수동 연결하려면 로컬 플레이어의 PriestUIManager 서브시스템을 가져와
Set HUD Widget Class(WBP_PriestHUD) → Show HUD(전체 Data) 순서로 호출한다.
HUD 전체는 HitTestInvisible로 표시되므로 클릭 버튼은 별도 메뉴/결과 위젯에서 구현한다.

## 4. 디자인 테스트

테스트 맵에 PriestHUDTestActor를 하나 배치하고 HUD Widget Class에 WBP_PriestHUD를 지정한다.
Play에서 1초마다 체력과 탄약이 변하는지 확인한다. Animate Data를 끄면 Test Data의 값을 고정 표시한다.
최대 체력 0, HP 0, 탄약 0, 긴 미션, 창 크기 변경, Play 종료/재시작, Hide 후 Show를 확인한다.
실제 데이터 연결 전 테스트 액터를 제거한다. 테스트 액터는 매초 전체 HUD 데이터를 덮어쓴다.

## 검증 범위

C++ 빌드와 실제 WBP 화면 검증은 별개다. WBP 에셋 생성/바인딩/PIE 시각 확인은 위 절차로 에디터에서 진행한다.
공유 대상은 수정된 Source와 이 문서, 새 WBP 및 설정을 저장한 BP/맵 에셋이다. Binaries/Intermediate/솔루션은 공유하지 않는다.

## 실제 전투 HUD 연결 (2026-09-12)

- 테스트 액터를 제거하고 기존 WBP를 그대로 사용한다. 공용 dev 맵 대신 개인 테스트 맵에서 검증한다.
- BP_IngamePlayerController의 HUD Widget Class를 지정하고 GameMode가 이 컨트롤러와 플레이어 BP를 사용하는지 확인한다.
- 플레이어 BP의 Weapon Class에 실제 무기 BP를 지정한다. 무기 BP의 Weapon Name은 HUD 표시명이다.
- 시작/조종 캐릭터 변경 시 HP와 탄약을 읽고, 피격/발사/재장전 완료 시 이벤트로 갱신한다. Tick 폴링은 하지 않는다.
- 무기가 없으면 Unarmed 및 0/0을 표시한다. 현재 전투 코드는 탄창이 0이 되면 자동 재장전을 시도한다.
- 기본 무기 설정 기준 12/60 → 한 발 발사 11/60 → 탄창 소진 0/60 → 재장전 완료 12/48을 확인한다. BP에서 값이 변경됐다면 해당 값으로 확인한다.
- 피격으로 실제 HP 감소, 최대 체력 0 처리, 미션 문구 유지, 재시작 및 조종 캐릭터 변경 후 기존 캐릭터의 이벤트가 HUD를 바꾸지 않는지 확인한다.
- 현재 로컬 전투 데이터 기준이다. 네트워크 복제, 새로운 무기 교체 API, BP에서 직접 변수 대입 시 알림은 별도 범위다.
- UpdateHUD로 전체 데이터를 전달하면 전투 값도 덮어쓰므로 미션 갱신용으로 기본 구조체를 전달하지 않는다.

## 명중 히트마커 연결

- 몬스터 TakeDamage에서 HP가 감소하면 공격자 컨트롤러 → ClientNotifyHitConfirmed → UIManager → HUD.OnHitConfirmed로 전달한다.
- 총격 및 성수 데미지 모두 포함한다. 성수는 GetInstigatorController로 공격자를 전달한다.
- 0/음수 데미지, 이미 죽은 몬스터, 벽/바닥 명중은 효과를 보내지 않는다. 처치하는 마지막 명중은 포함한다.
- WBP_PriestHUD Event Graph에서 우클릭하여 Event On Hit Confirmed를 추가한다.
- Event → Stop Animation(HitConfirm) → Play Animation(HitConfirm, Start 0, Loops 1, Forward)로 연결한다.
- 추가 BindWidget은 없다. HitMarker 기본 Render Opacity는 0으로 유지한다.
- L_UITest에서 몬스터 명중/빗나감/벽 명중/연속 명중/처치 명중을 확인한다. HUD를 숨긴 동안 발생한 알림은 재표시 때 재생하지 않는다.
- C++ Development Editor Win64 빌드 성공. WBP 이벤트 연결 및 PIE 애니메이션 재생은 에디터 확인이 필요하다.

## 데미지 숫자 표시 (2026-09-13)

- WBP_DamageNumber 설정이 필수다. 몬스터 피격 당시 위치 + Z 100에서 숫자가 0.7초 동안 위로 60 UI 단위 이동하며 사라진다. 디자인은 WBP에서 설정한다.
- 표시 값은 방어력 적용 후 실제 HP 감소량이며, 마지막 일격은 남은 HP까지만 표시한다. 소수점은 최대 1자리다.
- 카메라 이동 시 월드 위치를 다시 화면에 투영한다. 몬스터가 죽어 삭제되어도 저장한 위치에서 숫자가 유지된다.
- 총격/성수 명중마다 별도 숫자를 생성한다. 최대 32개이며 HUD 숨김/교체/종료 시 정리한다.
- 기존 On Hit Confirmed → HitConfirm 애니메이션 연결은 변경하지 않는다.

데미지 숫자 WBP 설정(필수):
1. /Game/01_PP/UI/WBP_DamageNumber를 생성하고 부모를 PriestDamageNumberWidget으로 선택한다.
2. Text Block을 루트로 두고 정확히 DamageText로 이름을 지정한다. Is Variable을 켠다. 또는 SizeBox 안에 DamageText를 배치한다.
3. 폰트/색/테두리/그림자를 편집한다. 화면 전체 Canvas 대신 숫자 크기의 레이아웃을 권장한다.
4. WBP Class Defaults의 Lifetime(기본 0.7), Rise Distance(기본 60)를 조절한다. Tick Frequency는 Auto를 유지한다.
5. WBP_PriestHUD의 Class Defaults → Priest | UI → Damage Number Widget Class에 WBP_DamageNumber를 지정한다.
6. 이동/페이드는 C++이 처리하므로 별도 이벤트 그래프/애니메이션은 필요 없다.

PIE 확인: 명중 숫자와 로그 비교, 빗나감에 표시 없음, 연속/범위 공격 여러 숫자, 마지막 일격 숫자, 카메라 회전, 해상도 변경, HUD 숨기기 및 재시작.

## 플레이어 피격 가장자리 효과

- 실제 HP가 감소할 때 피해받은 플레이어 컨트롤러 → UIManager → HUD로 전달한다.
- WBP_PriestHUD에 화면 전체를 채우는 Canvas Panel을 추가하고 이름을 DamageFeedback으로 지정한다. Is Variable을 켠다.
- 그 안에 Image 4개를 배치하여 상하좌우 단색 가장자리를 만든다. Brush의 Draw As는 Image, Tint는 붉은색(예: 0.8, 0, 0, 0.5)으로 설정한다.
- 예시 두께 60: 상단은 가로 Stretch/상단 고정, 높이 60. 하단은 가로 Stretch/하단 고정, Alignment Y=1, 높이 60. 좌우는 세로 Stretch/각 측면 고정, 너비 60, Top/Bottom 오프셋 60. 오른쪽은 Alignment X=1. 겹치지 않게 배치한다.
- DamageFeedback의 Render Opacity는 0, Visibility는 Not Hit-Testable (Self & All Children)로 설정한다. Hidden/Collapsed로 두지 않는다. Designer에서 편집할 때만 Opacity를 1로 올린다.
- 기본 지속 시간은 0.4초이며 연속 피격 시 다시 시작한다. Class Defaults → Priest | Damage Feedback에서 Enable Damage Feedback과 Duration을 조절한다. 색과 두께는 Designer에서 편집한다.
- WBP의 Tick Frequency는 Auto로 유지하고, HUD 루트는 플레이어 화면 전체를 채우도록 유지한다.
- 회복/체력 초기화/이미 HP 0인 상태에서는 표시하지 않는다. HideHUD 및 조종 캐릭터 변경 시 잔여 효과를 초기화한다.
- L_UITest에서 피격 후 0.4초 내 사라짐, 연속 피격, HP 0, 카메라/해상도 변경, HUD 숨김 후 재표시를 확인한다.

## 몬스터 처치 알림

- 공격으로 몬스터 HP가 0이 된 순간 공격자의 PlayerController에 ClientNotifyEnemyKilled를 보낸다. 일반 명중/벽 명중에는 보내지 않는다.
- 기존 명중 효과와 데미지 숫자는 유지한다. 총격과 성수 처치 모두 포함한다.
- WBP_PriestHUD의 루트 Canvas에 Text Block을 추가하고 이름을 KillNotification으로 지정한다. Is Variable을 켠다.
- Designer에서 문구를 몬스터 처치로 설정한다. 예시: 앵커 Min/Max (0.5, 0.3), Alignment (0.5, 0.5), Position (0, 0), Auto Size 켜기, 폰트 28, 노란색. 원하는 임포트 폰트를 지정할 수 있다.
- Render Opacity는 0, Visibility는 Not Hit-Testable (Self & All Children)로 설정한다. DamageFeedback의 자식으로 넣지 않는다.
- 지속 시간은 Class Defaults → Priest | Kill Notification의 Duration(기본 1.5초)에서 설정한다. 마지막 0.3초에 사라진다. 문구/색/폰트/위치는 Designer에서 편집한다.
- 연속 처치는 같은 문구의 지속 시간을 갱신한다. 처치 수 합산 또는 목록 표시는 하지 않는다.
- HUD 숨김/교체 및 조종 캐릭터 변경 시 알림을 초기화한다. Tick Frequency는 Auto를 유지한다.
- PIE 확인: 일반 명중에서는 미표시, 마지막 일격에서 표시, 마지막 데미지 숫자와 동시 표시, 성수 처치, 연속 처치, 표시 종료, HUD 숨김 후 재표시.

## PR 리뷰 반영: WBP 전환 (2026-09-14)

- C++ NativePaint와 DamageNumber RebuildWidget을 제거했다. 코드에서 위젯 생성/폰트/색/도형 배치를 하지 않는다.
- HUD의 DamageFeedback(UWidget 계열), KillNotification(TextBlock), 데미지 숫자의 DamageText(TextBlock)는 필수 BindWidget이다. 이름과 타입을 맞춘 뒤 각 WBP를 Compile/Save한다. 기존 HealthBar/HealthText/WeaponText/MissionText는 유지한다.
- WBP_DamageNumber를 HUD의 Damage Number Widget Class에 지정하지 않으면 숫자는 생성되지 않고 로그 경고가 출력된다. 네이티브 기본 위젯 대체 기능은 제거했다.
- C++은 데이터 갱신, 효과 표시 시간/불투명도, 데미지 숫자 월드 위치 투영/이동/수명 관리를 담당한다. 별도의 BP 타이머/Opacity 애니메이션을 연결하면 중복 제어되므로 필요 없다.
- 에디터 재시작 후 위 필수 WBP 설정을 완료하고 Compile한다. 필수 위젯이 누락된 기존 WBP는 컴파일 오류가 발생하므로 PIE 전에 수정한다.
- 검증: 초기 효과 숨김, 실제 피격과 연속 피격, 일반 명중과 처치 구분, 연속 처치, 숫자 표시와 소멸, 화면 크기 변경, 레벨 재시작.
