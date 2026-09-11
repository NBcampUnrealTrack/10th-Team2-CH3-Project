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
- MVC의 실제 HP 변경 전달과 무기 데이터 연결은 별도 작업이다. Initial HUD Data는 실제 캐릭터 상태를 자동 조회하지 않는다.

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
